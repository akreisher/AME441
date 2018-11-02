
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <utility>
#include <mutex>
#include <atomic>
#include <iomanip>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "navX/AHRS.h"
#include "MilliTimer.h"

volatile sig_atomic_t sflag = 0;
std::mutex sigmtx;



void handle_sig(int sig)
{
    sflag = 1;
}

void recIMUCont(AHRS* com, int period,float p, float r, float y)
{
	std::ofstream imuContData;
	imuContData.open("imuContData.txt");
	
	MilliTimer timer;
	while (true){
		if(sigmtx.try_lock()){
            imuContData.close();
            sigmtx.unlock();//unlock mtx
            break;
        }

        imuContData<<timer.now()<<",";
        imuContData<<com->GetPitch()-p<<",";
        imuContData<<com->GetRoll()-r<<",";
        imuContData<<com->GetYaw()-y<<","<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(period));
	}
	return;
}

int main()
{
	sigmtx.lock();
	signal(SIGINT, handle_sig);
	AHRS com = AHRS("/dev/ttyACM1");
	std::ofstream ofile;
	ofile.open("imuRec.txt");
	float zeroPitch = 0;
float zeroRoll = 0;
float zeroYaw = 0;
std::thread imuRecThread;
	MilliTimer timer;
	while (true){
        
                
        char command;
        std::cin>>command;

        //l records data, q quits

        if (command=='l'){
			if (zeroPitch==0){
				zeroPitch = com.GetPitch();
				zeroRoll = com.GetRoll();
				zeroYaw = com.GetYaw();
				imuRecThread=std::thread(recIMUCont,&com,10,zeroPitch,zeroRoll,zeroYaw);
				timer.reset();
				
			}
        	ofile<<timer.now()<<",";
        	ofile<<com.GetPitch()-zeroPitch<<",";
        	ofile<<com.GetRoll()-zeroRoll<<",";
        	ofile<<com.GetYaw()-zeroYaw<<","<<std::endl;
        }
        else if (command == 'q'){
        	break;
        }
        
	}
	sigmtx.unlock();
	imuRecThread.join();
	com.Close();
	ofile.close();
}
