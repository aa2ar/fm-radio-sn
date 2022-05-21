/**
 * @file   eventinput.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef EVENTINPUT_H
#define EVENTINPUT_H

namespace fmradio {

struct event_t;
class event_input_t
{
public:
    virtual ~event_input_t() = default;
    virtual void enqueue(event_t *) = 0;
};

} // namespace fmtuner

#endif // EVENTINPUT_H

