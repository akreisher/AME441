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
#include <cmath>

#define PI 3.14159265

int main(){
	Quaternion q((float)cos(PI/4),(float)sin(PI/4),0.0,0.0);
	

	Quaternion qstar = q.conjugate();
	
	std::cout<<q*qstar<<std::endl;
	Quaternion v(0.0,0.0,1.0,0.0);

	v = (q*v)*qstar;
	q=Quaternion(1,0,0.0,0.0);
	v = (q*v)*q.conjugate();
	std::cout<<"v is "<<v<<std::endl;
	
}

 
