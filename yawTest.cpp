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
#include "motor/motorClass.h"

volatile sig_atomic_t sflag = 0;
std::mutex sigmtx; //signal mutex
//queue of yaw data
std::queue<float> yawData;
std::queue<long> timeData;
float initYaw = -1000;


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
	AHRS com = AHRS("/dev/ttyACM3");
	printf("Initializing\n\n");
	while (true){
    	//check for mtx signal
    	if(sigmtx.try_lock()){
            com.Close();//close AHRS
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            sigmtx.unlock();//unlock mtx
            break;
        }
        float yaw = com.GetYaw();
		if (initYaw==-1000)
		{
			initYaw=yaw;
			std::cout<<"Initaw "<<initYaw<<std::endl;
		}
        yawData.push(yaw-initYaw);
        timeData.push(com.GetLastSensorTimestamp());

        std::this_thread::sleep_for(std::chrono::milliseconds(period));
	}

}

int main(int argc, char *argv[]) {
    std::cout << "Program Executing\n";
    sigmtx.lock();//lock mtx signal
    signal(SIGINT, handle_sig);//set SIGINT signal handler
    std::ofstream ofile;
    ofile.open("testdata1.txt");
    StepperMotor stepper(20,21,0.9);//stepper (dir,step,step angle)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::thread IMUthread(readIMUData,100);

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
			yawData.pop();
			float newYaw = yawData.front();
			long prevTime = timeData.front();
			timeData.pop();
			long newTime = timeData.front();

			//calculate angular disp. and speed
			float deltaTheta = newYaw-prevYaw;
			long deltat=(newTime-prevTime)/1000;

			if (deltaTheta>0) stepper.rotateToPos(newYaw,5,CCW);
			else stepper.rotateToPos(newYaw,5,CW);
            float motorPos = stepper.getCurrPos();
            std::cout <<"Yaw: " << newYaw << "Motor pos: " << motorPos<<std::endl;
            ofile<<newTime<<","<<newYaw<<","<<motorPos<<std::endl;

		}
    }
    ofile.close();
    system("python plotData.py");
    printf("\nExit Caught... Closing device.\n");


}






