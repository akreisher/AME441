import matplotlib.pyplot as plt
import numpy as np

#Open file
f=open(filename,"r");

#Count lines
count = 0;
for line in f:
	count=count+1;
	
#init arrays
t=np.zeros([count,1]);
a=np.zeros([count,10]);

#parse data
for i,line in enumerate(f):
	l=line.split(",");
	t[i]=l[0];
	a[i,:]=l[1:11];
	
#plot data
for i in range(10)
	plt.plot(t,a[:,i]);
plt.legend(range(0.2,2.2,0.2));
plt.show();


