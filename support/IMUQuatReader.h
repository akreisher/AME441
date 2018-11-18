#include "../navX/AHRS.h"
#include "Kinematics.h"
#include "MilliTimer.h"
#include <cmath>
#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>
#define PI 3.14159265



class IMUQuatReader	
{
public:
	IMUQuatReader(int num) : com(("/dev/ttyACM"+std::to_string(num)).c_str(),AHRS::SerialDataType::kRawData,60)
	{
	
		rot = Quaternion(1.0,0.0,0.0,0.0);
		tPrev = 0;
		timer = MilliTimer();
		running = true;
		std::cout<<"Starting IMU"<<std::endl;
		th= std::thread(gyroUpdate,this,1);
	}
	
	static void gyroUpdate(IMUQuatReader* imu,int period)
	{

		while (imu->isRunning()){
			imu->update();
			std::this_thread::sleep_for(std::chrono::milliseconds(period));
		}
	}
	
	void reset()
	{
		rot = Quaternion(1.0,0.0,0.0,0.0);
		tPrev = 0;
		timer.reset();
	}
	
	void update()
	{
		updateMtx.lock();
		float tNew = timer.now();
		float omega_x = PI*com.GetRawGyroX()/180;
		float omega_y = PI*com.GetRawGyroY()/180;
		float omega_z = PI*com.GetRawGyroZ()/180;
		float dt = tNew-tPrev;
		tPrev = tNew;
		Quaternion dq = (rot*0.5)*Quaternion(0,omega_x,omega_y,omega_z);
		rot = rot + dq*(dt/1000.0);
		rot.normalize();
		updateMtx.unlock();
	}
	
	Quaternion getQuat()
	{
		updateMtx.lock();
		Quaternion out = rot;
		updateMtx.unlock();
		return out;
	}
	
	float getRotAng()
	{
		return 2*acos(rot.getW());
	}
	
	void Close()
	{
		running = false;
		th.join();
		com.Close();
	}
	
	bool isRunning()
	{
		return running;
	}
	
private:
	AHRS com;
	Quaternion rot= Quaternion(1.0,0.0,0.0,0.0);
	MilliTimer timer;
	float tPrev;
	bool running;
	std::thread th;
	std::mutex updateMtx;
};
