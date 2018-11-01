#include <iostream>
#include "motorClass.h"

StepperMotor::StepperMotor(int dirPin, int stepPin, float stepAngle)
{
	wiringPiSetupGpio();
	this->dirPin = dirPin;
	this->stepPin = stepPin;
	this->stepAngle = stepAngle;
	this->currPos = 0;
	pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);
    limSet= false;
}

void StepperMotor::rotate(float angle, float time,bool dir)
{
	if (limSet){
		float targetPos = currPos;
		if (dir==CCW) targetPos += angle;
		else targetPos -= angle;
		if (targetPos>maxLim)
		{
			float newAngle = maxLim-currPos;
			rotate(newAngle, time, dir);
			return;
		}
		if (targetPos<minLim)
		{
			float newAngle = currPos-minLim;
			rotate(newAngle, time, dir);;
			return;
		}
	}
	int numSteps = angle/stepAngle;
	float dt = time;
	digitalWrite(dirPin,dir);
	for (int i = 0;i<numSteps;i++){
		digitalWrite(stepPin,HIGH);
		delay(dt);
		digitalWrite(stepPin,LOW);
		delay(dt);
		if (dir==CW) currPos-=stepAngle;
		else currPos+=stepAngle;
	}
}

void StepperMotor::rotateToPos(float newPos, float time,bool dir)
{
	if (limSet){
		if (newPos>maxLim)
		{
			rotateToPos(maxLim, time, CCW);
			return;
		}
		if (newPos<minLim)
		{
			rotate(minLim, time, CW);
			return;
		}
	}
	float angle;
	if (dir==CW) angle = currPos-newPos;
	else angle = newPos-currPos;
	if (angle<0) angle = -angle;
	rotate(angle,time,dir);
}

void StepperMotor::step(int dt, bool dir)
{
	digitalWrite(dirPin,dir);
	digitalWrite(stepPin,HIGH);
	delay(dt/2);
	digitalWrite(stepPin,LOW);
	delay(dt/2);
}
void StepperMotor::setZero()
{
	currPos=0;
}

float StepperMotor::getCurrPos()
{
	return currPos;
}

void StepperMotor::setLimits(float minLim, float maxLim)
{
	this->minLim=minLim;
	this->maxLim = maxLim;
	limSet=true;
}

void StepperMotor::resetLimits()
{
	limSet=false;
}
	
