#include "cstring.h"
#include "encoding.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define started printf("test started: %s\n", __func__)
#define done printf("test passed: %s\n", __func__)

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

  done;
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

  done;
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
  done;
}

static void test_str_cat() {
  started;

  string_t s = str_cat(str("base"), "test");
  assert(str_equals(s, str("basetest")));
  str_free(&s);
  string_t s1 = str_cat(str(""), "test");
  assert(str_equals(s1, str("test")));
  str_free(&s1);
  string_t s3 = str_cat(str(""), "");
  assert(str_equals(s3, str("")));
  str_free(&s3);
  string_t s4 =
      str_cat(str("base"), "test%d%l%s%S", 69, 55555, "test", str("test"));
  assert(str_equals(s4, str("basetest6955555testtest")));
  str_free(&s4);

  s = str_cat(str("base"), "%c", 'a');
  assert(str_equals(s, str("base")));
  str_free(&s);

  done;
}

static void test_str_prepend() {
  started;

  string_t s = str_prepend(str("base"), "test");
  assert(str_equals(s, str("testbase")));
  str_free(&s);
  string_t s1 = str_prepend(str(""), "test");
  assert(str_equals(s1, str("test")));
  str_free(&s1);
  string_t s3 = str_prepend(str(""), "");
  assert(str_equals(s3, str("")));
  str_free(&s3);
  string_t s4 =
      str_prepend(str("base"), "test%d%l%s%S", 69, 55555, "test", str("test"));
  assert(str_equals(s4, str("test6955555testtestbase")));
  str_free(&s4);

  done;
}

static void test_str_explode_debugprint(string_t *arr, uint64_t len) {
  for (int i = 0; i < len; i++) {
    printf("[%s] ", ((string_t) * (arr + i)).str);
  }
  printf("\n");
}

static void test_str_explode_autofree(string_t *arr, size_t len) {
  for (int i = 0; i < len; i++) {
    str_free(arr + i);
  }
  free(arr);
}

static void test_str_explode() {
  started;

  string_t *arr;
  uint64_t len;

  str_explode(str("test;123;abc"), str(";"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3);
  test_str_explode_autofree(arr, len);

  str_explode(str(";"), str(";"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 2);
  test_str_explode_autofree(arr, len);

  str_explode(str("123;123"), str(";"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 2);
  test_str_explode_autofree(arr, len);

  str_explode(str("123;123;"), str(";"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3);
  test_str_explode_autofree(arr, len);

  str_explode(str("123;123;"), str(""), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 1);
  test_str_explode_autofree(arr, len);

  str_explode(str("123;;delim;;123;;"), str(";;delim;;"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 2);
  test_str_explode_autofree(arr, len);

  str_explode(str("a,b,c"), str(","), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3);
  test_str_explode_autofree(arr, len);

  str_explode(str(",a,b,"), str(","), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 4); // ["", "a", "b", ""]
  test_str_explode_autofree(arr, len);

  str_explode(str(";;"), str(";"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3); // ["", "", ""]
  test_str_explode_autofree(arr, len);

  str_explode(str(""), str(","), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 1); // [""]
  test_str_explode_autofree(arr, len);

  str_explode(str("hello"), str(","), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 1); // ["hello"]
  test_str_explode_autofree(arr, len);

  str_explode(str("one::two::three"), str("::"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3);
  test_str_explode_autofree(arr, len);

  str_explode(str("split||this||string"), str("||"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3); // ["split", "this", "string"]
  test_str_explode_autofree(arr, len);

  str_explode(str("abc"), str("abc"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 2); // ["", ""]
  test_str_explode_autofree(arr, len);

  str_explode(str("x--x--x--"), str("--"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 4); // ["x", "x", "x", ""]
  test_str_explode_autofree(arr, len);

  str_explode(str(";;abc;;"), str(";;"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3); // ["", "abc", ""]
  test_str_explode_autofree(arr, len);

  str_explode(str("foo bar baz"), str(" "), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 3); // ["foo", "bar", "baz"]
  test_str_explode_autofree(arr, len);

  str_explode(str("delimiter"), str("delimiter"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 2); // ["", ""]
  test_str_explode_autofree(arr, len);

  str_explode(str("repeatrepeatrepeat"), str("repeat"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 4); // ["", "", "", ""]
  test_str_explode_autofree(arr, len);

  str_explode(str("applebanana"), str("orange"), &arr, &len);
  test_str_explode_debugprint(arr, len);
  assert(len == 1); // ["applebanana"] — no match
  test_str_explode_autofree(arr, len);

  done;
}

static void test_str_implode() {
  started;

  string_t s;

  string_t arr[] = {str("str1"), str("str2"), str("str3"),
                    str("longstring long string long string")};
  s = str_implode(str(""), arr, 4);
  assert(str_equals(s, str("str1str2str3longstring long string long string")));
  str_free(&s);
  s = str_implode(str(" "), arr, 4);
  assert(
      str_equals(s, str("str1 str2 str3 longstring long string long string")));
  str_free(&s);
  s = str_implode(str(";;;delimiter;;;"), arr, 4);
  assert(str_equals(s, str("str1;;;delimiter;;;str2;;;delimiter;;;str3;;;"
                           "delimiter;;;longstring long string long string")));
  str_free(&s);
  string_t arr_1[] = {str("")};
  s = str_implode(str(""), arr_1, 1);
  assert(str_equals(s, str("")));
  str_free(&s);
  done;
}

static void test_str_replace() {
  started;

  string_t s;

  // Basic replacement
  s = str_replace(str("abc"), str("b"), str("d"));
  assert(str_equals(s, str("adc")));
  str_free(&s);

  // No match
  s = str_replace(str("abc"), str("x"), str("d"));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  // Replace all instances
  s = str_replace(str("banana"), str("a"), str("o"));
  assert(str_equals(s, str("bonono")));
  str_free(&s);

  // Replace with empty string (deletion)
  s = str_replace(str("hello world"), str("l"), str(""));
  assert(str_equals(s, str("heo word")));
  str_free(&s);

  // Insert something in place of empty string (edge case: empty `from`)
  s = str_replace(str("abc"), str(""), str("x"));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  // Replace spaces with nothing
  s = str_replace(str("spaces test"), str(" "), str(""));
  assert(str_equals(s, str("spacestest")));
  str_free(&s);

  // Replace entire string
  s = str_replace(str("aaa"), str("aaa"), str("b"));
  assert(str_equals(s, str("b")));
  str_free(&s);

  // Empty input string
  s = str_replace(str(""), str("a"), str("b"));
  assert(str_equals(s, str("")));
  str_free(&s);

  // Replace with longer string
  s = str_replace(str("abc"), str("b"), str("xyz"));
  assert(str_equals(s, str("axyzc")));
  str_free(&s);

  // Replace at beginning
  s = str_replace(str("abc"), str("a"), str("z"));
  assert(str_equals(s, str("zbc")));
  str_free(&s);

  // Replace at end
  s = str_replace(str("abc"), str("c"), str("z"));
  assert(str_equals(s, str("abz")));
  str_free(&s);

  // Replace with self (no change expected)
  s = str_replace(str("abc"), str("b"), str("b"));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  // Replace overlapping substrings
  s = str_replace(str("aaaa"), str("aa"), str("b"));
  assert(str_equals(s, str("bb")));
  str_free(&s);

  // test strr_replace

  // Basic replacement
  s = str("abc");
  strr_replace(&s, str("b"), str("d"));
  assert(str_equals(s, str("adc")));
  str_free(&s);

  // No match
  s = str("abc");
  strr_replace(&s, str("x"), str("d"));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  // Replace all instances
  s = str("banana");
  strr_replace(&s, str("a"), str("o"));
  assert(str_equals(s, str("bonono")));
  str_free(&s);

  // Replace with empty string (deletion)
  s = str("hello world");
  strr_replace(&s, str("l"), str(""));
  assert(str_equals(s, str("heo word")));
  str_free(&s);

  // Insert something in place of empty string (edge case: empty `from`)
  s = str("abc");
  strr_replace(&s, str(""), str("x"));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  // Replace spaces with nothing
  s = str("spaces test");
  strr_replace(&s, str(" "), str(""));
  assert(str_equals(s, str("spacestest")));
  str_free(&s);

  // Replace entire string
  s = str("aaa");
  strr_replace(&s, str("aaa"), str("b"));
  assert(str_equals(s, str("b")));
  str_free(&s);

  // Empty input string
  s = str("");
  strr_replace(&s, str("a"), str("b"));
  assert(str_equals(s, str("")));
  str_free(&s);

  // Replace with longer string
  s = str("abc");
  strr_replace(&s, str("b"), str("xyz"));
  assert(str_equals(s, str("axyzc")));
  str_free(&s);

  // Replace at beginning
  s = str("abc");
  strr_replace(&s, str("a"), str("z"));
  assert(str_equals(s, str("zbc")));
  str_free(&s);

  // Replace at end
  s = str("abc");
  strr_replace(&s, str("c"), str("z"));
  assert(str_equals(s, str("abz")));
  str_free(&s);

  // Replace with self (no change expected)
  s = str("abc");
  strr_replace(&s, str("b"), str("b"));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  // Replace overlapping substrings
  s = str("aaaa");
  strr_replace(&s, str("aa"), str("b"));
  assert(str_equals(s, str("bb")));
  str_free(&s);

  done;
}

static void test_str_tolower() {
  started;
  str_auto s = str_tolower(str("AsDf123"));
  assert(str_equals(s, str("asdf123")));

  done;
}

static void test_str_toupper() {
  started;
  str_auto s = str_toupper(str("AsDf123_+"));
  assert(str_equals(s, str("ASDF123_+")));

  done;
}

static void test_str_substr() {
  started;

  string_t s;

  s = str_substr(str("abc"), -3, -1);
  str_free(&s);

  s = str_substr(str("abc"), 0, 1);
  assert(str_equals(s, str("a")));
  str_free(&s);

  s = str_substr(str("abc"), 1, 2);
  assert(str_equals(s, str("bc")));
  str_free(&s);

  s = str_substr(str("abc"), -2, 2);
  assert(str_equals(s, str("bc")));
  str_free(&s);

  s = str_substr(str("abc"), -3, 2);
  assert(str_equals(s, str("ab")));
  str_free(&s);

  s = str_substr(str("abc"), -3, -1);
  assert(str_equals(s, str("ab")));
  str_free(&s);

  s = str_substr(str("abc"), 0, -1);
  assert(str_equals(s, str("ab")));
  str_free(&s);

  s = str_substr(str("abcdef"), 2, -2);
  assert(str_equals(s, str("cd")));
  str_free(&s);

  s = str_substr(str("abcdef"), -4, -1);
  assert(str_equals(s, str("cde")));
  str_free(&s);

  s = str_substr(str("abc"), 0, 3);
  assert(str_equals(s, str("abc")));
  str_free(&s);

  s = str_substr(str("abc"), 0, 5);
  assert(str_equals(s, str("abc")));
  str_free(&s);

  s = str_substr(str("abc"), -5, 2);
  assert(str_equals(s, str("ab")));
  str_free(&s);

  s = str_substr(str("abc"), 3, 2);
  assert(str_equals(s, str("")));
  str_free(&s);

  s = str_substr(str("abc"), 1, 0);
  assert(str_equals(s, str("")));
  str_free(&s);

  done;
}

static void test_str_trim() {
  started;

  string_t s;

  s = str_trim(str("   abc  "));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  s = str_trim(str("abc  "));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  s = str_trim(str("   abc"));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  s = str_trim(str("abc"));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  s = str_trim(str("       "));
  assert(str_equals(s, str("")));
  str_free(&s);

  done;
}

static void test_str_rtrim() {
  started;

  string_t s;

  s = str_rtrim(str("   abc  "));
  assert(str_equals(s, str("   abc")));
  str_free(&s);

  s = str_rtrim(str("abc  "));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  s = str_rtrim(str("   abc"));
  assert(str_equals(s, str("   abc")));
  str_free(&s);

  s = str_rtrim(str("abc"));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  done;
}

static void test_str_ltrim() {
  started;

  string_t s;

  s = str_ltrim(str("   abc  "));
  assert(str_equals(s, str("abc  ")));
  str_free(&s);

  s = str_ltrim(str("abc  "));
  assert(str_equals(s, str("abc  ")));
  str_free(&s);

  s = str_ltrim(str("   abc"));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  s = str_ltrim(str("abc"));
  assert(str_equals(s, str("abc")));
  str_free(&s);

  done;
}

static void test_str_rev() {
  started;

  string_t s;

  s = str_rev(str("test"));
  assert(str_equals(s, str("tset")));
  str_free(&s);
  s = str_rev(str("test string with spaces "));
  assert(str_equals(s, str(" secaps htiw gnirts tset")));
  str_free(&s);

  done;
}

static void test_str_lpad() {
  started;

  string_t s;

  s = str_lpad(str("test"), 'c', 10);
  assert(str_equals(s, str("cccccctest")));
  str_free(&s);
  s = str_lpad(str("test string with spaces "), ' ', 10);
  assert(str_equals(s, str("test string with spaces ")));
  str_free(&s);

  done;
}

static void test_str_rpad() {
  started;

  string_t s;

  s = str_rpad(str("test"), 'c', 10);
  assert(str_equals(s, str("testcccccc")));
  str_free(&s);
  s = str_rpad(str("test string with spaces "), ' ', 10);
  assert(str_equals(s, str("test string with spaces ")));
  str_free(&s);

  done;
}

static void test_str_token() {
  started;

  int i = 0;
  tokenizer_t t = str_token_init(str(";;;;1;2;;;3;sdf4;5;;;;"), str(";"));
  string_t s = str_null;
  while (str_token_next(&t, &s)) {
    i++;    
  }

  assert(i == 5);

  done;
}

static void test_mem() {
  started;

  string_t s = str("string");
  str_free(&s);

  // dont free stack allocated strings
  assert(strcmp(s.str, "string") == 0);

  char *tmp = malloc(str_len(s) + 1);

  memcpy(tmp, s.str, s.len);

  string_t s1 = str_acquire_s(tmp, s.len);
  str_free(&s1);

  // free acquired strings
  assert(s1.str == 0);

  string_t s2 = str("string");
  string_t s3;
  str_borrow(&s3, s2.str + 4, 2);

  assert(strcmp(s3.str, "ng") == 0);

  tmp = malloc(5);
  memset(tmp, 65, 4); 

  *(tmp + 4) = 0;

  string_t heapstring = strh(tmp);

  str_free(&heapstring);

  assert(heapstring.str == 0);

  tmp = malloc(5);
  memset(tmp, 65, 4); 
  *(tmp + 4) = 0;

  /*string_t insert_test = str("abcdef");*/
  /*str_mem_insert(&insert_test, 5, tmp, 2);*/
  /**/
  /*printf("%s\n", insert_test.str);*/
  /**/
  /*memset(tmp, 66, 4); */
  /*str_mem_replace(&insert_test, 8, 2, tmp, 1); */
  /**/
  /*printf("%s\n", insert_test.str);*/

  /*str_free(&insert_test);*/
  free(tmp);

  

  done;
}

static void test_encoding() {
  str_auto teststr = str("üöüä");
  strr_utf8_encode(&teststr);
  str_println(teststr);

  str_auto utf8_str = str("üöäi😊");
  strr_utf8_decode(&utf8_str);
  str_println(utf8_str);
}

int main() {

  test_str_equals();
  test_str_equals_ic();
  test_str_pos();
  test_str_cat();
  test_str_prepend();
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
  test_str_lpad();
  test_str_rpad();

  test_str_token();

  test_mem();

  test_encoding();

  return puts("DONE") < 0;
}
