#include<chrono>

//Returns time from start position as a float representing milliseconds
class MilliTimer{
private:
	std::chrono::high_resolution_clock::time_point start;
	

public:
	MilliTimer()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	void reset()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	float now()
	{
		std::chrono::high_resolution_clock::time_point curr = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float,std::milli> span = std::chrono::duration_cast<std::chrono::duration<float>>(curr - start);
		return span.count();
	}
};