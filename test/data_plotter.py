#data_plotter.py


import numpy as np
import matplotlib.pyplot as plt

 
filename1 = "performance/e_data_log.log"
filename2 = "performance/r_data_log.log"

f1=open(filename1,"r")
f2=open(filename2,"r")

lines1=f1.readlines()
lines2=f2.readlines()

x1 = []
for line in lines1:
	x1.append(float(line))

x2 = []
for line in lines2:
	x2.append(float(line))

#print(x1)

t = np.arange(0, len(lines1), 1)

# Two signals with a coherent part at 10Hz and a random part

fig, axs = plt.subplots()
axs.plot(t, x1, t, x2)
axs.set_xlabel('time')
axs.set_ylabel('id0 replica (blue) and state0 replica (orange)')
#axs.grid()

plt.show()