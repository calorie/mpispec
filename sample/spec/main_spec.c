#include "mpispec/mpispec.h"
#include "../src/cpi.h"
#include "../src/integral.h"

mpispec_init

  MPISpec_XmlRun(cpi_spec)

  int strcmp_spec_ranks[2] = {0, 1};
  ex_ranks(strcmp_spec_ranks)
    MPISpec_XmlRun(strcmp_spec)
    MPISpec_JUnitXmlRun(strcmp_spec)
  end

  rank(1)
    MPISpec_VerboseRun(fabs_spec)
  end

mpispec_fin
