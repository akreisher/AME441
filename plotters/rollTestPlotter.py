import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np
import math
import sys


#Open data file
count=int(sys.argv[1])
data=sys.argv[2]


#Set up empty arrays
t=np.empty([count,1])
imu=np.empty([count,1])
mot=np.empty([count,1])

with open(data,"r") as f:
	#Parse data from file
	for i,line in enumerate(f):
		data=line.split(",")
		t[i]=data[0]
		mot[i]=data[1]
		imu[i]=data[2]

mot=-mot;
for i in range(0,count):
	
	
	
	
	num=mot[i]//360
	if (num>0 and num%2==0):
		mot[i] = mot[i]-num*360
	if (num>0 and num%2==1):
		mot[i] = (num+1)*360-mot[i]

print(mot[-1])
print(imu[-1])

print("e= "+str(math.fabs(mot[-1]-imu[-1])))


#Plot data
t=(t-t[0])/1000


plt.plot(t,imu,'b',label="IMU Roll")
plt.plot(t,mot,'r',label="Motor Position")
#plt.legend("IMU Yaw", "Motor Position")
plt.ylim(0,360)
print(t[-1])
plt.xlabel("Time [s]")
plt.ylabel("Angle [deg]")
plt.legend()
plt.box(True)
plt.show()


