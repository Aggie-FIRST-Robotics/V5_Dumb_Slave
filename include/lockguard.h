/*
 * RAII lock guard for clean mutex handling.
 *
 * @author John Sauer
 * @date 10/22/2019
 */

#pragma once

#include "vex.h"

class lockguard
{
public:
	lockguard(vex::mutex &mx)
	{
		mx_ = &mx;
		mx_->lock();
	}

	~lockguard()
	{
		mx_->unlock();
	}

private:
	lockguard()
	{
	}

	vex::mutex *mx_;
};