/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   inputlogger.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef INPUT_LOGGER_T_H
#define INPUT_LOGGER_T_H

#include <cstddef>

namespace fmradio {

class io_logger_t
{
public:
    virtual ~io_logger_t() = default;
    virtual void log_input(const char *) = 0;
    virtual void log_output(const char *) = 0;
    virtual void log_info(const char *) = 0;
};

} // namespace fmtuner

#endif // INPUT_LOGGER_T_H
