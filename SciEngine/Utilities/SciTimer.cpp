#include <chrono>
#include "SciTimer.h"

using namespace std::chrono;

typedef std::chrono::high_resolution_clock Clock;

// Usefull functions
SciTime getCurTime()
{
	Clock::time_point cur_time;
	cur_time = Clock::now();
	return cur_time.time_since_epoch().count();
}
// End of Usefull functions

// Constructors
SciTimer::SciTimer() : ups_(60), t_(0.0f), accum_t(0.0f)
{
	dt_ = 1.0f / (float)ups_;
	
	past_t = getCurTime();
}

SciTimer::SciTimer(int ups) : ups_(ups), t_(0.0f), accum_t(0.0f)
{
	dt_ = 1.0f / (float)ups_;

	past_t = getCurTime();
}
// End of Constructors

// Accsess functions
void SciTimer::SetUPS(int ups)
{
	ups_ = ups;
	dt_ = 1.0f / (float)ups_;
}
// End of Accsess functions

// Time manipulation
void SciTimer::AccumulateTime()
{
	SciTime new_t = getCurTime();
	// Get past frame time in seconds
	float past_frame_t = (float)(new_t - past_t) / Clock::period::den;
	past_t = new_t;
	// Accumulate time
	accum_t += past_frame_t;
}

void SciTimer::UpdateTime()
{
	t_ += dt_;
	accum_t -= dt_;
}
// End of Time manipulation