#include <math.h>
#include "mpispec/mpispec.h"

/* Define tests */
int a=0;
mpispec_def(fabs_spec)

  describe("標準ライブラリfabsの")
    before(fabs)
      a++;
      printf("fabs %d\n", a);
    end_before(fabs)

    context("入力が正の数の時")
      it("同じ数値を返す")
        expect_to_equal( fabs(1.0), 1.0)
        expect_to_equal( fabs(0.0), 0.0)
        expect_to_equal( fabs(6.7), 6.7)
      end
    end

    before(fabs2)
      a++;
      printf("fabs2 %d\n", a);
    end_before(fabs2)
    context("入力が負の数の時")
      it("正の数を返す")
        expect_to_equal( fabs(-1.1), 1.1)
        expect_to_equal( fabs(-6.7), 6.7)
      end
    end

  end

end_def
