/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   events.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "events.h"
#include "eventhandler.h"
#include <cstdlib>
#include <cstring>

using namespace fmradio;

template <typename T>
class typed_event_factory_t : public event_factory_t
{
public:
    typed_event_factory_t(const char * et)
        : _event_template(et)
    {
        _ev_t_len = strlen(_event_template);
    }

    T * get(const char * data) override {
        T * p = nullptr;
        if (strstr(data, _event_template) == data) {
            p = T::get();
            p->init(data + _ev_t_len);
        }
        return p;
    }

private:
    const char * _event_template = nullptr;
    size_t _ev_t_len = 0;
};

typed_event_factory_t<volume_event_t>    volume_event_factory("VOL=");
typed_event_factory_t<frequency_event_t> frequency_event_factory("FRE=");
typed_event_factory_t<campus_on_event_t> campus_on_event_factory("CAMPUS_ON");
typed_event_factory_t<campus_off_event_t> campus_off_event_factory("CAMPUS_OFF");
typed_event_factory_t<channel_event_t> channel_event_factory("CH=");
typed_event_factory_t<scan_event_t> scan_event_factory("SCAN");
typed_event_factory_t<control_play_event_t> control_play_event_factory("PLAY");
typed_event_factory_t<control_pause_event_t> control_pause_event_factory("PAUSE");
typed_event_factory_t<earphones_in_event_t> earphones_in_event_factory("earphone_in");
typed_event_factory_t<earphones_out_event_t> earphones_out_event_factory("earphone_out");
typed_event_factory_t<audio_mode_event_t> audio_mode_event_factory("Audio_mode: ");
typed_event_factory_t<version_event_t> version_event_factory("FM_RADIO_SN_V");
typed_event_factory_t<error_event_t> error_event_factory("ERR");
typed_event_factory_t<ret_event_t> ret_event_factory("/");

enum { EVENT_NUM = 14 };
event_factory_t * event_templates[] = {
    &volume_event_factory,
    &frequency_event_factory,
    &campus_on_event_factory,
    &campus_off_event_factory,
    &channel_event_factory,
    &scan_event_factory,
    &control_play_event_factory,
    &control_pause_event_factory,
    &earphones_in_event_factory,
    &earphones_out_event_factory,
    &audio_mode_event_factory,
    &version_event_factory,
    &error_event_factory,
    &ret_event_factory
};

event_t * fmradio::get_event(const char * data)
{
    for (int i = 0; i < EVENT_NUM; ++i) {
        event_factory_t * ef = event_templates[i];
        event_t * e = ef->get(data);
        if (e)
            return e;
    }
    return nullptr;
}


void volume_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}


void frequency_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}


void campus_on_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}


void campus_off_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}


void scan_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}

void scan_event_t::init(const char * data)
{
    if (*data == '\0')
        _value = START;
    else if (strstr(data, "OK") != 0)
        _value = END;
    else  if (strstr(data, "U") != 0)
        _value = UP;
    else  if (strstr(data, "D") != 0)
        _value = DOWN;
    else
        _value = UNDEFINED;
}


void channel_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}


void control_play_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}


void control_pause_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}


void earphones_in_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}


void earphones_out_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}


void audio_mode_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}

void audio_mode_event_t::init(const char * data)
{
    _value = strstr(data, "S") != nullptr;
}


void version_event_t::handle(event_handler_t * eh)
{
    eh->handle_event(this);
}

void version_event_t::init(const char * data)
{
    strncpy(_value, data, 16);
}


void ret_event_t::handle(event_handler_t *)
{}


