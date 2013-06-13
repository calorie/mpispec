
/* Time-stamp: <2007-11-24 22:06:19 shinya> */

#include <mpi.h>
#include "mpiunit/mpiunit.h"
#include "../src/cpi.h"

/* Define tests */
MPIUT_DEF(test_cpi)
{
  if(MPIUT_RANK == 0) {
    MPIUT_ASSERT_DOUBLE_EQUAL(cpi(), 3.1415926535, 0.001);
  }
  else {
    cpi();
  }
}
