import matplotlib
matplotlib.use('TkAgg')
import math
import numpy as np
import matplotlib.pyplot as plt

xDrift = np.empty([16,1])
yDrift = np.empty([16,1])
zDrift = np.empty([16,1])
t = np.empty([16,1])

for i in range(11,27):
	fileN = "data/double/double"+str(i)+".txt"

	with open(fileN) as f:
		data = f.readlines()
		last = data[-1].split(",");
		xDrift[i-11] = math.fabs(float(last[4])-1);
		
		yDrift[i-11] = math.fabs(float(last[3]))
		zDrift[i-11] = math.fabs(float(last[5]))
		t[i-11]=float(last[6])/1000.0
x=np.empty([4,1])
y=np.empty([4,1])
z=np.empty([4,1])
tn=np.empty([4,1])

x[0] = sum(xDrift[0:4])/5.0;
y[0] = sum(yDrift[0:4])/5.0;
z[0] = sum(zDrift[0:4])/5.0;
tn[0] = sum(t[0:4])/5.0;

x[1] = sum(xDrift[5:8])/4.0;
y[1] = sum(yDrift[5:8])/4.0;
z[1] = sum(zDrift[5:8])/4.0;
tn[1] = sum(t[5:8])/4.0;

x[2]= sum(xDrift[10:12])/3.0;
y[2] = sum(yDrift[10:12])/3.0;
z[2] = sum(zDrift[10:12])/3.0;
tn[2] = sum(t[10:12])/3.0;

x[3]= sum(xDrift[12:15])/3.0;
y[3] = sum(yDrift[12:15])/3.0;
z[3] = sum(zDrift[12:15])/3.0;
tn[3] = sum(t[12:15])/3.0;


plt.scatter(tn[0:2],x[0:2],label="X1")
plt.scatter(tn[0:2],y[0:2],label="Y1")
plt.scatter(tn[0:2],z[0:2],label="Z1")
plt.scatter(tn[2:4],x[2:4],label="X2")
plt.scatter(tn[2:4],y[2:4],label="Y2")
plt.scatter(tn[2:4],z[2:4],label="Z2")

plt.legend()
plt.show()
		
