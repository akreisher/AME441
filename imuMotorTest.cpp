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

//directions
const bool CW = 1;
const bool CCW = 0;

//SIGINT handler
void handle_sig(int sig)
{
    sflag = 1;
}

//IMU thread code, reads in data 
void readIMUData(AHRS* com,int period)
{
    std::ofstream imuFile;
    imuFile.open("imudata.txt");
	printf("Initializing\n\n");
	while (true){
    	//check for mtx signal
    	if(sigmtx.try_lock()){
            sigmtx.unlock();//unlock mtx
            imuFile.close();
            break;
        }

        imuFile<<com->GetLastSensorTimestamp()<<","<<com->GetYaw()<<std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(period));
	}

}

int main(int argc, char *argv[]) {
    std::cout << "Program Executing\n";
    sigmtx.lock();//lock mtx signal
    signal(SIGINT, handle_sig);//set SIGINT signal handler
    std::ofstream ofile;
    ofile.open("motordata.txt");
    StepperMotor stepper(20,21,0.9);//stepper (dir,step,step angle)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    AHRS com = AHRS("/dev/ttyACM0");
    std::thread IMUthread(readIMUData,&com,10);

   for (int i = 0;i<5;i++)
   {
        for (int j = 0; j<400;j++)
        {
            stepper.rotate(0.9,5,CCW);
            ofile<<com.GetLastSensorTimestamp()<<","<<stepper.getCurrPos();
            delay(25);
        }
        for (int j = 0; j<400;j++)
        {
            stepper.rotate(0.9,5,CW);
            ofile<<com.GetLastSensorTimestamp()<<","<<stepper.getCurrPos();
            delay(25);
        }
   }
    	sigmtx.unlock();
        
        ofile.close();
        com.close();

    ofile.close()

}






