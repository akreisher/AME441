#include "Kinematics.h"
#include "quaternion.h"
#include "ArmClass.h"
#include "ArmRecorder.h"
#include <iostream>
#include <locale>

int main(int argc, char *argv[]) {
	char def;
	std::cout<<"Use defaults? [y/n]: ";
	std::cin>>def;
	MotorInfo mot[3];
	if (def == 'y'||def=='Y')
	{
		//default settings
	}
	else
	{
		int num1,num2;
		std::cout<<"Upper IMU Serial Number: ";
		std::cin>>
		for (int i = 0;i<3;i++)
		{
			int dir,step;
			float angle;
			valid = false;
			while (!valid){
			valid = true;
			std::cout<<"Motor 1 info (dir,step,angle): ";
			std::string inf;
			cin>>inf;
			std::stringsttream ss(inf);
			inf<<dir<<step<<angle;
			mot[i]=MotorInfo(dir,step,angle);
		}
		con = ArmController(mot);
		ArmRecorder rec;
		ArmController con;


	}
    std::cout << "Program Executing\n";
    signal(SIGINT, handle_sig);
	std::string ser1 = "/dev/ttyACM"+std::to_string(num1);
	std::string ser2 = "/dev/ttyACM"+std::to_string(num2);
    AHRS com1 = AHRS(ser1.c_str());
    AHRS com2 = AHRS(ser2.c_str());
    Quaternion v1(0.0,0.0,1.0,0.0);
    Quaternion v2(0.0,0.0,1.0,0.0);

    st