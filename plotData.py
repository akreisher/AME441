import matplotlib.pyplot as plt
import numpy as np
import math
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
		yaw[i]=float(data[1])+45
		pos[i]=float(data[2])+45

#Plot data
t=(t-t[0])/1000
toSum = np.subtract(yaw,pos)**2;
rms = math.sqrt(np.sum(toSum)/yaw.size)
print(rms)
plt.plot(t,yaw,'b',label="IMU Yaw")
plt.plot(t+0.02,pos,'r',label="Motor Position")
plt.axis("on")
#plt.legend("IMU Yaw", "Motor Position")

plt.xlabel("Time [s]",fontsize=18)
plt.xlim(left=0)
plt.ylabel("Angle [deg]",fontsize=18)
plt.yticks([-45,-30,-15,0,15,30,45])
plt.tick_params(labelsize=16)
plt.rc('font',size=18)
plt.legend()
plt.box(True)
plt.show()
print()

