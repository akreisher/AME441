import matplotlib.pyplot as plt
import numpy as np



f=open("testfile.txt","r");
count = 0;
for line in f:
	count=count+1;
	
t=np.zeros([count,1]);
f.close();
a=np.zeros([count,5]);
f=open("testfile.txt","r");
for i,line in enumerate(f):
	l=line.split(",");
	t[i]=l[0];
	a[i,:]=l[1:6];

for i in range(5):
	plt.plot(t,a[:,i]);
plt.legend(range(5));
plt.show();

