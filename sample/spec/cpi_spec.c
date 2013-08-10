#include "mpispec/mpispec.h"
#include "../src/cpi.h"

mpispec_def( cpi_spec )

  describe( "cpi関数は" )

    rank( 0 )
      context( "rank0なら" )

          it( "piを返す" )
            expect_to_equal_double( cpi(), 3.1415926535, 0.001 );
          end

      end
    end

    ex_rank( 0 )
      context( "rank0以外なら" )

        it( "0を返す" )
          expect_to_equal_double( cpi(), 0.0, 0.001 );
        end

      end
    end

  end

end
