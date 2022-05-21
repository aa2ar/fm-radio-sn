/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   events.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef EVENTS_H
#define EVENTS_H

#include "io/event.h"
#include "ids.h"
#include <cstddef>

namespace fmradio {

template <typename T>
class int_event_t : public typed_pool_event_t<T>
{
public:
    void init(const char * data) override {
        _value = atoi(data);
    }

    inline int value() const { return _value; }

protected:
    int _value = 0;
};

class volume_event_t : public int_event_t<volume_event_t>
{
public:
    enum { ID = ids::volume_id };
    void handle(event_handler_t *) override;
};

class frequency_event_t : public int_event_t<frequency_event_t>
{
public:
    enum { ID = ids::frequency_id };
    void handle(event_handler_t *) override;
};

class campus_on_event_t : public typed_static_event_t<campus_on_event_t>
{
public:
    enum { ID = ids::campus_on_id };
    void handle(event_handler_t *) override;
};

class campus_off_event_t : public typed_static_event_t<campus_off_event_t>
{
public:
    enum { ID = ids::campus_off_id };
    void handle(event_handler_t *) override;
};

class scan_event_t : public typed_pool_event_t<scan_event_t>
{
public:
    enum { ID = ids::scan_id };
    void handle(event_handler_t *) override;
    void init(const char * data) override;

    enum value_t { UNDEFINED, START, END, UP, DOWN };
    inline value_t value() const { return _value; }

protected:
    value_t _value = UNDEFINED;
};

class channel_event_t : public int_event_t<channel_event_t>
{
public:
    enum { ID = ids::channel_id };
    void handle(event_handler_t *) override;
};

class control_play_event_t : public typed_static_event_t<control_play_event_t>
{
public:
    enum { ID = ids::control_play_id };
    void handle(event_handler_t *) override;
};

class control_pause_event_t : public typed_static_event_t<control_pause_event_t>
{
public:
    enum { ID = ids::control_pause_id };
    void handle(event_handler_t *) override;
};

class earphones_in_event_t : public typed_static_event_t<earphones_in_event_t>
{
public:
    enum { ID = ids::earphones_in_id };
    void handle(event_handler_t *) override;
};

class earphones_out_event_t : public typed_static_event_t<earphones_out_event_t>
{
public:
    enum { ID = ids::earphones_out_id };
    void handle(event_handler_t *) override;
};

class audio_mode_event_t : public typed_static_event_t<audio_mode_event_t>
{
public:
    enum { ID = ids::audio_mode_id };
    void handle(event_handler_t *) override;
    void init(const char * data) override;
    inline bool value() const { return _value; } // true - stereo, false - mono
private:
    bool _value = true;

};

class version_event_t : public typed_static_event_t<version_event_t>
{
public:
    enum { ID = ids::version_id };
    void handle(event_handler_t *) override;
    void init(const char * data) override;
    inline const char * value() const { return _value; }
private:
    char _value[16] = {0};

};

class ret_event_t : public typed_static_event_t<ret_event_t>
{
public:
    enum { ID = ids::ret_id };
    void handle(event_handler_t *) override;
private:
};

class error_event_t : public typed_static_event_t<error_event_t>
{
public:
    enum { ID = ids::error_id };
    void handle(event_handler_t *) override {}
private:
};

event_t * get_event(const char *);

} // namespace fmtuner

#endif // EVENTS_H
