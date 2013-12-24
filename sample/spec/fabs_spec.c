#include <math.h>
#include <stdlib.h>
#include "mpispec/mpispec.h"

int n = 1;

mpispec_def(fabs_spec)
  describe("標準ライブラリfabsの")
    context("入力が0の時")
      it("同じ数値を返す")
        expect_to_equal(fabs(0), 0)
      end
    end

#ifndef __clang__
    before_each(make_random_num)
      n = rand();
    end
    after_each(make_random_num)
      printf("%d\n", n+1);
    end
#endif

    context("入力が正の数の時")
      it("同じ数値を返す")
        expect_to_equal(fabs(n), n)
      end
    end

    context("入力が負の数の時")
      it("正の数を返す")
        expect_to_equal(fabs(-n), n)
      end
    end
  end
end
