#include <iostream>
#include "motorSim.h"

MotorSim::MotorSim(float stepAngle)
{
	wiringPiSetupSys();
	this->stepAngle = stepAngle;
	this->currPos = 0;
}

void MotorSim::rotate(float angle, float speed,bool dir)
{
	float dt = stepAngle/(2*speed);
	int numSteps = angle/stepAngle;
	for (int i = 0;i<numSteps;i++){
		if (dir==CW) currPos-=stepAngle;
		else currPos+=stepAngle;
		
		delay(dt);
		std::cout<<currPos<<std::endl;
		delay(dt);
		
	}
}

void MotorSim::rotateToPos(float newPos, float speed,bool dir)
{
	float angle;
	if (dir==CW) angle = currPos-newPos;
	else angle = newPos-currPos;
	if (angle<0) angle = -angle;
	rotate(angle,speed,dir);
}

void MotorSim::step(int dt, bool dir)
{
	if (dir==CW) currPos-=stepAngle;
	else currPos+=stepAngle;
	delay(dt/2);
	std::cout<<currPos<<std::endl;
	delay(dt/2);
}
void MotorSim::setZero()
{
	currPos=0;
}

float MotorSim::getCurrPos()
{
	return currPos;
}
