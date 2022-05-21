#ifndef EVENT_H
#define EVENT_H

#include "memory/allocator.h"
#include "memory/objectpool.h"

namespace fmradio {

class event_handler_t;
struct event_t
{
    virtual ~event_t() = default;
    virtual void init(const char *) = 0;
    virtual void handle(event_handler_t *) = 0;
    virtual void release() = 0;
    virtual uint32_t id() const = 0;
};

template <typename T, size_t t_maxobj=5>
class typed_pool_event_t : public event_t
{
public:
    static T * get() { return pool().get(); }
    void init(const char *) override {}
    void release() override { pool().release(static_cast<T *>(this)); }
    uint32_t id() const override { return T::ID; }

protected:
    typedef object_pool_t<T, t_maxobj> object_pool;
    static object_pool & pool() { static object_pool pool; return pool; }
};

template <typename T>
class typed_static_event_t : public event_t
{
public:
    static T * get() { static T e; return &e; }
    void init(const char *) override {}
    void release() override {}
    uint32_t id() const override { return T::ID; }
};

struct event_factory_t
{
    virtual ~event_factory_t() = default;
    virtual event_t * get(const char *) = 0;
};

} // namespace fmtuner

#endif // EVENT_H
