#include "mpiunit/mpiunit.h"
#include "../src/integral.h"

#define PI 3.14159265

static double
pi_func(double x)
{
  return 4.0 / (1.0 + x * x);
}

MPIUT_DEF(test_integral)
{
  if(MPIUT_RANK == 0) {
    MPIUT_ASSERT_DOUBLE_EQUAL(integral(0.0, 1.0, 100, pi_func), PI, 0.001);
  }
  else {
    integral(0, 1.0, 100, pi_func);
  }
}

MPIUT_INIT;
MPIUT_REGISTER("test integral", test_integral);
MPIUT_FINALIZE;
