
/* Time-stamp: <2007-11-14 22:31:31 shinya> */

#include "mpispec/mpispec.h"
#include "../src/cpi.h"
#include "../src/integral.h"
#include "../src/send_recv.h"

mpispec_prototype( fabs_spec )
mpispec_prototype( strcmp_spec )
mpispec_prototype( cpi_spec )
mpispec_prototype( send_recv_spec )

mpispec_init

/* run all ranks */
MPISpec_Run( strcmp_spec );

if( mpispec_rank == 0 ) {
  MPISpec_Run( cpi_spec );
  MPISpec_Run( send_recv_spec );
}
else {
  cpi();
}
if( mpispec_rank == 1 ) {
  MPISpec_Run( send_recv_spec );
  MPISpec_Run( fabs_spec );
}

mpispec_finalize
