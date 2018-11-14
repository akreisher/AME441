#include "../navX/AHRS.h"
#include "Kinematics.h"
#include "MilliTimer.h"
#define PI 3.14159265

class IMUQuatReader	
{
public:
	IMUQuatReader(int num) : com(("/dev/ttyACM"+std::to_string(num)).c_str(),AHRS::SerialDataType::kRawData,60)
	{
	
		rot = Quaternion(1.0,0.0,0.0,0.0);
		tPrev = 0;
		timer = MilliTimer();
	}
	
	void reset()
	{
		rot = Quaternion(1.0,0.0,0.0,0.0);
		tPrev = 0;
		timer.reset();
	}
	
	Quaternion getQuat()
	{
		float tNew = timer.now();
		float omega_x = PI*com.GetRawGyroX()/180;
		float omega_y = PI*com.GetRawGyroY()/180;
		float omega_z = PI*com.GetRawGyroZ()/180;
		float dt = tNew-tPrev;
		tPrev = tNew;
		Quaternion dq = (rot*0.5)*Quaternion(0,omega_x,omega_y,omega_z);
		rot = rot + dq*(dt/1000.0);
		rot.normalize();
		return rot;
		
	}
	
	void Close()
	{
		com.Close();
	}
	
private:
	AHRS com;
	Quaternion rot= Quaternion(1.0,0.0,0.0,0.0);
	MilliTimer timer;
	float tPrev;
};
