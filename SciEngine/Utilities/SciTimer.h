#ifndef SCITIMER_H
#define SCITIMER_H

typedef long long SciTime;

class SciTimer
{
public:
	// Constructors
	SciTimer();
	SciTimer(int ups);

	// Accsess functions
	int GetUPS() const { return ups_; }
	void SetUPS(int ups);
	float Dt() const { return dt_; }
	float Alpha() const { return accum_t / dt_; } // Interpolation factor [0, 1]
	float TotalTime() const { return t_; }

	// Time manipulation
	void AccumulateTime();
	bool AccumEnoughTime() const { return accum_t >= dt_; }
	void UpdateTime();

private:
	// Data
	int ups_; // Updates Per Second
	float dt_; // Delta time (sec)
	float accum_t; // Time accumulated by past frames (sec)
	float t_; // Total time (sec)
	SciTime past_t; // Past time (start of previous frame) (nanosec)
};

#endif /* SCITIMER_H */