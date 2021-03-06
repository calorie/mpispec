#include "mpispec/mpispec.h"
#include "../src/integral.h"

static double f(double x) { return x; }
static double pi_func(double x) { return 4.0 / (1.0 + x * x); }

MPISPEC_DEF(integral_spec)
  DESCRIBE("integral function")
    IT("return integral")
      SHOULD_EQUAL_DOUBLE(integral(0, 10, 100, f), 50.0000000, 0.001)
      SHOULD_EQUAL_DOUBLE(integral(0.0, 1.0, 100, pi_func), 3.14159265, 0.001)
    END
  END
END
