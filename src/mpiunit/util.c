
/* Time-stamp: <2007-11-14 22:37:47 shinya> */

/*
 *  MPIUnit - A Unit Testing Framework for MPI Programs, based on CUnit.
 *
 *  Author: Shinya Abe
 *  License: LGPL
 */

/*  
 *  Original:
 *
 *  CUnit - A Unit testing framework library for C.
 *  Copyright (C) 2001            Anil Kumar
 *  Copyright (C) 2004,2005,2006  Anil Kumar, Jerry St.Clair
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 *  Generic (internal) utility functions used across CUnit.
 *  These were originally distributed across the other CUnit
 *  source files, but were consolidated here for consistency.
 *
 *  13/Oct/2001   Initial implementation (AK)
 *
 *  26/Jul/2003   Added a function to convert a string containing special
 *                characters into escaped character for XML/HTML usage. (AK)
 *
 *  16-Jul-2004   New interface, doxygen comments. (JDS)
 */

/** @file
 *  Utility functions (implementation).
 */
/** @addtogroup Framework
 @{
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#include "mpiut_base.h"
#include "testdb.h"
#include "util.h"

static const struct {
  char special_char;
  char* replacement;
} bindings [] = {
    {'&', "&amp;"},
    {'>', "&gt;"},
    {'<', "&lt;"}
};

static int 
get_index(char ch)
{
  int length = sizeof(bindings)/sizeof(bindings[0]);
  int counter;

  for (counter = 0; counter < length && bindings[counter].special_char != ch; ++counter) {
    ;
  }

  return (counter < length ? counter : -1);
}

int 
CU_translate_special_characters(const char* szSrc, char* szDest, size_t maxlen)
{
  int count = 0;
  size_t src = 0;
  size_t dest = 0;
  size_t length = strlen(szSrc);
  int conv_index;

  assert(NULL != szSrc);
  assert(NULL != szDest);

  memset(szDest, 0, maxlen);
  while ((dest < maxlen) && (src < length)) {

    if ((-1 != (conv_index = get_index(szSrc[src]))) &&
        ((dest + (int)strlen(bindings[conv_index].replacement)) <= maxlen)) {
      strcat(szDest, bindings[conv_index].replacement);
      dest += (int)strlen(bindings[conv_index].replacement);
      ++count;
    } else {
      szDest[dest++] = szSrc[src];
    }

    ++src;
  }

  return count;
}

int 
CU_compare_strings(const char* szSrc, const char* szDest)
{
  assert(NULL != szSrc);
  assert(NULL != szDest);

	while (('\0' != *szSrc) && ('\0' != *szDest) && (toupper(*szSrc) == toupper(*szDest))) {
		szSrc++;
		szDest++;
	}

	return (int)(*szSrc - *szDest);
}

void 
CU_trim(char* szString)
{
  CU_trim_left(szString);
  CU_trim_right(szString);
}

void 
CU_trim_left(char* szString)
{
  int nOffset = 0;
  char* szSrc = szString;
  char* szDest = szString;

  assert(NULL != szString);

  for (; '\0' != *szSrc; szSrc++, nOffset++) {
    if (!isspace(*szSrc)) {
      break;
    }
  }

  for(; (0 != nOffset) && ('\0' != (*szDest = *szSrc)); szSrc++, szDest++) {
    ;
  }
}

void 
CU_trim_right(char* szString)
{
  size_t nLength;
  char* szSrc = szString;

  assert(NULL != szString);
  nLength = strlen(szString);

  for (; (0 != nLength) && isspace(*(szSrc + nLength - 1)); nLength--) {
    ;
  }

  *(szSrc + nLength) = '\0';
}

#ifdef CUNIT_BUILD_TESTS
#include "test_cunit.h"

#define MAX_LEN 100

static void 
test_CU_translate_special_characters(void)
{
  int nchars;
  char dest[MAX_LEN];

  strcpy(dest, "random initialized string");
  nchars = CU_translate_special_characters("", dest, MAX_LEN);
  TEST(0 == nchars);
  TEST(!strcmp(dest, ""));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("#", dest, 1);
  TEST(0 == nchars);
  TEST(!strcmp(dest, "#"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("&", dest, 1);
  TEST(0 == nchars);
  TEST(!strcmp(dest, "&"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("&", dest, 4);
  TEST(0 == nchars);
  TEST(!strcmp(dest, "&"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("&", dest, 5);
  TEST(1 == nchars);
  TEST(!strcmp(dest, "&amp;"));

  strcpy(dest, "random initialized string");
  nchars = CU_translate_special_characters("some <<string & another>", dest, 0);
  TEST(0 == nchars);
  TEST(!strcmp(dest, "random initialized string"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 1);
  TEST(0 == nchars);
  TEST(!strcmp(dest, "s"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 6);
  TEST(0 == nchars);
  TEST(!strcmp(dest, "some <"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 9);
  TEST(1 == nchars);
  TEST(!strcmp(dest, "some &lt;"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 10);
  TEST(1 == nchars);
  TEST(!strcmp(dest, "some &lt;<"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 13);
  TEST(2 == nchars);
  TEST(!strcmp(dest, "some &lt;&lt;"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 14);
  TEST(2 == nchars);
  TEST(!strcmp(dest, "some &lt;&lt;s"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 21);
  TEST(2 == nchars);
  TEST(!strcmp(dest, "some &lt;&lt;string &"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 22);
  TEST(2 == nchars);
  TEST(!strcmp(dest, "some &lt;&lt;string & "));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 23);
  TEST(2 == nchars);
  TEST(!strcmp(dest, "some &lt;&lt;string & a"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 24);
  TEST(2 == nchars);
  TEST(!strcmp(dest, "some &lt;&lt;string & an"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 25);
  TEST(3 == nchars);
  TEST(!strcmp(dest, "some &lt;&lt;string &amp;"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 26);
  TEST(3 == nchars);
  TEST(!strcmp(dest, "some &lt;&lt;string &amp; "));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, 37);
  TEST(4 == nchars);
  TEST(!strcmp(dest, "some &lt;&lt;string &amp; another&gt;"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some <<string & another>", dest, MAX_LEN);
  TEST(4 == nchars);
  TEST(!strcmp(dest, "some &lt;&lt;string &amp; another&gt;"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("some string or another", dest, MAX_LEN);
  TEST(0 == nchars);
  TEST(!strcmp(dest, "some string or another"));

  memset(dest, 0, MAX_LEN);
  nchars = CU_translate_special_characters("<><><<>>&&&", dest, MAX_LEN);
  TEST(11 == nchars);
  TEST(!strcmp(dest, "&lt;&gt;&lt;&gt;&lt;&lt;&gt;&gt;&amp;&amp;&amp;"));
}

static void 
test_CU_compare_strings(void)
{
  TEST(0 == CU_compare_strings("",""));
  TEST(0 == CU_compare_strings("@","@"));
  TEST(0 == CU_compare_strings("D","d"));
  TEST(0 == CU_compare_strings("s1","s1"));
  TEST(0 == CU_compare_strings("s1","S1"));
  TEST(0 != CU_compare_strings("s1","s12"));
  TEST(0 == CU_compare_strings("this is string 1","tHIS iS sTRING 1"));
  TEST(0 == CU_compare_strings("i have \t a tab!","I have \t a tab!"));
  TEST(0 != CU_compare_strings("not the same"," not the same"));
}

static void 
test_CU_trim(void)
{
  char string[MAX_LEN];

  strcpy(string, "");
  CU_trim(string);
  TEST(!strcmp("", string));

  strcpy(string, " ");
  CU_trim(string);
  TEST(!strcmp("", string));

  strcpy(string, "    ");
  CU_trim(string);
  TEST(!strcmp("", string));

  strcpy(string, " b");
  CU_trim(string);
  TEST(!strcmp("b", string));

  strcpy(string, "  B");
  CU_trim(string);
  TEST(!strcmp("B", string));

  strcpy(string, "s ");
  CU_trim(string);
  TEST(!strcmp("s", string));

  strcpy(string, "S  ");
  CU_trim(string);
  TEST(!strcmp("S", string));

  strcpy(string, "  5   ");
  CU_trim(string);
  TEST(!strcmp("5", string));

  strcpy(string, "~ & ^ ( ^  ");
  CU_trim(string);
  TEST(!strcmp("~ & ^ ( ^", string));

  strcpy(string, "  ~ & ^ ( ^");
  CU_trim(string);
  TEST(!strcmp("~ & ^ ( ^", string));

  strcpy(string, "  ~ & ^ ( ^  ");
  CU_trim(string);
  TEST(!strcmp("~ & ^ ( ^", string));
}

static void 
test_CU_trim_left(void)
{
  char string[MAX_LEN];

  strcpy(string, "");
  CU_trim_left(string);
  TEST(!strcmp("", string));

  strcpy(string, " ");
  CU_trim_left(string);
  TEST(!strcmp("", string));

  strcpy(string, "    ");
  CU_trim_left(string);
  TEST(!strcmp("", string));

  strcpy(string, " b");
  CU_trim_left(string);
  TEST(!strcmp("b", string));

  strcpy(string, "  B");
  CU_trim_left(string);
  TEST(!strcmp("B", string));

  strcpy(string, "s ");
  CU_trim_left(string);
  TEST(!strcmp("s ", string));

  strcpy(string, "S  ");
  CU_trim_left(string);
  TEST(!strcmp("S  ", string));

  strcpy(string, "  5   ");
  CU_trim_left(string);
  TEST(!strcmp("5   ", string));

  strcpy(string, "~ & ^ ( ^  ");
  CU_trim_left(string);
  TEST(!strcmp("~ & ^ ( ^  ", string));

  strcpy(string, "  ~ & ^ ( ^");
  CU_trim_left(string);
  TEST(!strcmp("~ & ^ ( ^", string));

  strcpy(string, "  ~ & ^ ( ^  ");
  CU_trim_left(string);
  TEST(!strcmp("~ & ^ ( ^  ", string));
}

static void 
test_CU_trim_right(void)
{
  char string[MAX_LEN];

  strcpy(string, "");
  CU_trim_right(string);
  TEST(!strcmp("", string));

  strcpy(string, " ");
  CU_trim_right(string);
  TEST(!strcmp("", string));

  strcpy(string, "    ");
  CU_trim_right(string);
  TEST(!strcmp("", string));

  strcpy(string, " b");
  CU_trim_right(string);
  TEST(!strcmp(" b", string));

  strcpy(string, "  B");
  CU_trim_right(string);
  TEST(!strcmp("  B", string));

  strcpy(string, "s ");
  CU_trim_right(string);
  TEST(!strcmp("s", string));

  strcpy(string, "S  ");
  CU_trim_right(string);
  TEST(!strcmp("S", string));

  strcpy(string, "  5   ");
  CU_trim_right(string);
  TEST(!strcmp("  5", string));

  strcpy(string, "~ & ^ ( ^  ");
  CU_trim_right(string);
  TEST(!strcmp("~ & ^ ( ^", string));

  strcpy(string, "  ~ & ^ ( ^");
  CU_trim_right(string);
  TEST(!strcmp("  ~ & ^ ( ^", string));

  strcpy(string, "  ~ & ^ ( ^  ");
  CU_trim_right(string);
  TEST(!strcmp("  ~ & ^ ( ^", string));
}

void 
test_cunit_Util(void)
{

  test_cunit_start_tests("Util.c");

  test_CU_translate_special_characters();
  test_CU_compare_strings();
  test_CU_trim();
  test_CU_trim_left();
  test_CU_trim_right();

  test_cunit_end_tests();
}

#endif
