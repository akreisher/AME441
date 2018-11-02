#include<chrono>

//Returns time from start position as a float representing milliseconds
class MilliTimer{
	using Clock = std::chrono::high_resolution_clock;
	Clock::time_point start;
	MilliTimer()
	{
		start = Clock::now();
	}

	void reset()
	{
		start = Clock::now();
	}

	float now()
	{
		Clock::time_point curr = Clock::now();
		duration<float> span = duration_cast<duration<float>>(curr - start);
		return span.count();
	}
}