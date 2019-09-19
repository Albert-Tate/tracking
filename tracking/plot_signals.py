import matplotlib
import csv
import matplotlib.pyplot as plt
import numpy as np

def read_all(PATH):
    with open(PATH) as infile:
        ltemp = []
        csvreader = csv.reader(infile, delimiter=',')
        for row in csvreader:
            ltemp.append(row)

    for i in ltemp[0][0:-2]:
        print(i)
        float(i)
    return ltemp
    return(np.asfarray([l[0:-2] for l in ltemp], dtype=np.float32))

VIN_PATH = "../AD2iface/vin.txt"
VOUT_PATH = "../AD2iface/vout.txt"

vin = read_all(VIN_PATH)
vout = read_all(VOUT_PATH)



fig, ax = plt.subplots(nrows=2, ncols=1, sharex = True);

ax[0].plot(vin[1][:])
ax[1].plot(vout[1][:])

ax[0].set(xlabel = 'Sample Number', ylabel='voltage', title='Input')
ax[0].grid
ax[1].set(xlabel = 'Sample Number', ylabel='voltage', title='Output')
ax[1].grid

plt.setp(ax, yticks=np.arange(-5,5,0.1))

plt.show()


