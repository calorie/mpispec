
/* Time-stamp: <2007-11-14 22:31:31 shinya> */

#include "mpispec/mpispec.h"
#include "../src/cpi.h"
#include "../src/integral.h"
#include "../src/send_recv.h"

mpispec_init

  MPISpec_Run( cpi_spec )

  int myranks[2] = {0, 1};
  ex_ranks( myranks )

    MPISpec_Run( strcmp_spec )

  end

  rank( 1 )

    MPISpec_Run( fabs_spec )

  end

mpispec_fin
