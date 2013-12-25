#include "mpispec/mpispec.h"
#include "../src/cpi.h"
#include "../src/integral.h"
#include "../src/send_recv.h"

mpispec_init
  MPISpec_Run(cpi_spec)

  int strcmp_spec_ranks[2] = {0, 1};
  ex_ranks(strcmp_spec_ranks)
    MPISpec_Run(strcmp_spec)
  end

  rank(1)
    MPISpec_JUnitXmlFileOpen("output.xml", "utf-8");
    MPISpec_jxRun(fabs_spec)
    MPISpec_JUnitXmlFileClose();
  end
mpispec_fin
