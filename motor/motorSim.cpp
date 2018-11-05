#include <iostream>
#include "motorSim.h"


MotorSim::MotorSim(float stepAngle, std::ostream* os)
{
	wiringPiSetupSys();
	this->os = os;
	this->stepAngle = stepAngle;
	this->currPos = 0;
	timer=MilliTimer();
	
}

void MotorSim::rotate(float angle, float time, bool dir)
{
	int numSteps = angle/stepAngle;
	for (int i = 0;i<numSteps;i++){
		if (dir==CW) currPos-=stepAngle;
		else currPos+=stepAngle;
		delay(time);
		*os<<timer.now()<<","<<currPos<<std::endl;
		delay(time);
		
	}
}

void MotorSim::rotateToPos(float newPos, float time,bool dir)
{
	float angle;
	if (dir==CW) angle = currPos-newPos;
	else angle = newPos-currPos;
	if (angle<0) angle = -angle;
	rotate(angle,time,dir);
}

void MotorSim::step(int dt, bool dir)
{
	if (dir==CW) currPos-=stepAngle;
	else currPos+=stepAngle;
	delay(dt/2);
	*os<<currPos<<std::endl;
	delay(dt/2);
}
void MotorSim::setZero()
{
	currPos=0;
	timer.reset();
}

float MotorSim::getCurrPos()
{
	return currPos;
}
