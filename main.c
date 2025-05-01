#include "string.h"
#include <assert.h>
#include <stdio.h>

#define passed printf("test passed: %s\n", __func__)

static void test_str_pos() {

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

int main() {

  test_str_equals();
  test_str_equals_ic();
  test_str_pos();

  return puts("OK") < 0;
}
