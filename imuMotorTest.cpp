#include <iostream>
#include <fstream>
#include <stdio.h>
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

volatile sig_atomic_t sflag = 0;
std::mutex sigmtx; //signal mutex
//queue of yaw data

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
	IMUQuatReader com(1);
    std::ofstream imuFile;
    imuFile.open("data/newimudata6.txt");
	printf("Initializing\n\n");

	MilliTimer timer;
	while (true){
    	//check for mtx signal
    	if(sigmtx.try_lock()){
            sigmtx.unlock();//unlock mtx
            imuFile.close();
             com.Close();
            break;
        }
		Quaternion q = com.getQuat();

		float alpha = 2*acos(q.getW());
        imuFile<<timer.now()<<","<<alpha<<std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(period));
	}

}

int main(int argc, char *argv[]) {
    std::cout << "Program Executing\n";
    sigmtx.lock();//lock mtx signal
    signal(SIGINT, handle_sig);//set SIGINT signal handler
    std::ofstream ofile;
    ofile.open("data/newmotordata6.txt");
    StepperMotor stepper(21,20,1.8);//stepper (dir,step,step angle)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::thread IMUthread(readIMUData,1);
	MilliTimer timer;

   for (int i = 0;i<10;i++)
   {
	   int rot = 200;

        for (int j = 0; j<rot;j++)
        {
            stepper.step(5,1);
            ofile<<timer.now()<<","<<stepper.getCurrPos()<<std::endl;
            
        }
   }

    	sigmtx.unlock();
    	IMUthread.join();
        
        ofile.close();
       system("python imuTestPlotter.py");



}






