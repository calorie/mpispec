#include <stdio.h>
#include "mpispec/mpispec.h"

MPISPEC_DEF(strcmp_spec)
  DESCRIBE("function strcmp")
    CONTEXT("if same word")
      IT("return 0")
        SHOULD_EQUAL(strcmp("hello", "hello"), 0)
      END
    END

    CONTEXT("if not same word")
      IT("return NULL")
        SHOULD_NOT_BE_NULL(strcmp("hello", "world"))
      END

      CONTEXT("arg1 smaller than arg2")
        IT("return negative")
          SHOULD_BE_TRUE(strcmp("hello", "world") < 0)
          SHOULD_BE_TRUE(strcmp("0123", "1321431") < 0)
        END
      END

      CONTEXT("arg1 larger than arg2")
        IT("retrun positive")
          SHOULD_BE_TRUE(strcmp("yellow", "world") > 0)
          SHOULD_BE_TRUE(strcmp("9", "789") > 0)
        END
      END
    END
  END
END
