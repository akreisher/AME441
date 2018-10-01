#include <wiringPi.h>
//Stepper class for RPI. Compile with -lwiringPi flag
class StepperMotor 
{
	public:
		//constructor, takes pins and step size, sets zero pos to curr pos
		StepperMotor(int dirPin, int stepPin, float stepAngle);
		//perform one step
		void step(int dt, bool dir);
		//rotate a certain number of degrees
		void rotate(float angle, float time,bool dir);
		//rotate to new set position
		void rotateToPos(float newPos, float time,bool dir);
		//set current position to zero
		void setZero();
		//get current position
		float getCurrPos();
	private:
		int dirPin;
		int stepPin;
		float stepAngle;
		float currPos;
		const bool CW = 1;
		const bool CCW = 0;
};
