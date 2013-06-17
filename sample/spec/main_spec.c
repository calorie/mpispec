
/* Time-stamp: <2007-11-14 22:31:31 shinya> */

#include "mpispec/mpispec.h"

/* Prototypes */
mpispec_prototype(fabs_spec)
/* MPIUT_PROTOTYPE(test_cpi); */
/* MPIUT_PROTOTYPE(test_integral); */
/* MPIUT_PROTOTYPE(test_mpiassertion); */

/* Register */
mpispec_init

mpispec_register("mpi fabs", fabs_spec)
/* MPIUT_REGISTER("mpi cpi", test_cpi); */
/* MPIUT_REGISTER("mpi integral", test_integral); */
/* MPIUT_REGISTER("mpi assertion", test_mpiassertion); */

mpispec_finalize
