#include <string>
#include <thread>
#include "support/IMUQuatReader.h"
#include <queue>


struct AngularVelocity
	{
		float omega_x, omega_y, omega_z;
	};
	
class ArmRecorder
{
public:
	


	ArmRecorder(int num1, int num2)
	{
	}

	void Run(std::queue<Coordinates>& q)
	{
		//set running
		running = true;
		//start data reading thread
		runThread = std::thread(ReadData, q);
		//wait
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	

	static void ReadData(std::queue<Coordinates>& queue)
	{
		//Vectors representing linkages
		Quaternion v1(0.0,0.0,1.0,0.0);
		Quaternion v2(0.0,0.0,1.0,0.0);

		//initial pos
		Quaternion q1prev = upIMU.getQuat();
		Quaternion q2prev = lowIMU.getQuat();

		while (running)
		{
			Quaternion q1 = upIMU.getQuat();
	        Quaternion q2now = lowIMU.getQuat();
	        Quaternion q2 = q2now/q2prev;
	        q2prev = q2now;
	        q2=q2/q1;
	        q1.normalize();
	        q2.normalize();
	        q1prev = q1;
	        v1 = v1.rotate(q1);
	        v2 = v2.rotate(q2);
	        v2 = v2.rotate(q1); 

	       	queue.push(Coordinates(v2.getX(),v2.getY(),v2.getZ()));
	       	std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

	}

	void Close()
	{
		if (running){
			running = false;
			runThread.join();
		}
		upIMU.Close();
		lowIMU.Close();
	}
private:
	static bool running;
	static IMUQuatReader lowIMU((int)0);
	static IMUQuatReader upIMU(1);
	static std::thread runThread;
};
