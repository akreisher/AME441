import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d.axes3d as p3
import matplotlib.animation as animation
import numpy as np
from mpl_toolkits.mplot3d import proj3d
import collections
import math

l1=1
l2=1

Vec = collections.namedtuple('Vec',['x','y','z'])
State = collections.namedtuple('State',['t1','t2','t3'])

def cross(u,p):
	#cross product
	vx=u.y*p.z-u.z*p.y
	vy=u.z*p.x-p.z*u.x
	vz=u.x*p.y-u.y*p.x
	v = Vec(vx,vy,vz)
	return v


def ikin(goal):
	r = math.sqrt(goal.x**2+goal.y**2+goal.z**2)
	if (r>2):
		return State(-100,r,0)
	x=goal.x
	t1=math.atan2(-goal.z,goal.y)
	y=goal.y*math.cos(t1)-goal.z*math.sin(t1)
	t3=math.acos(((x**2)+(y**2)-(l1**2)-(l2**2))/(2*l1*l2))
	t2=math.atan2(y,x)-math.atan2(l2*math.sin(t3),l1+l2*math.cos(t3))
	return State(t1,t2,t3)

def fk(t1,t2,t3):
	#u components (first linkage)
	if (t1==-100):
		return([Vec(thetas.t1,thetas.t2,thetas.t3),Vec(0,0,0)])
	x1=math.cos(t2)
	y1=math.cos(t1)*math.sin(t2)	
	z1=-math.sin(t1)*math.sin(t2)
	u=Vec(x1,y1,z1)
	
	#plane defined by t1
	px=0;
	py=-math.sin(t1)
	pz=-math.cos(t1)
	p=Vec(px,py,pz)
	
	#vector perpendicular to u in plane p
	uPerp=cross(u,p)

	#components of v (second linkage)
	x2=u.x+uPerp.x*math.sin(t3)+u.x*math.cos(t3)
	y2=u.y+uPerp.y*math.sin(t3)+u.y*math.cos(t3)
	z2=u.z+uPerp.z*math.sin(t3)+u.z*math.cos(t3)
	v=Vec(x2,y2,z2)
	return [u,v]
 
def orthogonal_proj(zfront, zback):
    a = (zfront+zback)/(zfront-zback)
    b = -2*(zfront*zback)/(zfront-zback)
    return np.array([[1,0,0,0],
                        [0,1,0,0],
                        [0,0,a,b],
                        [0,0,-0.0001,zback]])

def update_plot(frame,data,model,lines):
	if (frame<=50):
		return lines
	frame = frame - 50
	lines[0].set_data([0,data[0,frame],data[3,frame]],[0,-data[1,frame],-data[4,frame]])
	lines[0].set_3d_properties([0,data[2,frame],data[5,frame]])

	if (model[0,frame]==-100):
		print("Cannot catch end of dist "+str(model[1,frame]))
	else:
		lines[1].set_data([0,model[0,frame],model[3,frame]],[0,model[1,frame],model[4,frame]],)
		lines[1].set_3d_properties([0,model[2,frame],model[5,frame]])

	lines[2].set_data([0,data[0,frame],data[3,frame]],[0,-data[1,frame],-data[4,frame]])
	lines[2].set_3d_properties([0,data[2,frame],data[5,frame]])

	lines[3].set_data([model[0,frame]],[model[1,frame]])
	lines[3].set_3d_properties([model[2,frame]])
	return lines


count = 0
with open("data/dqfile.txt") as f:
	for line in f:
		count = count +1
		
data = np.empty([6,count])
model = np.empty([6,count])
rms = np.zeros([3,1])

with open("data/dqfile.txt") as f:
	for i,line in enumerate(f):
		dat = line.split(",")
		data[1,i]=float(dat[0])#YData
		data[0,i]=float(dat[1])#XData
		data[2,i]=float(dat[2])#ZData
		data[4,i]=float(dat[3])+float(dat[0])
		data[3,i]=float(dat[4])+float(dat[1])
		data[5,i]=float(dat[5])+float(dat[2])
		thetas = ikin(Vec(data[3,i],-data[4,i],data[5,i]))
		vecs = fk(thetas.t1,thetas.t2,thetas.t3)
		u = vecs[0]
		v = vecs[1]
		model[0,i]=u.x
		model[1,i]=u.y
		model[2,i]=u.z
		model[3,i]=v.x
		model[4,i]=v.y
		model[5,i]=v.z
		rms[0] = rms[0]+((data[3,i]-model[3,i])**2)
		rms[1] = rms[1]+((-data[4,i]-model[4,i])**2)
		rms[2] = rms[2]+((data[5,i]-model[5,i])**2)
rms=rms/count
rms[0] = math.sqrt(rms[0])
rms[1] = math.sqrt(rms[1])
rms[2] = math.sqrt(rms[2])
print("rms x = "+str(rms[0]))
print("rms y = "+str(rms[1]))
print("rms z = "+str(rms[2]))








fig = plt.figure()
ax = p3.Axes3D(fig)

iline = ax.plot([0,data[0,0],data[3,0]],[0,-data[1,0],-data[4,0]],[0,data[2,0],data[5,0]],linewidth=4,label="IMU Data")
rline = ax.plot([0,model[0,0],model[3,0]],[0,model[1,0],model[4,0]],[0,model[2,0],model[5,0]],linewidth=4,c="g",label="Inverse Kinematics")
idots = ax.plot([0,data[0,0],data[3,0]],[0,-data[1,0],-data[4,0]],[0,data[2,0],data[5,0]],linestyle="",marker="o",c='r')
rdots = ax.plot([model[0,0]],[model[1,0]],[model[2,0]],linestyle="",marker="o",c='r')
lines=iline+rline+idots+rdots

proj3d.persp_transformation = orthogonal_proj
ax.legend()
ax.set_xlim3d([0,4.0])
ax.set_xlabel("Y")
ax.set_ylim3d([-2,2.0])
ax.set_ylabel("X")
ax.set_zlim3d([-2.0,2.0])
ax.set_zlabel("Z")
#ax.view_init(0,90)

line_ani= animation.FuncAnimation(fig,update_plot,count+50, fargs=(data,model,lines), interval=100, blit=True)
		
plt.show()
