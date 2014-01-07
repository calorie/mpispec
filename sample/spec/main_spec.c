#include "mpispec/mpispec.h"
#include "../src/cpi.h"
#include "../src/integral.h"
#include "../src/send_recv.h"

mpispec_init
  MPISpec_xRun(cpi_spec)

  int strcmp_spec_ranks[2] = {0, 1};
  ex_ranks(strcmp_spec_ranks)
    MPISpec_jxRun(strcmp_spec)
  end

  rank(1)
    MPISpec_Run(fabs_spec)
  end
mpispec_fin
