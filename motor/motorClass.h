#include <wiringPi.h>
//Stepper class for RPI. Compile with -lwiringPi flag


struct StepperInfo
{
	int dirPin, stepPin;
	float angle;
};

class StepperMotor 
{
	public:
		//constructor, takes pins and step size, sets zero pos to curr pos
		StepperMotor(int dirPin, int stepPin, float stepAngle);
		StepperMotor(StepperInfo motI):StepperMotor(motI.dirPin,motI.stepPin,motI.angle){}
		//perform one step
		void step(int time, bool dir);
		//rotate a certain number of degrees
		void rotate(float angle, float time,bool dir);
		//rotate to new set position
		void rotateToPos(float newPos, float time,bool dir);
		//set current position to zero
		void setZero();
		//get current position
		float getCurrPos();
		//set motor limits
		void setLimits(float minLim,float maxLim);
		//turn off limits
		void resetLimits();
	private:
		float minLim;
		float maxLim;
		bool limSet;
		int dirPin;
		int stepPin;
		float stepAngle;
		float currPos;
		const bool CW = 1;
		const bool CCW = 0;
};
