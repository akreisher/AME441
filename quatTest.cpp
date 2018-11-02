#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "navX/AHRS.h"
#include <chrono>
#include <thread>
#include <iomanip>
#include <signal.h>
#include <string>
#include "quaternion.h"

int main(){
	Quaternion q1(1.0,-2.0,3.0,4.0);
	Quaternion q2(7.0,-3.0,-6.0,8.0);
	std::cout<<q1.getMag()<<std::endl;
	q1.normalize();
	std::cout<<q1.getMag()<<std::endl;
}

 
