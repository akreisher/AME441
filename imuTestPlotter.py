import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np

#Params
dataSet = 2;
s=2


#Open data file
count = 0
imuFile="data/imudata"+str(dataSet)+".txt"
motorFile="data/motordata"+str(dataSet)+".txt"
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

for i in range(0,count1):
	
	
	
	
	num=pos[i]//180
	if (num>0 and num%2==0):
		pos[i] = pos[i]-num*180
	if (num>0 and num%2==1):
		pos[i] = (num+1)*180-pos[i]

pos = pos-90

"""
	if (180<pos[i]<=360):
		pos[i]=360-pos[i]
	if (360<pos[i]<=540):
		pos[i]=pos[i]-360
	if (540<pos[i]<=720):
		pos[i]=720-pos[i]
"""





#Plot data
t=(t-t[s])/1000
t1=(t1-t1[0])/1000
plt.plot(t[s:count-1],roll[s:count-1],'b',label="IMU Roll")
plt.plot(t1,pos,'r',label="Motor Position")
#plt.legend("IMU Yaw", "Motor Position")
plt.xlim([-0.1,6.1])
plt.ylim([-95,95])
plt.yticks([-90,-60,-30,0,30,60,90])
plt.xlabel("Time [s]")
plt.ylabel("Angle [deg]")
plt.legend()
plt.box(True)
plt.show()

