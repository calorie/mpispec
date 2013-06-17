/* This sample file will describe the 2 following functions:
 *  . fabs
 *  . strcmp
*/

#include <stdio.h>
#include <string.h>

#include "mpispec/cspec_private.h"
#include "mpispec/cspec.h"
#include "mpispec/cspec_output_header.h"
#include "mpispec/cspec_output_verbose.h"
#include "mpispec/cspec_output_unit.h"


/* Here is the description of strcmp */
describe(strcmp, "int strcmp ( const char * str1, const char * str2 )")

  it( "returns 0 only when strings are equal" )
    should_equal( strcmp("hello", "hello"), 0)
    should_not_be_null( strcmp("hello", "world") )
  end_it

  it( "returns a negative integer when str1 is less than str2"  )
    should_be_true( strcmp("hello", "world") < 0 )
    should_be_true( strcmp("0123", "1321431") < 0 )
  end_it

  it( "returns a positive integer if str1 is greater than str2"  )
    should_be_true( strcmp("yellow", "world") > 0 )
    should_be_true( strcmp("9", "789") > 0 )
  end_it

end_describe

/* Here is the declaration of fabs description defined in an other file (fabs_desc.c in this sample)*/
define_description(fabs)

int main()
{
  /* Run strcmp description with the header output => it will only output all the description titles,
     whatever the result of the test
  */
  /* CSpec_Run( description( strcmp ), CSpec_NewOutputHeader() ); */

  /* Run the description with verbose output => as much information as possible is output:
     - descriptions
    - tests
    - tests results
  */
  CSpec_Run( description( fabs ), CSpec_NewOutputVerbose() );

  return 0;
}

