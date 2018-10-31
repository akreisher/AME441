#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "navX/AHRS.h"
#include <wiringPi.h>

#define pi 3.14159265359
#define dt 0.02

void compFilter(float gyroData[3],float accData[3], float* pitch, float* roll,float alpha)
{
	//addnew gyro values
	*pitch += (gyroData[0]*dt);
	*roll += (gyroData[1]*dt);

	//check for garbage data
	int magForce = abs(accData[0])+abs(accData[1])+abs(accData[2]);
	
		//if good data, add acceleration data
	float pitchAcc = atan2f(accData[1],accData[2])*180/pi;
	*pitch = (*pitch)*(1-alpha) + alpha*pitchAcc;
	float rollAcc = atan2f(accData[0],accData[2])*180/pi;
	*roll = (*roll)*(1-alpha) + alpha*rollAcc;
	
}


int main()
{
	AHRS com = AHRS("/dev/ttyACM0");//open IMU
	wiringPiSetupSys();
	//open ofiles
	std::ofstream pitchfile;
	std::ofstream rollfile;
	pitchfile.open("pitchData.txt");
	rollfile.open("rollData.txt");
	printf("Initializing\n\n");
	//get initial attitude
	float initPitch = com.GetPitch();
	float initRoll = com.GetRoll();
	float roll[10];
	float pitch[10];
	//set initial angles
	for (int i = 0;i<10;i++){
		roll[i]=initRoll;
		pitch[i]=initPitch;
	}
	//1000 steps
	for (int i = 0;i<1000;i++){
		//get sensor dat
		float accData[3];
		accData[0] = com.GetRawAccelX();
		accData[1] = com.GetRawAccelY();
		accData[2] = com.GetRawAccelZ();

		float gyroData[3];
		gyroData[0]=com.GetRawGyroX();
		gyroData[1]=com.GetRawGyroY();
		gyroData[2]=com.GetRawGyroZ();
		long time = com.GetLastSensorTimestamp();

		//print to files
		pitchfile<<time<<","<<gyroData[0];
		rollfile<<time<<","<<gyroData[1];
		for (int j = 0;j<10;j++)
		{
			//apply filter
			compFilter(gyroData,accData,&pitch[j],&roll[j],0.2+0.2*i);
			pitchfile<<","<<pitch[j];
			rollfile<<","<<roll[j];
		}
		pitchfile<<std::endl;
		rollfile<<std::endl;
		delay(20);
	}

}
