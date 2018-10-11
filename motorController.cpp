#include "motor/motorClass.h"
#include <wiringPi.h>
#include <iostream>

int main(int argc, char* argv [])
{
	int dirPin = 20;
	int stepPin = 21;
	//dirPin=20, stepPin=21
	StepperMotor step(dirPin,stepPin,0.9);

	while (true){
		float deg;
		int dir;
		std::cout<< "Degrees: ";
		std::cin>>deg;
		std::cout<<"Dir:";
		std::cin>>dir;
	
		step.rotate(deg,5,(bool)dir);
	}
}
