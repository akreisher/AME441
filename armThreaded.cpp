#include "motor/motorClass.h"
#include <string>
#include <sstream>
#include <iostream>
#include <thread>

void sendRotCommand(StepperMotor* mot, string com, float angle, long delay, bool dir){
	if (com=="rotate" || com=="r"){
		mot1->rotate(angle,delay,dir);
	else if (com=="rotateTo" || com=="rt"){
		mot1->rotateToPos(angle,delay,dir);
	}
}

int main(){
	//dir,step,angle
	StepperMotor mot1(20,21,0.9);
	StepperMotor mot2(18,19,0.9);
	while (true){
		std::cout<<"Motor 1 Position: "<<mot1.getCurrPos()<<std::endl;
		std::cout<<"Motor 2 Position: "<<mot2.getCurrPos()<<std::endl;
		std::cout<<"Command (mot#, com, Angle, dir, delay): ";
		string command;
	 	getline(std::cin,command);
	 	std::istringstream stream(command);
	 	int motNum;
	 	string com;
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
				std::thread mot1Thread(&mot1,com,angle,delay,dir);
				mot1Thread.join();
			}
			else if (motNum==2){
				std::thread mot2Thread(&mot2,com,angle,delay,dir);
				mot2Thread.join();
			}
			else if (motNum==12){
				if (!(stream.str().empty())){
					string com2;
					float angle2;
				 	bool dir2;
				 	long delay2;
				 	stream>>angle2>>delay2>>dir2;
				 	std::thread mot1Thread(&mot1,com,angle,delay,dir);
					std::thread mot2Thread(&mot2,com2,angle2,delay2,dir2);
					mot1Thread.join();
					mot2Thread.join();
				}
				else{
					std::thread mot1Thread(&mot1,com,angle,delay,dir);
					std::thread mot2Thread(&mot2,com,angle,delay,dir);
					mot1Thread.join();
					mot2Thread.join();
				}
			}
		}
	}
}