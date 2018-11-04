#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "navX/AHRS.h"
#include <chrono>
#include <thread>
#include <iomanip>
#include <signal.h>
#include <string>
#include <fstream>
#include "quaternion.h"

volatile sig_atomic_t sflag = 0;

void handle_sig(int sig)
{
    sflag = 1;
}


int main(int argc, char *argv[]) {
	int num;
	if (argc >1){
		num = atoi(argv[1]);
	}
	else{
		num = 0;
	}
    std::cout << "Program Executing\n";
    signal(SIGINT, handle_sig);
	std::string ser = "/dev/ttyACM"+std::to_string(num);
	std::cout<<ser<<std::endl;
    AHRS com = AHRS(ser.c_str());
    Quaternion v(0.0,0.0,1.0,0.0);
    std::ofstream ofile;
    ofile.open("data/qfile.txt");

    printf("Initializing\n\n");
	

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));


    std::cout << "QuatX | QuatY | QuatZ | Time |" << std::endl;
	int count = 0;
	Quaternion qprev(com.GetQuaternionW(),com.GetQuaternionX(),com.GetQuaternionY(),com.GetQuaternionZ());
    while( 1 == 1){
		Quaternion qnow(com.GetQuaternionW(),com.GetQuaternionX(),com.GetQuaternionY(),com.GetQuaternionZ());
        Quaternion q = qnow/qprev;
        q.normalize();
        qprev = qnow;
        v = (q*v)*q.conjugate();
        ofile<<v.getX()<<","<<v.getY()<<","<<v.getZ()<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        count ++;
        if (count ==1){
			count = 0;
			std::cout << std::fixed << std::setprecision(2) << v.getX()<< "   " << v.getY() << "     " << v.getZ() << "     " << com.GetLastSensorTimestamp() << "      " << '\r' << std::flush;
		}
			
			
        if(sflag){
            sflag = 0;
            com.Close();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
    
    }
    printf("\nExit Caught... Closing device.\n");

    return 0;
}
