#include "navx/AHRS.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <utility>
#include <mutex>
#include <atomic>
#include <iomanip>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

volatile sig_atomic_t sflag = 0;
std::mutex sigmtx;

void handle_sig(int sig)
{
    sflag = 1;
}

void recIMUCont(AHRS* com, int period)
{
	std::ofstream imuContData;
	imuContData.open("imuContData.txt");
	while (true){
		if(sigmtx.try_lock()){
            imuContData.close();
            sigmtx.unlock();//unlock mtx
            break;
        }
        imuContData<<com->GetLastSensorTimestamp()<<",";
        imuContData<<com->GetPitch()<<",";
        imuContData<<com->GetRoll()<<",";
        imuContData<<com->GetYaw()<<","<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(period));
	}
	return;
}

int main()
{
	sigmtx.lock();
	signal(SIGINT, handle_sig);
	AHRS com = AHRS("/dev/ttyACM0");
	std::ofstream ofile;
	ofile.open("imuRec.txt");
	std::thread imuRecThread(recIMUCont,&com,10);
	while (true){
		if(sflag){
            sflag = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
        char command;
        std::cin>>command;

        //l records data, q quits
        if (command=="l"){
        	ofile<<com->GetLastSensorTimestamp()<<",";
        	ofile<<com->GetPitch()<<",";
        	ofile<<com->GetRoll()<<",";
        	ofile<<com->GetYaw()<<","<<std::endl;
        }
        else if (command == "q"){
        	break;
        }
	}
	sigmtx.unlock();
	imuRecThread.join();
	com.Close();
	ofile.close();
}