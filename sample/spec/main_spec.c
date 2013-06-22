
/* Time-stamp: <2007-11-14 22:31:31 shinya> */

#include "mpispec/mpispec.h"
#include "../src/cpi.h"
#include "../src/integral.h"

mpispec_prototype(fabs_spec)
mpispec_prototype(strcmp_spec)
mpispec_prototype(cpi_spec)

mpispec_init

/* run all ranks */
MPISpec_Run(strcmp_spec);

if(MPIUT_RANK == 0) {
  MPISpec_Run(cpi_spec);
}
else {
  cpi();
}
if(MPIUT_RANK == 1) {
  MPISpec_Run(fabs_spec);
}

mpispec_finalize
