#include "motor/motorClass.h"
#include "Kinematics.h"



class RobotArm
{
	RobotArm(StepperInfo* motI);

	void reset();

	int Move(Coordinates newPos);

private:
	StepperMotor mot[3];
	int moveToPos();
	bool running;
	const bool CW = 1;
	const bool CCW = 0;

};