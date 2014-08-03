#include <math.h>
#include <stdlib.h>
#include "mpispec/mpispec.h"

int n = 1;

MPISPEC_DEF(fabs_spec)
  DESCRIBE("function fabs")
    CONTEXT("arg is 0")
      IT("return same value")
        SHOULD_EQUAL(fabs(0), 0)
      END
    END

    CONTEXT("arg is positive")
      IT("return same value")
        SHOULD_EQUAL(fabs(n), n)
      END
    END

    CONTEXT("arg is negative")
      IT("return positive value")
        SHOULD_EQUAL(fabs(-n), n)
      END
    END
  END
END
