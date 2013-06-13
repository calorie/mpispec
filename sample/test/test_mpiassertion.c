
/* Time-stamp: <2007-11-14 22:31:35 shinya> */

#include "mpiunit/mpiunit.h"

/* Define tests */
MPIUT_DEF(test_mpiassertion)
{
  if(MPIUT_RANK == 0) {
    MPIUT_PASS("pass");
    /* MPIUT_FAIL("fail"); */
    MPIUT_ASSERT_TRUE(1);
    MPIUT_ASSERT_FALSE(0);
    MPIUT_ASSERT_INT_EQUAL(3, 3);
    MPIUT_ASSERT_INT_NOT_EQUAL(1, 3);
    MPIUT_ASSERT_STRING_EQUAL("abc", "abc");
    MPIUT_ASSERT_STRING_NOT_EQUAL("abc", "def");
    MPIUT_ASSERT_DOUBLE_EQUAL(1.11, 1.12, 0.5);
    MPIUT_ASSERT_DOUBLE_NOT_EQUAL(1.11, 1.12, 0.005);
  }
}
