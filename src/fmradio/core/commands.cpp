/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   commands.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "commands.h"

#include <cstdlib>

status_command_t::status_command_t() : base_t("AT+RET") {}

void status_command_t::reset()
{
    _counter = 0;
}

bool status_command_t::eat(fmradio::event_t * e)
{
    if (response_id() == e->id()) ++_counter;
    return (_counter == 2);
}


volume_down_command_t::volume_down_command_t() : base_t("AT+VOLD") {}


volume_up_command_t::volume_up_command_t() : base_t("AT+VOLU") {}


set_frequency_command_t::set_frequency_command_t() : base_t("AT+FRE=") {}

void set_frequency_command_t::init(unsigned value)
{
    if (870 <= value && value <= 1080) {
        char buf[8];
        sprintf(buf, "%u", value);
        int len = strlen(buf);
        for (int i = 0; i < len; ++i)
            _buffer[7+i] = buf[i];
        _size = 7+len;
        _buffer[_size] = 0;
    }
}


scan_down_command_t::scan_down_command_t() : base_t("AT+SCAND") {}


scan_up_command_t::scan_up_command_t() : base_t("AT+SCANU") {}


pause_command_t::pause_command_t() : base_t("AT+PAUSE") {}
