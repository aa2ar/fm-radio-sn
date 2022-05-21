#ifndef COMMAND_H
#define COMMAND_H

#include "pdu.h"
#include "memory/allocator.h"
#include "memory/objectpool.h"
#include <cstring>

namespace fmradio {

struct event_t;
class command_t : public pdu_t
{
public:
    virtual void release() = 0;
    virtual void reset() {}
    virtual bool eat(event_t *) { return false; }
    virtual uint32_t response_id() const { return 0; }
};

template <typename T, size_t t_size, uint32_t t_resp_id=0, size_t t_maxobj=5>
class typed_pool_command_t : public command_t
{
public:
    typed_pool_command_t(const char * cmd_base) {
        _data = _buffer;
        _size = t_size;
        strncpy(_buffer, cmd_base, t_size+1);
    }

    static T * get() { T * c = pool().get(); c->reset(); return c; }
    void release() { pool().release(static_cast<T *>(this)); }
    uint32_t response_id() const { return t_resp_id; }

protected:
    char _buffer[t_size+1];

    typedef object_pool_t<T, t_maxobj> object_pool;
    static object_pool & pool() { static object_pool pool; return pool; }
};

template <typename T, size_t t_size>
class typed_static_command_t : public command_t
{
public:
    typed_static_command_t(const char * cmd_base) {
        _data = _buffer;
        _size = t_size;
        strncpy(_buffer, cmd_base, t_size);
    }

    static T * get() { static T e; e.reset(); return &e; }
    void release() {}
    uint32_t response_id() const { return 0; }

protected:
    char _buffer[t_size+1];
};

} // namespace fmtuner

#endif // COMMAND_H
