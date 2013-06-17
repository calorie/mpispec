
/* Time-stamp: <2007-11-24 22:06:19 shinya> */

#include <mpi.h>
#include "mpispec/mpiunit.h"
#include "../src/cpi.h"

/* Define tests */
MPIUT_DEF(test_cpi)
{
  if(MPIUT_RANK == 0) {
    /* CSpec_Run( description( fabs ), CSpec_NewOutputVerbose() ); */
    MPIUT_ASSERT_DOUBLE_EQUAL(cpi(), 3.1415926535, 0.001);
  }
  else {
    cpi();
  }
}
