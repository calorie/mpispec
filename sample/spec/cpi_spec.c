#include "mpispec/mpispec.h"
#include "../src/cpi.h"

MPISPEC_DEF(cpi_spec)
  DESCRIBE("function cpi")
    RANK(0)
      CONTEXT("rank0")
        IT("return pi")
          SHOULD_EQUAL_DOUBLE(cpi(), 3.1415926535, 0.001)
        END
      END
    END
    EX_RANK(0)
      CONTEXT("rank0")
        IT("return 0")
          SHOULD_EQUAL_DOUBLE(cpi(), 0.0, 0.001)
        END
      END
    END
  END
END
