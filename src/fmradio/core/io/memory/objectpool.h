#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <vector>
#include <mutex>
#include "allocator.h"

template <typename T, size_t t_maxobj, typename TAllocator=allocator_t<T> >
class object_pool_t
{
public:
    object_pool_t() {
        _objects.reserve(t_maxobj);
    }

    T * get() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_objects.empty())
            return TAllocator::alloc();
        T * obj = _objects.back();
        _objects.pop_back();
        return obj;
    }

    void release(T * obj) {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_objects.size() >= t_maxobj)
            TAllocator::free(obj);
        else
            _objects.push_back(obj);
    }

private:
    std::vector<T *> _objects;
    std::mutex       _mutex;
};

#endif // OBJECTPOOL_H
