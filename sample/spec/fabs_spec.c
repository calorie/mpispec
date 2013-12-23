#include <math.h>
#include <stdlib.h>
#include "mpispec/mpispec.h"

int n = 0;

mpispec_def(fabs_spec)
  describe("標準ライブラリfabsの")
    context("入力が0の時")
      it("同じ数値を返す")
        expect_to_equal(fabs(n), n)
      end
    end

    before_each(make_random_num)
      n = rand();
    end
    after_each(debug)
      printf("%d\n", n+1);
    end

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
