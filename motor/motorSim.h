#include <wiringPi.h>
//Stepper simulator for RPI. Outputs to terminal
class MotorSim
{
	public:
		//constructor,takes step size, sets zero pos to curr pos
		MotorSim( float stepAngle);
		//perform one step
		void step(int dt, bool dir);
		//rotate a certain number of degrees
		void rotate(float angle, float speed,bool dir);
		//rotate to new set position
		void rotateToPos(float newPos, float speed,bool dir);
		//set current position to zero
		void setZero();
		//get current position
		float getCurrPos();
	private:
		float stepAngle;
		float currPos;
		const bool CW = 1;
		const bool CCW = 0;
};
