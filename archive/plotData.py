import matplotlib.pyplot as plt
import numpy as np
#Open data file
count = 0
with open("testdata.txt","r") as f:
	#Get number of data entries
	for line in f:
		count=count+1

#Set up empty arrays
t=np.empty([count,1])
yaw=np.empty([count,1])
pos=np.empty([count,1])
with open("testdata.txt","r") as f:
	#Parse data from file
	for i,line in enumerate(f):
		data=line.split(",")
		t[i]=data[0]
		yaw[i]=data[1]
		pos[i]=data[2]

#Plot data
t=(t-t[0])/1000
plt.plot(t,yaw,'b',label="IMU Yaw")
plt.plot(t,pos,'r',label="Motor Position")
#plt.legend("IMU Yaw", "Motor Position")
plt.xlabel("Time [s]")
plt.ylabel("Angle [deg]")
plt.legend()
plt.box(False)
plt.show()

