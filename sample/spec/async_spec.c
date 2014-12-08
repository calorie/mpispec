#include "mpispec/mpispec.h"
#include "../src/async.h"

MPISPEC_DEF(async_spec)
  DESCRIBE("function async")
    int val = 1, from = 0, to = 1, tag = 0;
    STUB_NON_BLOCKING_P2P(&val, from, to, tag)

    RANK(0)
      CONTEXT("rank0")
        IT("return 1")
          SHOULD_EQUAL(async(), 1)
        END
      END
    END
    RANK(1)
      CONTEXT("rank1")
        IT("return 1")
          SHOULD_EQUAL(async(), 1)
        END
      END
    END
  END
END
