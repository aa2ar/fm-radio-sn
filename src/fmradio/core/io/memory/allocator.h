#ifndef ALLOCATOR_H
#define ALLOCATOR_H

template <typename T>
class allocator_t
{
public:
    static T *  alloc()       { return new T(); }
    static void free(T * obj) { delete obj; }
};

#endif // ALLOCATOR_H

