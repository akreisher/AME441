import matplotlib.pyplot as plt
import numpy as np
#Open data file
count = 0
with open("imudata.txt","r") as f:
	#Get number of data entries
	for line in f:
		count=count+1

#Set up empty arrays
t=np.empty([count,1])
roll=np.empty([count,1])

with open("imudata.txt","r") as f:
	#Parse data from file
	for i,line in enumerate(f):
		data=line.split(",")
		t[i]=data[0]
		roll[i]=data[1]
		
roll = roll-roll[1]

count1 = 0
with open("motordata.txt","r") as f:
	#Get number of data entries
	for line in f:
		count1=count1+1
t1=np.empty([count1,1])
pos=np.empty([count1,1])
with open("motordata.txt","r") as f:
	#Parse data from file
	for i,line in enumerate(f):
		data=line.split(",")
		t1[i]=data[0]
		pos[i]=data[1]
for i in range(0,count1):
	if (180<pos[i]<=360):
		pos[i]=360-pos[i]
	if (360<pos[i]<=540):
		pos[i]=pos[i]-360
	if (540<pos[i]<=720):
		pos[i]=720-pos[i]




#Plot data
t=(t-t[20])/1000
t1=(t1-t1[0])/1000
plt.plot(t[20:count-1],roll[20:count-1],'b',label="IMU Roll")
plt.plot(t1,pos,'r',label="Motor Position")
#plt.legend("IMU Yaw", "Motor Position")
plt.xlabel("Time [s]")
plt.ylabel("Angle [deg]")
plt.legend()
plt.box(True)
plt.show()

