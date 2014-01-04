#include <mpi.h>
#include "unity.h"
#include "mpispec_output_verbose.h"

CSpecOutputStruct* verbose;

void setUp(void)
{
  verbose = CSpec_NewOutputVerbose();
}

void tearDown(void)
{
}

void test_endFunVerbose(void)
{
}
