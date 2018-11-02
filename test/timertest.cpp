#include "MilliTimer.h"
#include <thread>
#include <chrono>

int main(){
	MilliTimer timer();

	for (int i = 0; i<5; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(i*1000));
		cout<<"Elapsed time: "<<timer.now()<<std::endl;
	}
}