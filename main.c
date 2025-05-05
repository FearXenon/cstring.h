#include "string.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

/*#define started printf("test started: %s\n", __func__)*/
#define started ;
#define passed printf("test passed: %s\n", __func__)

static void test_str_pos() {
  started;

  assert(str_pos(str("123"), str("123")) == 0);
  assert(str_pos(str("123"), str("123")) == 0);
  assert(str_pos(str("abcde"), str("bcd")) == 1);
  assert(str_pos(str("hello world"), str("world")) == 6);
  assert(str_pos(str("banana"), str("na")) == 2);
  assert(str_pos(str("abc"), str("xyz")) == -1);
  assert(str_pos(str("abc"), str("abcd")) == -1);
  assert(str_pos(str("ababab"), str("ab")) == 0);
  assert(str_pos(str("abcabcabc"), str("cabc")) == 2);
  assert(str_pos(str("abc"), str("")) == 0);
  assert(str_pos(str(""), str("")) == 0);
  assert(str_pos(str(""), str("a")) == -1);
  assert(str_pos(str("a"), str("a")) == 0);
  assert(str_pos(str("a"), str("b")) == -1);
  assert(str_pos(str("abc"), str("c")) == 2);
  assert(str_pos(str("start middle end"), str("start")) == 0);
  assert(str_pos(str("start middle end"), str("middle")) == 6);
  assert(str_pos(str("start middle end"), str("end")) == 13);
  assert(str_pos(str("aaaaaa"), str("aaa")) == 0);
  assert(str_pos(str("hello\tworld"), str("\t")) == 5);
  assert(str_pos(str("a\nb\nc"), str("\n")) == 1);
  assert(str_pos(str("foo bar"), str(" ")) == 3);

  passed;
}

static void test_str_equals() {
  started;

  assert(str_equals(str("abc"), str("abc")) == 1);
  assert(str_equals(str("ABC"), str("abc")) == 0);
  assert(str_equals(str("abc"), str("abcd")) == 0);
  assert(str_equals(str("abcd"), str("abc")) == 0);
  assert(str_equals(str(""), str("")) == 1);
  assert(str_equals(str(" "), str("")) == 0);
  assert(str_equals(str(""), str(" ")) == 0);
  assert(str_equals(str("hello world"), str("hello world")) == 1);
  assert(str_equals(str("hello world"), str("hello  world")) ==
         0); // double space
  assert(str_equals(str("abc"), str("ABC")) == 0);
  assert(str_equals(str("123"), str("123")) == 1);
  assert(str_equals(str("123"), str("0123")) == 0);
  assert(str_equals(str("abcdef"), str("abcdef")) == 1);
  assert(str_equals(str("abc def"), str("abc def")) == 1);
  assert(str_equals(str("\t\n"), str("\t\n")) == 1);
  assert(str_equals(str("\t\n"), str("\n\t")) == 0);
  assert(str_equals(str("a"), str("a")) == 1);
  assert(str_equals(str("a"), str("b")) == 0);
  assert(str_equals(str("a"), str("aa")) == 0);
  assert(str_equals(str("🙂"), str("🙂")) ==
         1); // Unicode emoji (UTF-8 assumed)
  assert(str_equals(str("🙂"), str("🙃")) == 0); // Different emoji

  passed;
}

static void test_str_equals_ic() {
  started;

  assert(str_equals_ic(str("abc"), str("abc")) == 1);
  assert(str_equals_ic(str("ABC"), str("abc")) == 1);
  assert(str_equals_ic(str("aBcDeF"), str("AbCdEf")) == 1);
  assert(str_equals_ic(str("abc"), str("abcd")) == 0);
  assert(str_equals_ic(str("abcd"), str("abc")) == 0);
  assert(str_equals_ic(str(""), str("")) == 1);
  assert(str_equals_ic(str(" "), str("")) == 0);
  assert(str_equals_ic(str(""), str(" ")) == 0);
  assert(str_equals_ic(str("hello world"), str("HELLO WORLD")) == 1);
  assert(str_equals_ic(str("hello world"), str("hello  world")) ==
         0); // double space
  assert(str_equals_ic(str("123abc"), str("123ABC")) == 1);
  assert(str_equals_ic(str("abc123"), str("ABC123")) == 1);
  assert(str_equals_ic(str("a"), str("A")) == 1);
  assert(str_equals_ic(str("Z"), str("z")) == 1);
  assert(str_equals_ic(str("a"), str("b")) == 0);
  assert(str_equals_ic(str("a"), str("aa")) == 0);
  assert(str_equals_ic(str("abc def"), str("ABC DEF")) == 1);
  assert(str_equals_ic(str("abc def"), str("ABC  DEF")) ==
         0); // spacing matters
  assert(str_equals_ic(str("\t\n"), str("\t\n")) == 1);
  assert(str_equals_ic(str("\t\n"), str("\n\t")) == 0);
  assert(str_equals_ic(str("🙂"), str("🙂")) == 1); // Non-ASCII, should match
  assert(str_equals_ic(str("🙂"), str("🙃")) == 0); // Different emoji
  passed;
}

static void test_str_cat() {
  started;

  assert(str_equals(str_cat(str("base"), "test"), str("basetest")));
  assert(str_equals(str_cat(str(""), "test"), str("test")));
  assert(str_equals(str_cat(str(""), ""), str("")));
  assert(str_equals(
      str_cat(str("base"), "test%d%l%s%S", 69, 55555, "test", str("test")),
      str("basetest6955555testtest")));

  passed;
}

static void test_str_push() {
  started;

  string_t s = str("test");
  str_push(&s, "%S", str(""));

  assert(str_equals(s, str("test")));

  passed;
}

static void test_str_explode_debugprint(string_t *arr, uint64_t len) {
  for (int i = 0; i < len; i++) {
    printf("[%s] ", ((string_t) * (arr + i)).str);
  }
  printf("\n");
}

static void test_str_explode() {
  started;

  string_t *arr;
  uint64_t len;

  str_explode(str("test;123;abc"), str(";"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3);

  str_explode(str(";"), str(";"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 2);

  str_explode(str("123;123"), str(";"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 2);

  str_explode(str("123;123;"), str(";"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3);

  str_explode(str("123;123;"), str(""), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 1);

  str_explode(str("123;;delim;;123;;"), str(";;delim;;"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 2);

  str_explode(str("a,b,c"), str(","), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3);

  str_explode(str(",a,b,"), str(","), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 4); // ["", "a", "b", ""]

  str_explode(str(";;"), str(";"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3); // ["", "", ""]

  str_explode(str(""), str(","), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 1); // [""]

  str_explode(str("hello"), str(","), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 1); // ["hello"]

  str_explode(str("one::two::three"), str("::"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3);

  str_explode(str("split||this||string"), str("||"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3); // ["split", "this", "string"]

  str_explode(str("abc"), str("abc"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 2); // ["", ""]

  str_explode(str("x--x--x--"), str("--"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 4); // ["x", "x", "x", ""]

  str_explode(str(";;abc;;"), str(";;"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3); // ["", "abc", ""]

  str_explode(str("foo bar baz"), str(" "), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3); // ["foo", "bar", "baz"]

  str_explode(str("delimiter"), str("delimiter"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 2); // ["", ""]

  str_explode(str("repeatrepeatrepeat"), str("repeat"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 4); // ["", "", "", ""]

  str_explode(str("applebanana"), str("orange"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 1); // ["applebanana"] — no match

  passed;
}

static void test_str_implode() {
  started;

  string_t arr[] = {str("str1"), str("str2"), str("str3"),
                    str("longstring long string long string")};

  assert(str_equals(str_implode(str(""), arr, 4),
                    str("str1str2str3longstring long string long string")));

  assert(str_equals(str_implode(str(" "), arr, 4),
                    str("str1 str2 str3 longstring long string long string")));

  assert(str_equals(str_implode(str(";;;delimiter;;;"), arr, 4),
                    str("str1;;;delimiter;;;str2;;;delimiter;;;str3;;;"
                        "delimiter;;;longstring long string long string")));

  string_t arr_1[] = {str("")};

  assert(str_equals(str_implode(str(""), arr_1, 1), str("")));

  passed;
}

static void test_str_replace() {
  started;

  // Basic replacement
  assert(str_equals(str_replace(str("abc"), str("b"), str("d")), str("adc")));

  // No match
  assert(str_equals(str_replace(str("abc"), str("x"), str("d")), str("abc")));

  // Replace all instances
  assert(str_equals(str_replace(str("banana"), str("a"), str("o")),
                    str("bonono")));

  // Replace with empty string (deletion)
  assert(str_equals(str_replace(str("hello world"), str("l"), str("")),
                    str("heo word")));

  // Insert something in place of empty string (edge case: empty `from`)
  assert(str_equals(str_replace(str("abc"), str(""), str("x")), str("abc")));

  // Insert something in place of empty string (edge case: empty `from`)
  assert(str_equals(
      str_replace(str("das ist ein test mit leerzeichen"), str(" "), str("")),
      str("dasisteintestmitleerzeichen")));

  // Replace entire string
  assert(str_equals(str_replace(str("aaa"), str("aaa"), str("b")), str("b")));

  // Empty input string
  assert(str_equals(str_replace(str(""), str("a"), str("b")), str("")));

  // Replace with longer string
  assert(
      str_equals(str_replace(str("abc"), str("b"), str("xyz")), str("axyzc")));

  // Replace at beginning
  assert(str_equals(str_replace(str("abc"), str("a"), str("z")), str("zbc")));

  // Replace at end
  assert(str_equals(str_replace(str("abc"), str("c"), str("z")), str("abz")));

  // Replace with self (no change expected)
  assert(str_equals(str_replace(str("abc"), str("b"), str("b")), str("abc")));

  // Replace overlapping substrings (depends on implementation—assume
  // left-to-right, no re-evaluation)
  assert(str_equals(str_replace(str("aaaa"), str("aa"), str("b")), str("bb")));

  passed;
}

static void test_str_tolower() {
  started;

  assert(str_equals(str_tolower(str("AsDf123")), str("asdf123")));

  passed;
}

static void test_str_toupper() {
  started;

  assert(str_equals(str_toupper(str("AsDf123_+")), str("ASDF123_+")));

  passed;
}

static void test_str_substr() {
  started;

  string_t s = str_substr(str("abc"), -3, -1);

  assert(str_equals(str_substr(str("abc"), 0, 1),
                    str("a"))); // start at 0, length 1
  assert(str_equals(str_substr(str("abc"), 1, 2),
                    str("bc"))); // start at 1, length 2
  assert(str_equals(str_substr(str("abc"), -2, 2),
                    str("bc"))); // start 2 from end, length 2
  assert(str_equals(str_substr(str("abc"), -3, 2),
                    str("ab"))); // start 3 from end, length 2
  assert(str_equals(str_substr(str("abc"), -3, -1),
                    str("ab"))); // start at -3, go to 1 before end
  assert(str_equals(str_substr(str("abc"), 0, -1),
                    str("ab"))); // start at 0, exclude last char
  assert(str_equals(str_substr(str("abcdef"), 2, -2),
                    str("cd"))); // from index 2 to 2 before end
  assert(str_equals(str_substr(str("abcdef"), -4, -1),
                    str("cde"))); // from 4th-last to 1 before end
  assert(str_equals(str_substr(str("abc"), 0, 3), str("abc"))); // whole string
  assert(str_equals(str_substr(str("abc"), 0, 5),
                    str("abc"))); // too long length, clamp
  assert(str_equals(str_substr(str("abc"), -5, 2),
                    str("ab"))); // start before start, clamp
  assert(
      str_equals(str_substr(str("abc"), 3, 2), str(""))); // start at end, empty
  assert(
      str_equals(str_substr(str("abc"), 1, 0), str(""))); // zero length, empty

  passed;
}

static void test_str_trim() {
  started;

  assert(str_equals(str_trim(str("   abc  ")), str("abc")));
  assert(str_equals(str_trim(str("abc  ")), str("abc")));
  assert(str_equals(str_trim(str("   abc")), str("abc")));
  assert(str_equals(str_trim(str("abc")), str("abc")));

  passed;
}

static void test_str_rtrim() {
  started;

  assert(str_equals(str_rtrim(str("   abc  ")), str("   abc")));
  assert(str_equals(str_rtrim(str("abc  ")), str("abc")));
  assert(str_equals(str_rtrim(str("   abc")), str("   abc")));
  assert(str_equals(str_rtrim(str("abc")), str("abc")));

  passed;
};

static void test_str_ltrim() {
  started;

  assert(str_equals(str_ltrim(str("   abc  ")), str("abc  ")));
  assert(str_equals(str_ltrim(str("abc  ")), str("abc  ")));
  assert(str_equals(str_ltrim(str("   abc")), str("abc")));
  assert(str_equals(str_ltrim(str("abc")), str("abc")));

  passed;
}

static void test_str_rev() {
  started;

  assert(str_equals(str_rev(str("test")), str("tset")));
  assert(str_equals(str_rev(str("test string with spaces ")),
                    str(" secaps htiw gnirts tset")));

  passed;
}

int main() {

  test_str_equals();
  test_str_equals_ic();
  test_str_pos();
  test_str_cat();
  test_str_push();
  test_str_explode();
  test_str_tolower();
  test_str_toupper();
  test_str_replace();
  test_str_implode();
  test_str_substr();
  test_str_trim();
  test_str_rtrim();
  test_str_ltrim();
  test_str_rev();

  string_t s = str("   abc   ");
  strr_trim(&s);

  return puts("OK") < 0;
}
