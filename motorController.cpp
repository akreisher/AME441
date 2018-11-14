#include "motor/motorClass.h"
#include <wiringPi.h>
#include <iostream>
#include <stdlib.h>

int main(int argc, char* argv [])
{
	int dirPin, stepPin;
	float angle;
	if (argc>1){
		dirPin = atoi(argv[1]);
		stepPin= atoi(argv[2]);
		angle = atof(argv[3]);
	}
	else{
		dirPin = 20;
		
		stepPin = 21;
		angle = 0.9;
	}
	//dirPin=20, stepPin=21
	StepperMotor step(dirPin,stepPin,angle);

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
