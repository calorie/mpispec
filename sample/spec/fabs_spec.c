#include <math.h>
#include <stdlib.h>
#include "mpispec/mpispec.h"

int a = 0;

mpispec_def(fabs_spec)

  describe("標準ライブラリfabsの")

    context("入力が0の時")

      it("同じ数値を返す")
        expect_to_equal( fabs(a), a)
      end

    end_context

    before(fabs)

      a = rand() % 11;

    end_before

    context("入力が正の数の時")

      it("同じ数値を返す")
        expect_to_equal( fabs(a), a)
      end

    end_context

    context("入力が負の数の時")

      it("正の数を返す")
        expect_to_equal( fabs(-a), a)
      end

    end_context

  end_describe

end_def
