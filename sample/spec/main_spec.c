#include "mpispec/mpispec.h"
#include "../src/cpi.h"
#include "../src/integral.h"

MPISPEC_INIT

  MPISpec_XmlRun(cpi_spec)

  int strcmp_spec_ranks[2] = {0, 1};
  EX_RANKS(strcmp_spec_ranks)
    MPISpec_XmlRun(strcmp_spec)
    MPISpec_JUnitXmlRun(strcmp_spec)
  END

  RANK(1)
    MPISpec_VerboseRun(fabs_spec)
  END

MPISPEC_FINALIZE
