#include <math.h>
#include "mpispec/cspec.h"
#include "mpispec/cspec_private.h"
#include "mpispec/cspec_output_header.h"
#include "mpispec/cspec_output_verbose.h"
#include "mpispec/cspec_output_unit.h"
#include <mpi.h>
#include "mpispec/mpiunit.h"
#include "../src/cpi.h"

describe(fabs, "double fabs( double arg )")

  it( "returns the same number if the input number is positive" )
    should_equal( fabs(1.0), 1.0)
    should_equal( fabs(0.0), 0.0)
    should_equal( fabs(6.7), 6.7)
  end

  it( "returns the opposite number if the number is negative"  )
    should_equal( fabs(-1.1), 1.0)
    should_equal( fabs(-6.7), 6.7)
  end

end

/* Define tests */
MPIUT_DEF(fabs_spec)
{
  if(MPIUT_RANK == 0) {
    /* MPIUT_ASSERT_DOUBLE_EQUAL(cpi(), 3.1415926535, 0.001); */
    CSpec_Run( description( fabs ), CSpec_NewOutputVerbose() );
  }
  else if(MPIUT_RANK == 1) {
      /* MPIUT_ASSERT_DOUBLE_EQUAL(cpi(), 3.1415926535, 0.001); */
      CSpec_Run( description( fabs ), CSpec_NewOutputVerbose() );
  }
  else {
    /* cpi(); */
  }
}

