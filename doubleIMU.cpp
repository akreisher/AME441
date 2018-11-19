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
#include "support/IMUQuatReader.h"

volatile sig_atomic_t sflag = 0;

void handle_sig(int sig)
{
    sflag = 1;
}


int main(int argc, char *argv[]) {
	int num1, num2;
	if (argc >2){
		num1 = atoi(argv[2]);
		num2 = atoi(argv[3]);
	}
	else{
		num1 = 2;
		num2 = 3;
	}
	
	if (argc <2)
	{
		std::cout<<"Specify file number and number of rotations!"<<std::endl;
		return -1;
	}
	int filenum = atoi(argv[1]);
	
	std::cout<<filenum<<std::endl;
	
    std::cout << "Program Executing\n";
    std::ofstream ofile;
    std::string str = "data/double/double";
    str+=std::to_string(filenum);
    str+=".txt";
    ofile.open(str.c_str());
    std::cout << "Program Executing\n";
    signal(SIGINT, handle_sig);
	IMUQuatReader com1(num1);
	IMUQuatReader com2(num2);
    Quaternion v1(0.0,0.0,1.0,0.0);
    Quaternion v2(0.0,0.0,1.0,0.0);


    printf("Initializing\n\n");
	

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));


    std::cout << "QuatX | QuatY | QuatZ | Time |" << std::endl;
	
	Quaternion q1prev = com1.getQuat();
	Quaternion q2prev = com2.getQuat();
	MilliTimer timer;
    while( 1 == 1){
		Quaternion q1now = com1.getQuat();
        Quaternion q1 = q1now/q1prev;
        Quaternion q2now = com2.getQuat();
        Quaternion q2 = q2now/q2prev;
        q2prev = q2now;
        q2=q2/q1;
        q1.normalize();
        q2.normalize();
        q1prev = q1now;
        v1 = v1.rotate(q1);
        v2 = v2.rotate(q2);
        v2 = v2.rotate(q1);
        std::cout<<v2.getX()<<","<<v2.getY()<<","<<v2.getZ()<<std::endl;
        ofile<<v1.getX()<<","<<v1.getY()<<","<<v1.getZ()<<","<<v2.getX()<<","<<v2.getY()<<","<<v2.getZ()<<","<<timer.now()<<std::endl;
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
    std::string command = "python animD3D.py ";
    command = command+str;
	system(command.c_str());
    return 0;
}
