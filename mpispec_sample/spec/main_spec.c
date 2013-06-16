
/* Time-stamp: <2007-11-14 22:31:31 shinya> */

#include <stdlib.h>
#include <mpi.h>
#include "mpispec/mpiunit.h"

/* Prototypes */
MPIUT_PROTOTYPE(fabs_spec);
/* MPIUT_PROTOTYPE(test_cpi); */
/* MPIUT_PROTOTYPE(test_integral); */
/* MPIUT_PROTOTYPE(test_mpiassertion); */

/* Register */
MPIUT_INIT;

MPIUT_REGISTER("mpi fabs", fabs_spec);
/* MPIUT_REGISTER("mpi cpi", test_cpi); */
/* MPIUT_REGISTER("mpi integral", test_integral); */
/* MPIUT_REGISTER("mpi assertion", test_mpiassertion); */

MPIUT_FINALIZE;
