#include "motor/motorClass.h"
#include <wiringPi.h>
#include <iostream>

int main()
{
	StepperMotor step(20,21,0.9);
	while (true){
		float deg;

		std::cout<< "Degrees: ";
		std::cin>>deg;
		step.rotate(deg,1,1);
	}
}
