
/* Time-stamp: <2007-11-14 22:31:31 shinya> */

#include "mpispec/mpispec.h"

mpispec_prototype(fabs_spec)
mpispec_prototype(fab_spec)

mpispec_init

if(MPIUT_RANK == 0) {
  MPISpec_Run(fabs_spec);
}
else if(MPIUT_RANK == 1) {
  MPISpec_Run(fab_spec);
}
else {
}

mpispec_finalize
