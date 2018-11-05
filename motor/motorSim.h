#include <wiringPi.h>
#include <ostream>
#include "../MilliTimer.h"
//Stepper simulator for RPI. Outputs to terminal
class MotorSim
{
	public:
		//constructor,takes step size, sets zero pos to curr pos
		MotorSim(float stepAngle,std::ostream* os);
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
		std::ostream* os;
		float stepAngle;
		float currPos;
		const bool CW = 1;
		const bool CCW = 0;
		MilliTimer timer;
		
};
