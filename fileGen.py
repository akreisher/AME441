import numpy as np

f = open("testfile.txt","w");

for i in range(100):
	f.write(str(i));
	
	a=np.random.rand(5,1);
	
	for j in range(5):
		f.write(",");
		a[j]=(j)+a[j];
		f.write(str(a.item(j)));
	f.write("\n");
	f.flush();
f.close();

	
