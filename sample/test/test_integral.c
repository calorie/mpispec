
/* Time-stamp: <2007-11-14 22:31:27 shinya> */

#include "mpispec/mpiunit.h"
#include "../src/integral.h"

static double
f(double x)
{
  return x;
}

static double
pi_func(double x)
{
  return 4.0 / (1.0 + x * x);
}

/* Define tests */
MPIUT_DEF(test_integral)
{
  if(MPIUT_RANK == 0) {
    MPIUT_ASSERT_DOUBLE_EQUAL(integral(0, 10, 100, f),
                              50.0000000, 0.001);
    MPIUT_ASSERT_DOUBLE_EQUAL(integral(0.0, 1.0, 100, pi_func),
                              3.14159265, 0.001);
  }
  else {
    integral(0, 10, 100, f);
    integral(0, 1.0, 100, pi_func);
  }
}
