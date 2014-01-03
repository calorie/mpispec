#include <math.h>
#include <stdlib.h>
#include "mpispec/mpispec.h"

int n = 1;

mpispec_def(fabs_spec)
  describe("function fabs")
    context("arg is 0")
      it("return same value")
        expect_to_equal(fabs(0), 0)
      end
    end

    context("arg is positive")
      it("return same value")
        expect_to_equal(fabs(n), n)
      end
    end

    context("arg is negative")
      it("return positive value")
        expect_to_equal(fabs(-n), n)
      end
    end
  end
end
