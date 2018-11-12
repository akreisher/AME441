#include "ArmClass.h"
#include "motor/motorClass.h"
#include "support/Kinematics.h"
#include "support/RaceQueue.h"
#include <thread>


RobotArm::RobotArm(StepperInfo* motI,int speed){
	running = false;
	for (int i = 0; i<3;i++){
		mot[i]=StepperMotor(motI[i].dirPin,motI[i].stepPin,motI[i].angle);
	}
}

void RobotArm::reset()
{
	for (int i = 0;i<3;i++){
		mot[i].setZero();
	}
}

bool RobotArm::Move(Coordinates newPos)
{
	float* thetas = Kinematics.inverse(newPos);
	std::thread motThreads[3];
	bool posReached = true;
	for (int i = 0;i<3;i++)
	{
		motThread = std::thread(SendRot,i,thetas[i],&posReached);
	}
	for (int i = 0;i<3;i++){
		motThreads[i].join();
	}
	return posReached;

}

std::thread RobotArm::run(RaceQueue<Coordinates>& q)
{
	running = true;
	std::thread armthread(CatchUp,q);
	return thread;
}

void CatchUp(RaceQueue<Coordinates>& q)
{
	while (running)
	{
		Coordinates newPos = q.pop();
		move(newPos);
	}
}

void RobotArm::SendRot(int i, float angle, bool* posReached){
		/*
	if (mot[i].getCurrPos()+angle>lim[i] ||mot[i].getCurrPos()+angle<lim[i]){
		*posReached = false;
	}
	*/
	bool dir = CCW;
	if (angle<0){
		angle = -angle;
		dir = CW;
	}
	mot[i].rotateToPos(angle,5,dir);
}