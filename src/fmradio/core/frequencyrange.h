/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   frequencyrange.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef FREQUENCY_RANGE_T_H
#define FREQUENCY_RANGE_T_H

#include <cstdint>

namespace fmradio {

class frequency_range_t
{
public:
    enum {
        FREQ_MIN_CAMPUS_ON  = 760,
        FREQ_MIN_CAMPUS_OFF = 870,
        FREQ_MAX            = 1080,
    };
    frequency_range_t();

    inline uint32_t min() const { return _min; }
    inline uint32_t max() const { return _max; }

    bool set_campus_mode(bool);
    bool campus_mode() const;
    bool frequency_valid(uint32_t) const;
    uint32_t adjust_frequency(uint32_t) const;

private:
    bool _campus_mode = false;
    uint32_t _min = FREQ_MIN_CAMPUS_OFF;
    uint32_t _max = FREQ_MAX;
};

} // namespace fmtuner

#endif // FREQUENCY_RANGE_T_H
