#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "AHRS.h"
#include <wiringPi.h>

#define pi 3.14159265359
#define dt 0.01

void compFilter(float gyroData[3],float accData[3], float* pitch, float* roll,float alpha)
{
	//addnew gyro values
	*pitch += (gyroData[0]*dt);
	*roll += (gyroData[1]*dt);

	//check for garbage data
	int magForce = abs(accData[0])+abs(accData[1])+abs(accData[2]);
	if (magForce>1&&magForce<2){
		//if good data, add acceleration data
		pitchAcc = atan2f(accData[1],accData[2])*180/pi;
		*pitch = (*pitch)*(1-alpha) + alpha*pitchAcc;
		rollAcc = atan2f(accData[0],accData[2])*180/pi;
		*roll = (*roll)*(1-alpha) + alpha*rollAcc;
	}
}


int main()
{
	AHRS com = AHRS("/dev/ttyACM0");//open IMU
	wiringPiSetupSys();
	//open ofiles
	std::ofstream pitchfile;
	std::ofstream rollfile;
	ofile.open("pitchData.txt");
	ofile.open("rollData.txt");
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
		accData[0] = com.GetWorldLinearAccelX();
		accData[1] = com.GetWorldLinearAccelY();
		accData[2] = com.GetWorldLinearAccelZ();

		float gyroData[3];
		gyroData[0]=com.GetPitch();
		gyroData[1]=com.GetRoll();
		gyroData[2]=com.GetYaw();
		long time = com.GetLastSensorTimestamp();

		//print to files
		pitchFile<<time<<","<<gyroData[0];
		rollFile<<time<<","<<gyroData[1];
		for (int j = 0;j<10;j++)
		{
			//apply filter
			compFilter(gyroData,accData,&pitch[i],&roll[i],0.2+0.2*i);
			pitchFile<<","<<pitch[i];
			rollFile<<roll[i];
		}
		pitchFile<<std::endl;
		rollFile<<std::endl;
		delay(10);
	}

}