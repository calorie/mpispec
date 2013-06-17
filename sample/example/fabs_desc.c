#include <math.h>
#include "mpispec/cspec.h"

describe(fabs, "double fabs( double arg )")

  it( "returns the same number if the input number is positive" )
    should_equal( fabs(1.0), 1.0)
    should_equal( fabs(0.0), 0.0)
    should_equal( fabs(6.7), 6.7)
  end_it

  it( "returns the opposite number if the number is negative"  )
    should_equal( fabs(-1.0), 1.0)
    should_equal( fabs(-6.7), 6.7)
  end_it

end_describe

