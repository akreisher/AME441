import matplotlib.pyplot as plt
import numpy as np



f=open("testfile.txt","r")
count = 0
for line in f:
	count=count+1
f.close()
t=np.empty([count,1])
yaw=np.empty([count,1])
pos=np.empty([count,1])
f=open("testfile.txt","r")
for i,line in enumerate(f):
	l=line.split(",")
	t[i]=l[0]
	yaw[i]=l[1]
	pos[i]=l[2]


plt.legend("IMU Yaw", "Motor Position")
plt.xlabel("Time [s]")
plt.ylabel("Angle [deg]")
plt.show()

