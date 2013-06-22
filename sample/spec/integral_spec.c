#include "mpispec/mpispec.h"
#include "mpispec/cspec_output_verbose.h"
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
mpispec_def(integral_spec)

  describe(integral, "integral関数は")

    it("積分値を返す")
      expect_to_equal_double(integral(0, 10, 100, f),
                             50.0000000, 0.001);
      expect_to_equal_double(integral(0.0, 1.0, 100, pi_func),
                             3.14159265, 0.001);
    end

  end

end_def
