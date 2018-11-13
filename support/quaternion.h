#include <cmath>
#include <iostream>
#include "../navX/AHRS.h"

class Quaternion{
public:
	Quaternion(float w, float x, float y, float z){
		this->w=w;
		this->x=x;
		this->y=y;
		this->z=z;
		this->mag = this->calcMag();
	}
	Quaternion(AHRS* com){
		this->w=com->GetQuaternionW();

			this->x=com->GetQuaternionX();
			this->y=com->GetQuaternionY();
		
		this->z=com->GetQuaternionZ();
		this->mag = this->calcMag();
	}
	
	Quaternion operator+(Quaternion b)
	{
		return Quaternion(w+b.w,x+b.x,y+b.y,z+b.z);
	}
	
	Quaternion operator*(Quaternion b){
		float cw = this->w*b.w - this->x*b.x - this->y*b.y - this->z*b.z;
		float cx = this->w*b.x + this->x*b.w + this->y*b.z - this->z*b.y;
		float cy = this->w*b.y - this->x*b.z + this->y*b.w + this->z*b.x;
		float cz = this->w*b.z + this->x*b.y - this->y*b.x + this->z*b.w;
		return Quaternion(cw,cx,cy,cz);
	}
	Quaternion operator*(float b){		
		return Quaternion(b*w,b*x,b*y,b*z);
	}
	Quaternion operator/(Quaternion b){
		return (*this)*(b.conjugate());
	}
	Quaternion conjugate(){
		return Quaternion(w,-x,-y,-z);
	}
	Quaternion rotate(Quaternion b){
		return b*(*this)/b;
	}
	void normalize(){
		w=w/mag;
		x=x/mag;
		y=y/mag;
		z=z/mag;
		mag = calcMag();
	}
	float getMag(){
		return mag;
	}
	
	
	const float getW(){return w;}
	const float getX(){return x;}
	const float getY(){return y;}
	const float getZ(){return z;}
	
	void setW(float w){
		this->w = w;
		mag = calcMag();
	}
	void setX(float x){
		this->x = x;
		mag = calcMag();
	}
	void setY(float y){
		this->y = y;
		mag = calcMag();
	}
	void setZ(float z){
		this->z = z;
		mag = calcMag();
	}
	
private:
	float w,x,y,z,mag;
	float calcMag(){
		return sqrt(pow(w,2)+pow(x,2)+pow(y,2)+pow(z,2));
	}
	
};

std::ostream& operator<<(std::ostream& os,  Quaternion q){
	float w,x,y,z;
	w = q.getW();
	x = q.getX();
	y = q.getY();
	z = q.getZ();
	os<<w;
	if (x>=0) os<<"+";
	os<<x<<"i";
	if (y>=0) os<<"+";
	os<<y<<"j";
	if (z>=0) os<<"+";
	os<<z<<"k";
	return os;
}
