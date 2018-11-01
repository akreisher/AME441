import matplotlib.pyplot as plt
import numpy as np

count = 0
with open("imuContData.txt","r") as f:
	#Get number of data entries
	for line in f:
		count=count+1
t=np.empty([count,1])
pitch=np.empty([count,1])
roll=np.empty([count,1])
yaw=np.empty([count,1])

with open("imuContData.txt","r") as f:
	#Parse data from file
	for i,line in enumerate(f):
		data=line.split(",")
		t[i]=data[0]
		pitch[i]=data[1]
		roll[i]=data[2]
		yaw[i]=data[3]

t1=np.empty([8,1])
p1=np.empty([8,1])
r1=np.empty([8,1])
y1=np.empty([8,1])
with open("imuRec.txt","r") as f:
	for i,line in enumerate(f):
		data=line.split(",")
		t1[i]=data[0]
		p1[i]=data[1]
		r1[i]=data[2]
		y1[i]=data[3]
t=(t)/1000
t1=(t1)/1000
plt.plot(t[1:count-1],roll[1:count-1],'g',label="IMU Roll")
plt.plot(t[1:count-1],pitch[1:count-1],'b',label="IMU Pitch")
plt.plot(t[1:count-1],yaw[1:count-1],'r',label="IMU Yaw")
plt.plot([t[1],t[count-1]],[45,45],"--k")
plt.plot([t[1],t[count-1]],[-45,-45],"--k")
plt.plot([t[1],t[count-1]],[0,0],"--k")
plt.plot([t[1],t[count-1]],[90,90],"--k")
plt.plot([t[1],t[count-1]],[180,180],"--k")
plt.plot([t[1],t[count-1]],[-90,-90],"--k")
plt.scatter(t1,r1,c='g')
plt.scatter(t1,p1,c='b')
plt.scatter(t1,y1,c='r')
plt.xlabel("Time [s]")
plt.ylabel("Angle [deg]")
plt.legend()
plt.box(True)
plt.show()
