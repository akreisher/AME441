#include "quaternion.h"
#include <cmath>

#define PI 3.14159265

struct Coordinates
{
	Coordinates(float t1s, float t2s, float t3s)
	{
		x=t1s;
		y = t2s;
		z = t3s;
	}
	float x,y,z;
};

struct Angles
{
	Angles(float t1s, float t2s, float t3s)
	{
		t1=t1s;
		t2 = t2s;
		t3 = t3s;
	}
	float t1,t2,t3;
};

class Kinematics
{
	public:
		static Angles inverse(Coordinates u,float l1, float l2)
		{
			float x,y,z;
			x = u.x;
			y = u.y;
			z = u.z;
			float t1,t2,t3;
			t1=atan2(-z,y);
			float yp=y*cos(t1)-z*sin(t1);
			t3=acos(((x*x)+(yp*yp)-(l1*l1)-(l2*l2))/(2*l1*l2));
			t2=atan2(yp,x)-atan2(l2*sin(t3),l1+l2*cos(t3));
			return Angles(t1,t2,t3);
		}
};
