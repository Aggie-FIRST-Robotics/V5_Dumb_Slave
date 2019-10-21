/*
 * A thread safe wrapper for a primitive.
 * 
 * @author John Sauer
 * @date 10/13/2019
 */

#pragma once

#include "vex.h"

/*
 * A thread safe primitive wrapper class
 */
template <class T>
class atomic_primitive
{
    public:

        /*
         * Default constructor.
         */
        atomic_primitive()
        {
        }

        /*
         * Default destructor.
         */
        ~atomic_primitive()
        {
        }

        /*
         * Constructor with initial value.
         */
        atomic_primitive(const T& initial_val)
        {
            m.lock();
            val_ = initial_val;
            m.unlock();
        }

        /*
         * Set the value of the primitive.
         *
         * @param val The value the primitive is set to.
         */
        void set_value(const T& val)
        {
            m.lock();
            val_ = val;
            m.unlock();
        }

        /*
         * Get the value of the primitive.
         *
         * @return The value of the primitive.
         */
        T get_value()
        {
            m.lock();
            T temp = val_;
            m.unlock();
            return temp;
        }

    private:

        /*
         * The mutex used for locking the object when it is being accessed
         * or modified.
         */
        vex::mutex m;

        /*
         * The internal primitive storing data.
         */
        T val_;
};