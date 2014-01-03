#include "mpispec/mpispec.h"
#include "../src/cpi.h"

mpispec_def(cpi_spec)
  describe("function cpi")
    rank(0)
      context("rank0")
        it("return pi")
          expect_to_equal_double(cpi(), 3.1415926535, 0.001)
        end
      end
    end
    ex_rank(0)
      context("rank0")
        it("return 0")
          expect_to_equal_double(cpi(), 0.0, 0.001)
        end
      end
    end
  end
end
