#include "motor/motorClass.h"
#include <string>
#include <sstream>

int main(){
	//dir,step,angle
	StepperMotor mot1(20,21,0.9);
	StepperMotor mot2(18,19,0.9);
	while (true){
		std::cout<<"Motor 1 Position: "<<mot1.getCurrPos()<<std::endl;
		std::cout<<"Motor 2 Position: "<<mot1.getCurrPos()<<std::endl;
		std::cout<<"Command (mot#, com, Angle, dir, delay): ";
		string command;
	 	std::cin>>command;
	 	std::istringstream stream(command);
	 	int motNum;
	 	string com;
	 	stream>>motNum>>com;
	 	if (com.equals("setZero")) {
	 		if (motNum==1) mot1.setZero();
	 		else mot2.setZero();
	 	}
	 	else{
			float angle;
		 	bool dir;
		 	long delay;
		 	stream>>angle>>dir>>delay;
		 	if (com.equals("rotate")){
		 		if (motNum==1) mot1.rotate(angle,delay,dir);
		 		else mot2.rotate(angle,delay,dir);
		 	}
		 	if (com.equals("rotateTo")){
		 		if (motNum==1) mot1.rotateToPos(angle,delay,dir);
		 		else mot2.rotateToPos(angle,delay,dir);
		 	}
	}
}