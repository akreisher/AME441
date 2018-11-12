#include "quaternion.h"
#include <cmath>

#define PI 3.14159265

struct Coordinates
{
	float x,y,z;
};

class Kinematics
{
	public:
		float* static inverse(Coordinates u,float l1, float l2)
		{
			float x,y,z;
			x = u.x;
			y = u.y;
			z = u.z;
			float t[3];
			t[0]=atan2(-z,y);
			float yp=y*cos(t[0])-z*sin(t[0]);
			t[2]=acos(((x*x)+(yp*yp)-(l1*l1)-(l2*l2))/(2*l1*l2));
			t[1]=atan2(yp,x)-atan2(l2*sin(t[2]),l1+l2*cos(t[2]));
			return t;
		}
}