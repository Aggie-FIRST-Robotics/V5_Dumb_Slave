#pragma once

#include "vex.h"
#include "abstract_queue.h"
#include "lockguard.h"

template <typename T, size_t CAPACITY>
class atomic_ringbuffer : public abstract_queue<T>
{
    public:
    atomic_ringbuffer<T, CAPACITY>() :
        head_ptr(0),
        tail_ptr(0),
        size_(0)
    {
    }

    ~atomic_ringbuffer<T, CAPACITY>()
    {
    }

    size_t size() override
    {
        return size_;
    }

    bool empty() override
    {
        return size_ == 0;
    }

    bool full() override
    {
        return size_ == CAPACITY;
    }

    size_t capacity() override
    {
        return CAPACITY;
    }

    bool push(const T &element) override
    {
        if(!full())
        {
            lockguard lock(m);
            buffer[tail_ptr] = element;
            tail_ptr = (tail_ptr + 1) % CAPACITY;
            size_++;
            return true;
        }

        return false;
    }

    bool pop(T &element) override
    {
        if(!empty())
        {
            lockguard lock(m);
            element = buffer[head_ptr];
            head_ptr = (head_ptr + 1) % CAPACITY;
            size_--;
            return true;
        }

        return false;
    }

    bool clear() override
    {
        lockguard lock(m);
        head_ptr = 0;
        tail_ptr = 0;
        size_ = 0;
        return true;
    }

    private:
    vex::mutex m;
    T buffer[CAPACITY];
    size_t head_ptr;
    size_t tail_ptr;
    size_t size_;
};