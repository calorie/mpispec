#include "mpispec.h"

void
MPISpec_Setup(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    MPISpec_Basic_Setup();
}

void
MPISpec_Dispatch(void)
{
    MPISpec_JUnitXmlFileClose();
    MPISpec_XmlFileClose();
    MPISpec_Run_Summary();
    MPISpec_Result_File_Close();
    MPI_Barrier(MPI_COMM_WORLD);
    MPISpec_Display_Results();
    MPISpec_Redis_Disconnect();
    MPI_Finalize();
}

int
MPISpec_Rank(void)
{
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    return myrank;
}
