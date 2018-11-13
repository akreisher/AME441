#include <string>
#include <thread>
class ArmRecorder
{
public:
	struct AngularVelocity
	{
		float omega_x, omega_y, omega_z;
	}


	ArmRecorder(int num1, int num2)
	{
		std::string ser1 = "/dev/ttyACM"+std::to_string(num1);
		std::string ser2 = "/dev/ttyACM"+std::to_string(num2);
		//Raw data constructors
		upIMU=AHRS(ser1,AHRS::SerialDataType::kRawData,60);
		lowIMU=AHRS(ser2,AHRS::SerialDataType::kRawData,60);
	}

	void Run(RaceQueue<Coordinates>& q)
	{
		//set running
		running = true;
		//start data reading thread
		runThread = std::thread(ReadData, q);
		//wait
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	void ReadData(RaceQueue<Coordinates>& q)
	{
		//Vectors representing linkages
		Quaternion l1(0.0,0.0,1.0,0.0);
		Quaternion l2(0.0,0.0,1.0,0.0);

		//initial pos
		Quaternion q1prev(1.0,0.0,0.0,0.0);
		Quaternion q2prev(1.0,0.0,0.0,0.0);

		while (running)
		{
			Quaternion q1now(&upIMU);
	        Quaternion q1 = q1now/q1prev;
	        Quaternion q2now(&lowIMU);
	        Quaternion q2 = q2now/q2prev;
	        q2prev = q2now;
	        q2=q2/q1;
	        q1.normalize();
	        q2.normalize();
	        q1prev = q1now;
	        v1 = v1.rotate(q1);
	        v2 = v2.rotate(q2);
	        v2 = v2.rotate(q1); 

	       	q.push(Coordinates(v2.getX(),v2.getY(),v2.getZ()));
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
	bool running;
	AHRS lowIMU;
	AHRS upIMU;
	std::thread runThread;
}
