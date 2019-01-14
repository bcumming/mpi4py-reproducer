#  Reproducer of MPI4PY linking issue on Daint.

```
git clone https://github.com/bcumming/mpi4py-reproducer.git --recursive

module load daint-mc
module swap PrgEnv-cray PrgEnv-gnu  
module load CMake
module load cray-python/3.6.5.1

export CRAYPE_LINK_TYPE=dynamic
export CC=`which cc`; export CXX=`which CC`

# if you pick gcc 7 the error will be reproduced
# gcc 5 and 6 work
#module swap gcc/7.1.0 
#module swap gcc/5.3.0
#module swap gcc/6.1.0

mkdir build
cd build
cmake ..
```

First, check which mpich library mpi4py is linked against
```
> ldd /opt/python/3.6.5.1/lib/python3.6/site-packages/mpi4py/MPI.cpython-36m-x86_64-linux-gnu.so | grep mpich
    libmpich_gnu_51.so.3 => /opt/cray/pe/lib64/libmpich_gnu_51.so.3 (0x00002b0446eec000))
```

Below, both gcc 5 and 6 link against `/opt/cray/pe/lib64/libmpich_gnu_51.so.3`

## gcc 5.3

```
> module swap gcc/5.3.0
> cmake ..
> ldd mpitest.cpython-36m-x86_64-linux-gnu.so | grep libmpich
    libmpich_gnu_51.so.3 => /opt/cray/pe/lib64/libmpich_gnu_51.so.3 (0x00002b99d7132000)
> srun -n1 python ../test.py
MPI has not been initialized
MPI has been initialized
according to pybind: rank 0 of 1
according to mpi4py: 0 of 1
```

## gcc 6.1

```
> module swap gcc/6.1.0
> cmake ..
> ldd mpitest.cpython-36m-x86_64-linux-gnu.so | grep libmpich
    libmpich_gnu_51.so.3 => /opt/cray/pe/lib64/libmpich_gnu_51.so.3 (0x00002b99d7132000)
> srun -n1 python ../test.py
MPI has not been initialized
MPI has been initialized
according to pybind: rank 0 of 1
according to mpi4py: 0 of 1
```

## gcc 7.3

But when compiled with gcc, a different mpich library is linked against our target.

```
> module swap gcc/7.3.0
> cmake ..
> ldd mpitest.cpython-36m-x86_64-linux-gnu.so | grep libmpich
    libmpich_gnu_71.so.3 => /opt/cray/pe/mpt/7.7.2/gni/mpich-gnu/7.1/lib/libmpich_gnu_71.so.3 (0x00002b67f8965000)
> srun -n1 python ../test.py
MPI has not been initialized
MPI has not been initialized
Attempting to use an MPI routine before initializing MPICH
srun: error: nid00056: task 0: Exited with exit code 1
srun: Terminating job step 11406222.10
```

## Local Fix

I fixed the problem locally by installing my own version of mpi4py built with the toolchain used by my project.

```
#module list
#Currently Loaded Modulefiles:
#  1) modules/3.2.10.6
#  2) gcc/7.3.0
#  3) craype-broadwell
#  4) craype-network-aries
#  5) craype/2.5.15
#  6) cray-mpich/7.7.2
# ....                                                                                                   
# 22) PrgEnv-gnu/6.0.4
# 23) daint-mc
# 24) CMake/.3.12.0
# 25) CrayGNU/.18.08
# 26) cray-python/3.6.5.1
# 27) PyExtensions/3.6.5.1-CrayGNU-18.08

git clone git@github.com:mpi4py/mpi4py.git --recursive
cd mpi4py
git checkout 3.0.0
python setup.py build --mpicc=`which cc` 
python setup.py install --user

# The result is installed in ~/.local/lib/python3.6/site-packages/
```
