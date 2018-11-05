#include "motor/motorSim.h"
#include <fstream>

int main(){
	std::ofstream ofile;
	ofile.open("motTest.txt");
	MotorSim mot(0.9,&ofile);
	mot.rotate(45,5,0);
}



