#include <stdio.h>
#include "mpispec/mpispec.h"

mpispec_def( strcmp_spec )

  describe( "標準ライブラリのstrcmpの" )

    context( "入力が一致するとき" )

      it( "0を返す" )
        expect_to_equal( strcmp("hello", "hello"), 0 )
      end

    end

    context( "入力が一致しないとき" )

      it( "NULL以外を返す" )
        expect_not_to_be_null( strcmp("hello", "world") )
      end

      context( "第1引数が第2より小さい時" )

        it( "負の数を返す" )
          expect_to_be_true( strcmp("hello", "world") < 0 )
          expect_to_be_true( strcmp("0123", "1321431") < 0 )
        end

      end

      context( "第1引数が第2より大きい時" )

        it( "正の数を返す" )
          expect_to_be_true( strcmp("yellow", "world") > 0 )
          expect_to_be_true( strcmp("9", "789") > 0 )
        end

      end

    end

  end

end
