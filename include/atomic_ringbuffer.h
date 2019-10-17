#pragma once

#include "abstract_queue.h"

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

    void push(const T &element) override
    {
        if(!full())
        {
            m.lock();
            buffer[tail_ptr] = element;
            tail_ptr = (tail_ptr + 1) % CAPACITY;
            size_++;
            m.unlock();
        }
    }

    T pop() override
    {
        T retval;

        if(!empty())
        {
            m.lock();
            retval = buffer[head_ptr];
            head_ptr = (head_ptr + 1) % CAPACITY;
            size_--;
            m.unlock();
        }

        return retval;
    }

    void clear() override
    {
        m.lock();
        head_ptr = 0;
        tail_ptr = 0;
        size_ = 0;
        m.unlock();
    }

    private:
    vex::mutex m;
    T buffer[CAPACITY];
    size_t head_ptr;
    size_t tail_ptr;
    size_t size_;
};