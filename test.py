import mpitest

mpitest.test_mpi_status()

from mpi4py import MPI

mpi_rank = MPI.COMM_WORLD.rank
mpi_size = MPI.COMM_WORLD.size
print("according to mpi4py:", mpi_rank, "of", mpi_size)

mpitest.test_mpi_status()
mpitest.print_mpi_info()
