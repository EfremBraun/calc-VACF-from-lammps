----------Installation----------
To compile you will need cmake (sudo apt-get install cmake if it is not installed already): 
$ tar -xf corrmsd.tar.bz2
$ cd corrmsd/build
$ cmake ../
$ make
$ make install

this will create a binary and put it in the corrmsd/bin folder. Put this in your binary folder for your convenience. 

Note that on NERSC's Hopper system, the following 2 commands have been found to be necessary prior to installation.
module load cmake
module swap PrgEnv-pgi PrgEnv-gnu

----------Input File----------
./corrfunc -p will print a sample input-file. 
The meaning of 'blocks' and 'elements' should be obvious from the order-n algorithm.
The box size is used to unwrap the coordinates of the particles.
The timestep is the difference in time between each of the snapshots provided in the data files.
The 'corr. drift' and 'ind.p' options do nothing.
Each of the number of particle types specified must have its own data file. Each of these files should be put into the input-file, as the sample one does for its 2 types of particles.
The masses of the particle types also do nothing.

----------Data Files----------
The format of these data files is the one given by LAMMPS (valid per version 1Feb14) if the time-integration is done using rigid/nvt and the COMs of each molecule are then output with ave/time. Ex.
    fix 1 Benzene rigid/nvt molecule temp 300.0 300.0 100.0
    fix 2 Benzene ave/time 1 1 5 f_1[1] f_1[2] f_1[3] f_1[13] f_1[14] f_1[15] file traj.CofMs mode vector
Format:
    The first 3 lines are ignored.
    There is then a cycle of a header line for each timestep followed by the positions of all atoms at that timestep.
    The header line: TimeStep Number-of-rows
        TimeStep is obvious.
        Number-of-rows is the number of rows following the header line until the next header line is found, i.e., the number of atoms.
        Both of these values are ignored. The code reads each snapshot as being 1 timestep from the last snapshot. It is converted into real time using the value given in the input file.
    The position lines: ID x y z ix iy iz
        ID is the ID of the atom (or COM of the molecule). The code actually ignores this value and reads in the list of atoms based on the order it's presented, so make sure the ordering stays the same for all timesteps.
        x, y, and z are the wrapped positions of the atom.
        ix, iy, and iz are the image flags of the atom, which the code uses to unwrap the position coordinates.

----------Testing and Running----------
The program should be run from the directory in which you have stored the input.msd and data files.
The program make_test.py will create a few input-files. They should give linear MSD for all times.

----------Notes and limitations----------
Note that the self and cross mean-squared displacements are printed, not the diffusion coefficients themselves.
The code only works for orthogonal unit cells.
