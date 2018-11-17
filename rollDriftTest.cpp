#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <cstring>
#include "support/IMUQuatReader.h"
#include "motor/motorClass.h"

int main(int argc, char *argv[]) {
	if (argc <3)
	{
		std::cout<<"Specify file number and number of rotations!"<<std::endl;
		return -1;
	}
	int filenum = atoi(argv[1]);
	int numRot = atoi(argv[2]);
	
	std::cout<<filenum<<" "<<numRot<<std::endl;
	
    std::cout << "Program Executing\n";
    std::ofstream ofile;
    std::string str = "data/roll/roll";
    str+=std::to_string(numRot);
    str+="Rdata";
    str+=std::to_string(filenum);
    str+=".txt";
    ofile.open(str.c_str());
    StepperMotor stepper(21,20,1.8);//stepper (dir,step,step angle)
    IMUQuatReader IMU(3);   
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	MilliTimer timer;
	
	int n = 0;

   for (int i = 0;i<numRot;i++)
   {
	   std::cout<<i<<std::endl;
        for (int j = 0; j<200;j++)
        {
            stepper.step(2,1);
            float alpha = 180*2*acos(IMU.getQuat().getW())/PI;
            ofile<<timer.now()<<","<<stepper.getCurrPos()<<","<<alpha<<std::endl;
            n++;
        }
   }
   
   for (int i = 0;i<numRot;i++)
   {
        for (int j = 0; j<200;j++)
        {
            stepper.step(2,0);

        }
   }

        ofile.close();
        IMU.Close();
        std::string arg = "python plotters/rollTestPlotter.py ";
        arg+=std::to_string(n);
        arg+=" "+str;
       system(arg.c_str());
   }
