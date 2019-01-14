#include <iostream>
#include <cstdlib>

#include <pybind11/pybind11.h>

#include <mpi.h>

void test_mpi_error(int ecode) {
    if (ecode!=MPI_SUCCESS) {
        static char buf[256];
        int len;
        MPI_Error_string(ecode, buf, &len);
        std::cerr << "MPI ERROR: " << buf << std::endl;
        std::exit(1);
    }
}

void test_mpi_status() {
    int flag;
    test_mpi_error(MPI_Initialized(&flag));

    std::cout << (flag? "MPI has been initialized": "MPI has not been initialized") << std::endl;
}

void print_mpi_info() {
    int rank, size;
    test_mpi_error(MPI_Comm_size(MPI_COMM_WORLD, &size));
    test_mpi_error(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
    std::cout << "according to pybind: rank " << rank << " of " << size << std::endl;
}

PYBIND11_MODULE(mpitest, m) {
    m.def("test_mpi_status", &test_mpi_status, "Prints a message about MPI has been initialized.");
    m.def("print_mpi_info", &print_mpi_info, "Prints a message with rank and communicator size.");
}
