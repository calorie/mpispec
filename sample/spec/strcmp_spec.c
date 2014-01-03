#include <stdio.h>
#include "mpispec/mpispec.h"

mpispec_def(strcmp_spec)
  describe("function strcmp")
    context("if same word")
      it("return 0")
        expect_to_equal(strcmp("hello", "hello"), 0)
      end
    end

    context("if not same word")
      it("return NULL")
        expect_not_to_be_null(strcmp("hello", "world"))
      end

      context("arg1 smaller than arg2")
        it("return negative")
          expect_to_be_true(strcmp("hello", "world") < 0)
          expect_to_be_true(strcmp("0123", "1321431") < 0)
        end
      end

      context("arg1 larger than arg2")
        it("retrun positive")
          expect_to_be_true(strcmp("yellow", "world") > 0)
          expect_to_be_true(strcmp("9", "789") > 0)
        end
      end
    end
  end
end
