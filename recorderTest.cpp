#include "ArmRecorder.h"



int main()
{
	ArmRecorder rec(0,1);
	std::queue<Coordinates> q;
	q.push(Coordinates(0.0,1.0,0.0));
	Coordinates r = q.front();
	std::cout<<r.x<<" "<<r.y<<" "<<r.z<<std::endl;
}
