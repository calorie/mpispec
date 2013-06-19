#include <math.h>
#include "mpispec/mpispec.h"
#include "mpispec/cspec_output_verbose.h"

/* Define tests */
mpispec_def(fabs_spec)

  if(MPIUT_RANK == 0) {
    describe(fabs, "標準ライブラリfabsの")

      context("入力が正の数の時")
        it("同じ数値を返す")
          expect_to_equal( fabs(1.0), 1.0)
          expect_to_equal( fabs(0.0), 0.0)
          expect_to_equal( fabs(6.7), 6.7)
        end_it
      end

      context("入力が負の数の時")
        it("正の数を返す")
          expect_to_equal( fabs(-1.1), 1.0)
          expect_to_equal( fabs(-6.7), 6.7)
        end_it
      end

    end_describe
    describe(fab, "標準ライブラリfabsの")

      context("入力が正の数の時")
        it("同じ数値を返す")
          expect_to_equal( fabs(1.0), 1.0)
          expect_to_equal( fabs(0.0), 0.0)
          expect_to_equal( fabs(6.7), 6.7)
        end_it
      end

    end_describe
    /* CSpec_Run( description( fabs ), CSpec_NewOutputUnit() ); */
  }
  else if(MPIUT_RANK == 1) {
    describe(fabs, "標準ライブラリfabsの")

      context("入力が負の数の時")
        it("正の数を返す")
          expect_to_equal( fabs(-1.1), 1.0)
          expect_to_equal( fabs(-6.7), 6.7)
        end_it
      end

    end_describe
  }
  else {
  }

end_def
