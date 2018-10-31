#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <AHRS.h>
#include <chrono>
#include <thread>
#include <iomanip>
#include <signal.h>
#include <wiringPi.h>

volatile sig_atomic_t sflag = 0;

void handle_sig(int sig)
{
    sflag = 1;
}

int main(int argc, char *argv[]) {
    std::cout << "Program Executing\n";
    signal(SIGINT, handle_sig);

    //works for SPI, change constructor for I2C
    AHRS com = AHRS("/dev/ttyACM0");

    printf("Initializing\n\n");
    
    wiringPiSetupGpio();
    const int dirPin=;
    const int stepPin=;
    const bool CW = 1;
    const bool CCW = 0;

    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //TimeSTamp code
    /*
    std::cout << "Pitch  |  Roll  |  Yaw  |  X-Accel  | Y-Accel  |  Z-Accel  |  Time  |" << std::endl;

    while( 1 == 1){
        std::cout << std::fixed << std::setprecision(2) << com.GetPitch() << "      " << com.GetRoll() << "   " << com.GetYaw() << "     " <<com.GetWorldLinearAccelX() << "     " << com.GetWorldLinearAccelY() << "       " << com.GetWorldLinearAccelZ() << "      " << com.GetLastSensorTimestamp() << "      " << '\r' << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(125));
        if(sflag){
            sflag = 0;
            com.Close();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }
    
    }
    */

    while (true){
    	//check for close signal
    	if(sflag){
            sflag = 0;
            com.Close();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }

        //get IMU pitch
        float pitch = com.GetPitch();
        if (pitch>90) pitch = 90;
        if (pitch<-90) pitch = -90;


        digitalWrite(dirPin,HIGH); //set dir CW
        float m = (4000-300)/(-90-90);
        float b = 300-m*90;
		float delay= m*(pitch)+b; //get new delay

		//send pulse
		digitalWrite(stepPin,HIGH);
		delay((int)delay);
		digitalWrite(stepPin,LOW);
		delay((int)delay);
		
    }
    printf("\nExit Caught... Closing device.\n");

    return 0;
}
