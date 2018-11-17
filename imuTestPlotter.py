import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np
import math

#Params
dataSet = 2;
s=5


#Open data file
count = 0
imuFile="data/newimudata6.txt"
motorFile="data/newmotordata6.txt"
with open(imuFile,"r") as f:
	#Get number of data entries
	for line in f:
		count=count+1

#Set up empty arrays
t=np.empty([count,1])
roll=np.empty([count,1])

with open(imuFile,"r") as f:
	#Parse data from file
	for i,line in enumerate(f):
		data=line.split(",")
		t[i]=data[0]
		roll[i]=data[1]
		
roll = (roll)

count1 = 0
with open(motorFile,"r") as f:
	#Get number of data entries
	for line in f:
		count1=count1+1
t1=np.empty([count1,1])
pos=np.empty([count1,1])
with open(motorFile,"r") as f:
	#Parse data from file
	for i,line in enumerate(f):
		data=line.split(",")
		t1[i]=data[0]
		pos[i]=data[1]

"""
for p in pos:
	if p>90:
		p = 90-p
	if p<-90:
		p = -90-p

		
"""

pos=-pos;
for i in range(0,count):
	
	
	
	
	num=pos[i]//360
	if (num>0 and num%2==0):
		pos[i] = pos[i]-num*360
	if (num>0 and num%2==1):
		pos[i] = (num+1)*360-pos[i]




"""
	if (180<pos[i]<=360):
		pos[i]=360-pos[i]
	if (360<pos[i]<=540):
		pos[i]=pos[i]-360
	if (540<pos[i]<=720):
		pos[i]=720-pos[i]
"""
print(pos[-1])
print(180*roll[-1]/math.pi)

print("e= "+str(math.fabs(pos[-1]-180*roll[-1]/math.pi)))


#Plot data
t=(t-t[s])/1000
t1=(t1-t1[0])/1000

plt.plot(t[s:count-1],180*roll[s:count-1]/math.pi,'b',label="IMU Roll")
plt.plot(t1,pos,'r',label="Motor Position")
#plt.legend("IMU Yaw", "Motor Position")
plt.ylim(0,360)
print(t[-1])
plt.xlabel("Time [s]")
plt.ylabel("Angle [deg]")
plt.legend()
plt.box(True)
plt.show()

