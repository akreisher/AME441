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
	int num1, num2;
	if (argc >1){
		num1 = atoi(argv[1]);
		num2 = atoi(argv[2]);
	}
	else{
		num1 = 0;
		num2 = 1;
	}
    std::cout << "Program Executing\n";
    signal(SIGINT, handle_sig);
	std::string ser1 = "/dev/ttyACM"+std::to_string(num1);
	std::string ser2 = "/dev/ttyACM"+std::to_string(num2);
    AHRS com1 = AHRS(ser1.c_str());
    AHRS com2 = AHRS(ser2.c_str());
    Quaternion v1(0.0,0.0,1.0,0.0);
    Quaternion v2(0.0,0.0,1.0,0.0);
    std::ofstream ofile;
    ofile.open("data/dqfile.txt");

    printf("Initializing\n\n");
	

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));


    std::cout << "QuatX | QuatY | QuatZ | Time |" << std::endl;

	Quaternion q1prev(&com1);
	Quaternion q2prev(&com2);
    while( 1 == 1){
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
			
			
        if(sflag){
            sflag = 0;
            com1.Close();
            com2.Close();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
    
    }
    printf("\nExit Caught... Closing device.\n");
    system("python animD3D.py");

    return 0;
}
