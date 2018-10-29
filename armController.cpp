#include "motor/motorClass.h"
#include <string>
#include <sstream>
#include <iostream>

/*
 * First parameter is motor number
 Commands:
 * setZero: sets corr pos to zero
 * setLim lim1 lim2: sets lower and upper limits, respectively
 * rotate(or r) angle delay dir: rotate a given angle wit a given delay towards dir
 * rotateTo(or rt) pos delay dir: rotate to given pos with delay in direction dir
 * 
 * Directions: CW=1, CCW=0
 * ex: 1 r 30 1 0 (rotate mot 1 30 deg ccw with delay of 1 millisecond)
*/
int main(){
	//dir,step,angle
	StepperMotor mot1(20,21,0.9);
	StepperMotor mot2(23,24,0.9);
	while (true){
		std::cout<<"Motor 1 Position: "<<mot1.getCurrPos()<<std::endl;
		std::cout<<"Motor 2 Position: "<<mot2.getCurrPos()<<std::endl;
		std::cout<<"Command (mot#, com, params): ";
		std::string command;
	 	getline(std::cin,command);
	 	std::istringstream stream(command);
	 	int motNum;
	 	std::string com;
	 	stream>>motNum>>com;
	 	if (com=="setZero") {
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
		 	if (com=="rotate"||com=="r"){
		 		if (motNum==1) {
					mot1.rotate(angle,delay,dir);
					std::cout<<"Rotating 1"<<std::endl;
				}
		 		else {
					mot2.rotate(angle,delay,dir);
					std::cout<<"Rotating 2"<<std::endl;
				}
		 		
		 	}
		 	else if (com=="rotateTo"||com=="rt"){
		 		if (motNum==1) mot1.rotateToPos(angle,delay,dir);
		 		else mot2.rotateToPos(angle,delay,dir);
		 	}
		 	else{
				continue;
			}
			stream.str("");
	}
}
}
