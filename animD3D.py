import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d.axes3d as p3
import matplotlib.animation as animation
import numpy as np
from mpl_toolkits.mplot3d import proj3d
 
def orthogonal_proj(zfront, zback):
    a = (zfront+zback)/(zfront-zback)
    b = -2*(zfront*zback)/(zfront-zback)
    return np.array([[1,0,0,0],
                        [0,1,0,0],
                        [0,0,a,b],
                        [0,0,-0.0001,zback]])
n=25
def update_plot(frame,data,lines):
	if (frame<=n):
		return lines
	frame = frame - n
	for line in lines:
		line.set_data([0,data[0,frame],data[3,frame]],[0,-data[1,frame],-data[4,frame]],)
		line.set_3d_properties([0,data[2,frame],data[5,frame]])
	return lines


count = 0
with open("data/dqfile.txt") as f:
	for line in f:
		count = count +1
		
data = np.empty([6,count])
with open("data/dqfile.txt") as f:
	for i,line in enumerate(f):
		dat = line.split(",")
		data[1,i]=float(dat[0])
		data[0,i]=float(dat[1])
		data[2,i]=float(dat[2])
		data[4,i]=float(dat[3])+float(dat[0])
		data[3,i]=float(dat[4])+float(dat[1])
		data[5,i]=float(dat[5])+float(dat[2])

fig = plt.figure()
ax = p3.Axes3D(fig)

line = ax.plot([0,data[0,0],data[3,0]],[0,-data[1,0],-data[4,0]],[0,data[2,0],data[5,0]],linewidth=5)
dots = ax.plot([0,data[0,0],data[3,0]],[0,-data[1,0],-data[4,0]],[0,data[2,0],data[5,0]],linestyle="",marker="o",c='r',mew=6,ms=3)

lines=line+dots

proj3d.persp_transformation = orthogonal_proj
print(type(dots))

ax.set_xlim3d([0,4.0])
ax.set_xlabel("Y")
ax.set_ylim3d([-2,2.0])
ax.set_ylabel("X")
ax.set_zlim3d([-2.0,2.0])
ax.set_zlabel("Z")
#ax.view_init(0,90)

line_ani= animation.FuncAnimation(fig,update_plot,count+n, fargs=(data,lines), interval=140, blit=True)
		
plt.show()
