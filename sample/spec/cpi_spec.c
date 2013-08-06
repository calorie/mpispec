#include "mpispec/mpispec.h"
#include "../src/cpi.h"

/* Define tests */
mpispec_def( cpi_spec )

  describe( "cpi関数は" )

    context( "rank0なら" )
      it( "piを返す" )
        expect_to_equal_double( cpi(), 3.1415926535, 0.001 );
      end
    end

  end

end
