#include <math.h>
#include "mpispec/mpispec.h"

int a=0;

before(fabs)
  a++;
end_before

before(fabs2)
  a++;
end_before

mpispec_def(fabs_spec)

  describe("標準ライブラリfabsの")

    context("入力が正の数の時")
      run_before(fabs)

      it("同じ数値を返す")
        expect_to_equal( fabs(1.0), 1.0)
        expect_to_equal( fabs(0.0), 0.0)
        expect_to_equal( fabs(6.7), 6.7)
      end
    end_context

    context("入力が負の数の時")
      run_before(fabs2)

      it("正の数を返す")
        expect_to_equal( fabs(-1.1), 1.1)
        expect_to_equal( fabs(-6.7), 6.7)
      end
    end_context

  end_describe

end_def
