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
}

void StepperMotor::rotate(float angle, float speed,bool dir)
{
	float dt = stepAngle/(2*speed);
	int numSteps = angle/stepAngle;
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

void StepperMotor::rotateToPos(float newPos, float speed,bool dir)
{
	float angle;
	if (dir==CW) angle = currPos-newPos;
	else angle = newPos-currPos;
	if (angle<0) angle = -angle;
	rotate(angle,speed,dir);
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
