/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   controller.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "controller.h"
#include "io/command.h"
#include "io/protocol.h"
#include "io/output.h"
#include "events.h"
#include "inputlogger.h"
#include "eventinput.h"
#include <cstdio>

#define USE_CONTROLLER_LOG 1

#if USE_CONTROLLER_LOG
#define CONTROLLER_LOG(args) {printf args; fflush(stdout);}
#else
#define CONTROLLER_LOG (void)0;
#endif


using namespace fmradio;

controller_t::controller_t(protocol_t * p, event_input_t * ein, output_t * out, io_logger_t * logger)
    : _p(p)
    , _ei(ein)
    , _out(out)
    , _logger(logger)
    , _log_enabled(false)
    , _log_only_unhandled(false)
{
    _tp_add_wait_resp = std::chrono::system_clock::now();
}

void controller_t::on_input(const char * data, size_t size)
{
    if (_p->parse(data, size)) {
        const char * data = 0;
        while ((data = _p->get()) != 0) {
            event_t * e = get_event(data);
            if (e) {

                { std::lock_guard<std::mutex> lock(_mutex);

                    if (_wait_resp) {
                        if (_wait_resp->eat(e)) {

                            //CONTROLLER_LOG(("received event id: %u\n", id))

                            _wait_resp->release();
                            _wait_resp = nullptr;
                        }
                        else {
                            using namespace std::chrono;
                            if (system_clock::now() - _tp_add_wait_resp > milliseconds(WAIT_RESP_TMO)) {

                                CONTROLLER_LOG(("WAIT_RESP_TMO\n"))
                                _logger->log_info("WAIT_RESP_TMO");

                                // resend
                                _queue.push(_wait_resp);
                                _wait_resp = nullptr;
                            }
                        }
                    }
                    _notified = true;
                    _cv.notify_one();
                }

                _ei->enqueue(e);
            }

            log_output(data, e == nullptr);
        }
    }
}

void controller_t::start()
{
    std::thread t(&controller_t::run, this);
    _id = t.get_id();
    t.detach();
}

void controller_t::stop()
{
    _stop.store(true);
    _notified = true;
    _cv.notify_one();
}

void controller_t::send(command_t * c)
{
    using namespace std::chrono;

    if (c == nullptr)
        return;

    std::lock_guard<std::mutex> locker(_mutex);

    if (_wait_resp) {
        if (system_clock::now() - _tp_add_wait_resp > milliseconds(WAIT_RESP_TMO)) {
            //_wait_resp->release();

            CONTROLLER_LOG(("WAIT_RESP_TMO\n"))
            _logger->log_info("WAIT_RESP_TMO");

            // resend
            _queue.push(_wait_resp);
            _wait_resp = nullptr;

//            _queue.push(c);
//            _notified = true;
//            _cv.notify_one();
        }
//        else if (_wait_resp->response_id() == c->response_id())
//            c->release();

        _queue.push(c);
        _notified = true;
        _cv.notify_one();
    }
    else {

        //CONTROLLER_LOG(("add command: %u\n", c->response_id()))

        _queue.push(c);
        _notified = true;
        _cv.notify_one();
    }
}

void controller_t::enable_log(bool value)
{
    _log_enabled.store(value);
}

void controller_t::log_only_unhandled(bool value)
{
    _log_only_unhandled.store(value);
}

void controller_t::run()
{
    _stop.store(false);

    while (1) {

        std::unique_lock<std::mutex> locker(_mutex);
        while(!_notified) // от ложных пробуждений
            _cv.wait(locker);

        if (_stop.load())
            break;

        if (_wait_resp == nullptr) {

            while (!_queue.empty()) {

                command_t * c = _queue.front();
                _queue.pop();

                if (c->response_id() == 0) {
                    _out->write(c->data(), c->size());
                    c->release();
                }
                else {
                    //CONTROLLER_LOG(("set _wait_resp: %u\n", c->response_id()))

                    _wait_resp = c;
                    _tp_add_wait_resp = std::chrono::system_clock::now();
                    _out->write(c->data(), c->size());

                    log_input(c->data());
                }

            }

        }
        _notified = false;
    }
    printf("controller_t::stopped\n");
    fflush(stdout);
}

void controller_t::log_input(const char * data)
{
    if (_log_enabled.load())
        _logger->log_input(data);
}

void controller_t::log_output(const char * data, bool unhandled)
{
    if (_log_enabled.load()) {
        if (_log_only_unhandled.load()) {
            if (unhandled)
                _logger->log_output(data);
        }
        else
            _logger->log_output(data);
    }
}

