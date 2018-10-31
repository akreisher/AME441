#include "motor/motorClass.h"
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
/*
 * First parameter is motor number (12 for both, use 2 rotation commands)
 * Commands:
 * setZero: sets corr pos to zero
 * setLim lim1 lim2: sets lower and upper limits, respectively
 * rotate(or r) angle delay dir: rotate a given angle wit a given delay towards dir
 * rotateTo(or rt) pos delay dir: rotate to given pos with delay in direction dir
 * 
 * Directions: CW=1, CCW=0
 * ex: 1 r 30 1 0 (rotate mot 1 30 deg ccw with delay of 1 millisecond)
*/

void sendRotCommand(StepperMotor* mot, std::string com, float angle, long delay, bool dir){
	if (com=="rotate" || com=="r"){
		mot->rotate(angle,delay,dir);
	}
	else if (com=="rotateTo" || com=="rt"){
		mot->rotateToPos(angle,delay,dir);
	}
}

int main(){
	//dir,step,angle
	StepperMotor mot1(20,21,0.9);
	StepperMotor mot2(23,24,1.8);
	while (true){
		std::cout<<"Motor 1 Position: "<<mot1.getCurrPos()<<std::endl;
		std::cout<<"Motor 2 Position: "<<mot2.getCurrPos()<<std::endl;
		std::cout<<"Command (mot#, com, Angle, dir, delay): ";
		std::string command;
	 	getline(std::cin,command);
	 	std::istringstream stream(command);
	 	int motNum;
	 	std::string com;
	 	stream>>motNum>>com;
	 	if (com=="setZero"){
	 		if (motNum==1) mot1.setZero();
	 		else mot2.setZero();
	 	}
	 	else if (com=="setLim"){
	 		float lim1, lim2;
	 		stream>>lim1>>lim2;
	 		if (motNum==1) mot1.setLimits(lim1,lim2);
	 		else mot2.setLimits(lim1,lim2);
	 	}
	 	else{
			float angle;
		 	bool dir;
		 	long delay;
		 	stream>>angle>>delay>>dir;
			if (motNum==1){
				std::thread mot1Thread(sendRotCommand,&mot1,com,angle,delay,dir);
				mot1Thread.join();
			}
			else if (motNum==2){
				std::thread mot2Thread(sendRotCommand,&mot2,com,angle,delay,dir);
				mot2Thread.join();
			}
			else if (motNum==12){
					std::cout<<"Aww"<<std::endl;
					std::string com2;
					float angle2;
				 	bool dir2;
				 	long delay2;
				 	stream>>angle2>>delay2>>dir2;
				 	std::thread mot1Thread(sendRotCommand,&mot1,com,angle,delay,dir);
					std::thread mot2Thread(sendRotCommand,&mot2,com2,angle2,delay2,dir2);
					mot1Thread.join();
					mot2Thread.join();
			}
		}
	}
}
