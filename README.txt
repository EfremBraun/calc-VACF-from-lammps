----------Installation----------
To compile you will need cmake (sudo apt-get install cmake if it is not installed already): 
$ tar -xf corrvacf.tar.bz2
$ cd corrvacf/build
$ cmake ../
$ make
$ make install

this will create a binary and put it in the corrvacf/bin folder. Put this in your binary folder for your convenience. 

Note that on NERSC's Hopper system, the following 2 commands have been found to be necessary prior to installation.
module load cmake
module swap PrgEnv-pgi PrgEnv-gnu

----------Input File----------
./corrfunc -p will print a sample input-file. 
The meaning of 'blocks' and 'elements' should be obvious from the order-n algorithm. If you want to maximize sampling and you don't care about memory/CPU usage, set blocks to1 and elements to 1 more than the number of timesteps.
The timestep is the difference in time (in femtoseconds) between each of the snapshots provided in the data files.
For the VACF needed to compute the phonon DOS, particle types don't matter, so you'd set particle types to 1 and have one dump file that contains all of their velocities.
For generality, you can compute the VACF for each kind of particle in the system and calculate the cross-VACFs between particle types. In that case, each of the number of particle types specified must have its own dump file. I don't know if this is ever used for anything, but I see no need to remove it from the code.

----------Data Files----------
The format of these data files is the one given by a LAMMPS dump file (valid per version 14May16) per the command:
    dump 1 all custom 1 traj.dump id vx vy vz
Format:
    There is a cycle of 9 header lines for each timestep followed by the velocities of all atoms at that timestep.
    The header lines are all ignored. The code reads each snapshot as being 1 timestep from the last snapshot. It is converted into real time using the value given in the input file.
    The position lines: ID vx vy vz
        ID is the ID of the atom (or COM of the molecule). This is used since the order of particles in the dump file changes.
        vx, vy, and vz are the velocities of the atom.

----------Testing and Running----------
The program should be run from the directory in which you have stored the input.vacf and dump file.
In the 'test' directory, a small trajectory is created for which the VACF can be computed by hand and compared with the program's output. LAMMPS's 'compute vacf' function is shown to give poor statistics.

----------Reading the source code---------
I took the source code from one that was used to calculate the MSD, and I didn't change the names of variables. So to make sense of these variables:
PosX                = VelX of particle at a given timestep
CentreOfMassX[comp] = average velocity of the particles of type 'comp' at a given timestep
ParticleX           = vector of particle velocities that covers all timesteps
CenterOfMassX[comp] = vector of average velocities of the particles of type 'comp' that covers all timesteps
msdx                = velocity at the time origin times the velocity at the current time
SampleMSD->SelfX    = running sum of msdx which gets divided by counts at the end to get average VACF
