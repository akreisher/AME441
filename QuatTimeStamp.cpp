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
		num = 1;
	}
	
    std::cout << "Program Executing\n";
    signal(SIGINT, handle_sig);
    /*
	std::string ser = "/dev/ttyACM"+std::to_string(num);
	std::cout<<ser<<std::endl;
    AHRS com = AHRS(ser.c_str());
    * */
	IMUQuatReader read(0);
    printf("Initializing\n\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));


    std::cout << "QuatW  | QuatX | QuatY | QuatZ | Time |" << std::endl;
    while( 1 == 1){
		Quaternion q =  read.getQuat();
		//q=q/qRef;
        std::cout << std::fixed << std::setprecision(2) << q.getW() << "      " << q.getX()<< "   " << q.getY() << "     " << q.getZ() << "     " << "      " << '\r' << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(125));
        if(sflag){
            sflag = 0;
            read.Close();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
    
    }
    printf("\nExit Caught... Closing device.\n");

    return 0;
}
