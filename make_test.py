#! /usr/bin/env python

"""
Build a test-case to calculate mean-square displacements.
We have a particle that moves on a grid. We print the position every N-timestep

"""
TIMESTEPS = 500000
BOXX = 100.0
BOXY = 100.0
BOXZ = 100.0
STRIDE = 0.1


# one particle, moving in x (f1), y(f2), and z(f3) direction
f1 = open("test1.dat","w")
f2 = open("test2.dat","w")
f3 = open("test3.dat","w")

f1.write("# test-case for msd-calculator. One particle moving in x-direction\n")
f2.write("# test-case for msd-calculator. One particle moving in y-direction\n")
f3.write("# test-case for msd-calculator. One particle moving in z-direction\n")
f1.write("# File containes %i timesteps.\n" % TIMESTEPS)
f2.write("# File containes %i timesteps.\n" % TIMESTEPS)
f3.write("# File containes %i timesteps.\n" % TIMESTEPS)
f1.write("# \n")
f2.write("# \n")
f3.write("# \n")


abs_pos = 0.0

for i in xrange(TIMESTEPS+1):
    f1.write("%i    %i\n" % (i+1,1))
    f2.write("%i    %i\n" % (i+1,1))
    f3.write("%i    %i\n" % (i+1,1))


    ix = int(abs_pos / BOXX)
    iy = int(abs_pos / BOXY)
    iz = int(abs_pos / BOXZ)

    posx = abs_pos - float(BOXX*ix)
    posy = abs_pos - float(BOXY*iy)
    posz = abs_pos - float(BOXZ*iz)

    f1.write("%i   %f   %f  %f   %i     %i  %i\n" % (1,posx,0.0,0.0,ix,0,0))
    f2.write("%i   %f   %f  %f   %i     %i  %i\n" % (1,0.0,posy,0.0,0,iy,0))
    f3.write("%i   %f   %f  %f   %i     %i  %i\n" % (1,0.0,0.0,posz,0,0,iz))
    
    abs_pos += STRIDE


f1.close()
f2.close()
f3.close()
