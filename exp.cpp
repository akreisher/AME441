#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "navX/AHRS.h"
#include <chrono>
#include <thread>
#include <iomanip>
#include <signal.h>
#include <string>
#include "support/IMUQuatReader.h"
#include "motor/motorClass.h"
#include <wiringPi.h>
#include <queue>
#include <cmath>

std::queue<float> qu;
volatile sig_atomic_t sflag = 0;

void handle_sig(int sig)
{
    sflag = 1;
}

void threadFunc()
{
	IMUQuatReader upIMU(0);
	IMUQuatReader lowIMU(1);
	
	Quaternion v1(0.0,0.0,1.0,0.0);
	//Quaternion v2(0.0,0.0,1.0,0.0);

		//initial pos
	Quaternion q1prev = upIMU.getQuat();
		//Quaternion q2prev = lowIMU.getQuat();

	while (true)
	{
		if(sflag){
			upIMU.Close();
			lowIMU.Close();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			break;
		}
		Quaternion q = upIMU.getQuat();
		float alpha = acos(q.getW())*2.0;
		//std::cout<<alpha<<std::endl;
		qu.push(alpha);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
			/*
			Quaternion q1 = upIMU.getQuat();
	        Quaternion q2now = lowIMU.getQuat();
	        Quaternion q2 = q2now/q2prev;
	        q2prev = q2now;
	        q2=q2/q1;
	        q1.normalize();
	        q2.normalize();
	        q1prev = q1;
	        v1 = v1.rotate(q1);
	        v2 = v2.rotate(q2);
	        v2 = v2.rotate(q1); 

	       	queue->push(Coordinates(v2.getX(),v2.getY(),v2.getZ()));
	       		std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			* */
		}
}


int main()
{
	signal(SIGINT, handle_sig);
	std::thread sThread(threadFunc);
	int dirPin,stepPin;
	dirPin=27;
	stepPin=17;
	float angle = 0.9;
	StepperMotor mot = StepperMotor(dirPin,stepPin,angle);
	Quaternion qPrev(1.0,0.0,0.0,0.0);
	float theta = 0;
	while (true)
	{
		if (sflag)
		{
			sThread.join();
			break;
		}
		if (qu.size()>2)
		{
			/*
			Quaternion qNew  = qu.front();
			Quaternion q = q;
			std::cout<<q.getW()<<std::endl;
			qPrev=qNew;
			qu.pop();
			float w = q.getW();
			float alpha = acos(w)*2.0;
			bool dir = 0;
			theta = alpha;
			* */
					bool dir = 0;
			float alpha = qu.front();
			std::cout<<alpha<<std::endl;
			
			if (alpha<0)
			{
				alpha = -alpha;
				dir = 1;
			}
			mot.rotateToPos(theta,5,dir);
			//std::cout<<theta<<std::endl;
			//std::cout << q.getW() << "      " << q.getX()<< "   " << q.getY() << "     " << q.getZ() << "     " << "      " <<std::endl;
		}
	}
	
}
