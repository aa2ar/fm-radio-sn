/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   eventhandler.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef EVENHANDLER_H
#define EVENHANDLER_H

namespace fmradio {

struct event_t;
class volume_event_t;
class frequency_event_t;
class campus_on_event_t;
class campus_off_event_t;
class channel_event_t;
class scan_event_t;
class control_play_event_t;
class control_pause_event_t;
class earphones_in_event_t;
class earphones_out_event_t;
class audio_mode_event_t;
class version_event_t;
class event_handler_t
{
public:
    virtual ~event_handler_t() = default;
    virtual void handle_event(event_t *) {}
    virtual void handle_event(volume_event_t *) {}
    virtual void handle_event(frequency_event_t *)  {}
    virtual void handle_event(campus_on_event_t *)  {}
    virtual void handle_event(campus_off_event_t *) {}
    virtual void handle_event(channel_event_t *) {}
    virtual void handle_event(scan_event_t *) {}
    virtual void handle_event(control_play_event_t *) {}
    virtual void handle_event(control_pause_event_t *) {}
    virtual void handle_event(earphones_in_event_t *) {}
    virtual void handle_event(earphones_out_event_t *) {}
    virtual void handle_event(audio_mode_event_t *) {}
    virtual void handle_event(version_event_t *) {}
};

} // namespace fmtuner

#endif // EVENHANDLER_H
