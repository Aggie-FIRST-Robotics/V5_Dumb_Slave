#pragma once

#include <cstdlib>
#include <cstdint>

template <typename T>
class abstract_queue
{
    public:
        
    virtual void push(const T &element) = 0;
    virtual T pop() = 0;

    virtual size_t size() = 0;

    virtual size_t capacity() = 0;

    virtual bool empty() = 0;

    virtual bool full() = 0;

    virtual void clear() = 0;
};