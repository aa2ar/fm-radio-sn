/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   commands.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include "io/command.h"
#include "io/event.h"
#include "ids.h"

template <typename T, size_t t_size, uint32_t t_resp_id=0, size_t t_maxobj=5>
class response_command_t
        : public fmradio::typed_pool_command_t<T, t_size, t_resp_id, t_maxobj>
{
    typedef fmradio::typed_pool_command_t<T, t_size, t_resp_id, t_maxobj> base_t;
public:
    response_command_t(const char * cmd_base) : base_t(cmd_base) {}
    bool eat(fmradio::event_t * e) override { return e->id() == t_resp_id; }
};

template <typename T, size_t t_size, uint32_t t_resp_id=0, size_t t_maxobj=5>
class error_response_command_t
        : public fmradio::typed_pool_command_t<T, t_size, t_resp_id, t_maxobj>
{
    typedef fmradio::typed_pool_command_t<T, t_size, t_resp_id, t_maxobj> base_t;
public:
    error_response_command_t(const char * cmd_base) : base_t(cmd_base) {}
    bool eat(fmradio::event_t * e) override { return (e->id() == t_resp_id || e->id() == fmradio::ids::error_id); }
};

class status_command_t
        : public response_command_t<status_command_t, 6, fmradio::ids::ret_id>
{
    typedef response_command_t<status_command_t, 6, fmradio::ids::ret_id> base_t;
public:
    status_command_t();
    void reset() override;
    bool eat(fmradio::event_t * e) override;
private:
    int _counter = 0;
};

class volume_down_command_t
        : public error_response_command_t<volume_down_command_t, 7, fmradio::ids::volume_id>
{
    typedef error_response_command_t<volume_down_command_t, 7, fmradio::ids::volume_id> base_t;
public:
    volume_down_command_t();
};

class volume_up_command_t
        : public error_response_command_t<volume_up_command_t, 7, fmradio::ids::volume_id>
{
    typedef error_response_command_t<volume_up_command_t, 7, fmradio::ids::volume_id> base_t;
public:
    volume_up_command_t();
};

class set_frequency_command_t
        : public error_response_command_t<set_frequency_command_t, 11, fmradio::ids::frequency_id>
{
    typedef error_response_command_t<set_frequency_command_t, 11, fmradio::ids::frequency_id> base_t;
public:
    set_frequency_command_t();
    void init(unsigned);
};

class scan_down_command_t
        : public fmradio::typed_pool_command_t<scan_down_command_t, 8>
{
    typedef fmradio::typed_pool_command_t<scan_down_command_t, 8> base_t;
public:
    scan_down_command_t();
};

class scan_up_command_t
        : public fmradio::typed_pool_command_t<scan_up_command_t, 8>
{
    typedef fmradio::typed_pool_command_t<scan_up_command_t, 8> base_t;
public:
    scan_up_command_t();
};

class pause_command_t
        : public fmradio::typed_pool_command_t<pause_command_t, 8>
{
    typedef fmradio::typed_pool_command_t<pause_command_t, 8> base_t;
public:
    pause_command_t();
};

#endif // COMMANDS_H
