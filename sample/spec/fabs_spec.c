#include <math.h>
#include "mpispec/mpispec.h"
#include "mpispec/cspec_output_verbose.h"

/* Define tests */
mpispec_def(fabs_spec)

  describe(fabs, "標準ライブラリfabsの")

    context("入力が正の数の時")
      it("同じ数値を返す")
        expect_to_equal( fabs(1.0), 1.0)
        expect_to_equal( fabs(0.0), 0.0)
        expect_to_equal( fabs(6.7), 6.7)
      end
    end

    context("入力が負の数の時")
      it("正の数を返す")
        expect_to_equal( fabs(-1.1), 1.1)
        expect_to_equal( fabs(-6.7), 6.7)
      end
    end

  end

end_def
