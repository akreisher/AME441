#include <iostream>
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
//queue of yaw data, race-condition safe

std::queue<std::pair<float,long>> yawData;

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
	AHRS com = AHRS("/dev/ttyACM0");
	printf("Initializing\n\n");
	while (true){
    	//check for mtx signal
    	if(sigmtx.try_lock()){
            com.Close();//close AHRS
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            sigmtx.unlock();//unlock mtx
            break;
        }

        yawData.push(std::pair<float,long>(com.GetYaw(),com.GetLastSensorTimestamp()));

        std::this_thread::sleep_for(std::chrono::milliseconds(period));
	}

}

int main(int argc, char *argv[]) {
    std::cout << "Program Executing\n";
    sigmtx.lock();//lock mtx signal
    signal(SIGINT, handle_sig);//set SIGINT signal handler

    StepperMotor stepper(20,21,1.8);//stepper (dir,step,step angle)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::thread IMUthread(readIMUData,100);

    while(true){
    	//get SIGIN signal
    	if(sflag){
            sflag = 0;
            sigmtx.unlock();
            IMUthread.join();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }

        //get yaw data
        std::pair<float,long> yawPair1 = yawData.front();
        yawData.pop();
        std::pair<float,long> yawPair2 = yawData.front();
        yawData.pop();

        //claculate angular disp. and speed
        float deltaTheta = yawPair2.first-yawPair1.first;
        long deltat=(yawPair2.second-yawPair1.second)/1000;

        //into rad/s
        //check if this is right (imu in s)
        float speed = deltaTheta/deltat;
        if (deltaTheta>0) stepper.rotate(deltaTheta,speed,CCW);
        else stepper.rotate(-deltaTheta,-speed,CW);
    }
    printf("\nExit Caught... Closing device.\n");
}





