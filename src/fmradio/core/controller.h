/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   controller.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <cstddef>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <queue>
#include <chrono>

namespace fmradio {

class protocol_t;
class output_t;
class io_logger_t;
class command_t;
class event_input_t;
class controller_t
{
public:
    controller_t(protocol_t *, event_input_t *, output_t *, io_logger_t *);

    void on_input(const char *, size_t);

    void start();
    void stop();

    void send(command_t *);

    void enable_log(bool);
    void log_only_unhandled(bool);

private:
    enum { WAIT_RESP_TMO = 1000 /*ms*/ };

    protocol_t * _p = nullptr;
    event_input_t * _ei = nullptr;
    output_t * _out = nullptr;
    io_logger_t * _logger = nullptr;
    std::atomic_bool _log_enabled;
    std::atomic_bool _log_only_unhandled;

    std::thread::id _id;
    std::atomic_bool _stop;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::queue<command_t *> _queue;
    command_t * _wait_resp = nullptr;
    std::chrono::time_point<std::chrono::system_clock> _tp_add_wait_resp;
    bool _notified = false;
    void run();
    void log_input(const char *);
    void log_output(const char *, bool);
};

} // namespace fmtuner

#endif // CONTROLLER_H
