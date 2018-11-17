#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include "navX/AHRS.h"
#include <chrono>
#include <thread>
#include <iomanip>
#include <signal.h>
#include <wiringPi.h>
#include <queue>
#include <utility>
#include <mutex>
#include <atomic>
#include <cmath>
#include "motor/motorClass.h"
#include "support/IMUQuatReader.h"
#include <fstream>

#define PI 3.14159265

volatile sig_atomic_t sflag = 0;
std::mutex sigmtx; //signal mutex
//queue of yaw data
std::queue<float> yawData;
std::queue<float> timeData;



//directions
const bool CW = 1;
const bool CCW = 0;

//SIGINT handler
void handle_sig(int sig)
{
    sflag = 1;
}

//IMU thread code, reads in data 
void readIMUData(int period)
{
	IMUQuatReader com1(0);
	IMUQuatReader com2(1);
	printf("Initializing\n\n");
	MilliTimer timer;
	
	Quaternion q1prev= com1.getQuat();
	Quaternion q2prev(&com2);
	while( 1 == 1){
		if(sigmtx.try_lock()){
			com.Close();//close AHRS
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			sigmtx.unlock();//unlock mtx
			break;
		}
		Quaternion q1now(&com1);
		Quaternion q1 = q1now/q1prev;
		Quaternion q2now(&com2);
		Quaternion q2 = q2now/q2prev;
		q2prev = q2now;
		q2=q2/q1;
		q1.normalize();
		q2.normalize();
		q1prev = q1now;
		v1 = v1.rotate(q1);
		v2 = v2.rotate(q2);
		v2 = v2.rotate(q1);
		ofile<<v1.getX()<<","<<v1.getY()<<","<<v1.getZ()<<","<<v2.getX()<<","<<v2.getY()<<","<<v2.getZ()<<std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
			
		float alpha = 2*acos(q.getW());
		if (q.getZ()<0) alpha = -alpha;
		
        yawData.push(-180*alpha/PI);
        timeData.push(timer.now());
 //       timeData.push(com.GetLastSensorTimestamp());

        std::this_thread::sleep_for(std::chrono::milliseconds(period));
	}

}

int main(int argc, char *argv[]) {
	std::string file("data/");;
	std::string filename;
	if (argc>1)
	{
		filename= argv[1];
	}
	else
	{
		filename="expdata.txt";
	}
	file=file+filename;
	std::cout<<filename<<std::endl;
	std::ofstream ofile;
	ofile.open(filename);
    std::cout << "Program Executing\n";
    sigmtx.lock();//lock mtx signal
    signal(SIGINT, handle_sig);//set SIGINT signal handler
    StepperMotor stepper(27,17,0.9);//stepper (dir,step,step angle)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	stepper.setLimits(-90,90);
    std::thread IMUthread(readIMUData,10);
	MilliTimer timer;
    while(true){
    	//get SIGINT signal
    	if(sflag){
            sflag = 0;
            sigmtx.unlock();
            IMUthread.join();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
		if (yawData.size()>2){
			//get yaw data
			float prevYaw = yawData.front();
			//float deltaTheta = yawData.front();
			yawData.pop();
			timeData.pop();
			float newYaw = yawData.front();
			//long prevTime = timeData.front();
			//timeData.pop();
			//long newTime = timeData.front();

			//calculate angular disp. and speed
			float deltaTheta = newYaw-prevYaw;
			//long deltat=(newTime-prevTime)/1000;

			if (deltaTheta>0) stepper.rotateToPos(newYaw,3,CCW);
			else stepper.rotateToPos(newYaw,3,CW);
			ofile<<timeData.front()<<","<<newYaw<<std::endl;
            float motorPos = stepper.getCurrPos();
            std::cout <<"Yaw: " << newYaw<< " Motor pos: " << motorPos<<std::endl;
            
		}
    }
    printf("\nExit Caught... Closing device.\n");


}






