/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   frequencyrange.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "frequencyrange.h"

using namespace fmradio;

frequency_range_t::frequency_range_t()
{}

bool frequency_range_t::set_campus_mode(bool value)
{
    if (value == _campus_mode)
        return false;

    _campus_mode = value;
    _min = _campus_mode ? FREQ_MIN_CAMPUS_ON : FREQ_MIN_CAMPUS_OFF;

    return true;
}

bool frequency_range_t::campus_mode() const
{
    return _campus_mode;
}

bool frequency_range_t::frequency_valid(uint32_t value) const
{
    return (value >= _min && value <= _max);
}

uint32_t frequency_range_t::adjust_frequency(uint32_t value) const
{
    if (value < _min) return _min;
    if (value > _max) return _max;
    return value;
}

