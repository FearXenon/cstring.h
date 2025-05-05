#include "string.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

const char *const str_empty_chr = "";

string_t str_init_from_chr(char *c) {
  string_t s = {.str = c, .len = 0, .flags = 0};

  str_init(&s);

  return s;
}

string_t str_init_from_chr_s(char *c, size_t len) {
  string_t s = {.str = c, .len = len, .flags = 0};

  str_init_s(&s);

  return s;
}

void str_init(string_t *s) {

  if (!s || !s->str) {
    *s = str_null;
    return;
  }

  size_t len = 0;
  while (*(s->str + len))
    len++;

  s->len = len;

  if (s->len == 0)
    *s = str_null;
}

void str_init_s(string_t *s) {

  if (!s || !s->str)
    return;

  char *tmp = malloc(s->len + 1);

  if (tmp)
    memcpy(tmp, s->str, s->len);

  s->str = tmp;

  // the char* is heap allocated and the current string_t is the owner of it
  s->flags |= STR_HEAP | STR_OWNER;

  if (!s->str)
    return;

  s->str[s->len] = 0;
}

/**
 * transfer the char* ownership to another string_t
 * */
void str_set_owner(string_t *dest, string_t *source) {

  if (!dest || !source)
    return;

  source->flags &= ~STR_OWNER;
  dest->str = source->str;
  dest->flags |= STR_OWNER;
}

/**
 * creates a heap allocated version of the given string
 * */
void str_heapify(string_t *s) {

  if (!s || !s->str)
    return;

  if (!(s->flags & STR_HEAP)) {
    char *tmp = malloc(s->len + 1);

    if (tmp)
      memcpy(tmp, s->str, s->len);

    s->str = tmp;

    // the char* is heap allocated and the current string_t is the owner of it
    s->flags |= STR_HEAP | STR_OWNER;
  }
}

void str_free(string_t *s) {

  if (!s || !s->str)
    return;

  if (s->flags & STR_OWNER) {
    free(s->str);
    s->len = 0;
  }
}

int str_chr_is_ws(char *c) {
  if (!c)
    return 0;

  if (*c == ' ' || *c == '\t' || *c == '\r')
    return 1;

  return 0;
}

size_t str_len(string_t s) { return s.len; }

string_t str_cat(string_t string, char *str, ...) {
  string_t new_s;
  str_clone(&new_s, string);

  va_list args;
  va_start(args, str);

  // Forward to strr_cat
  strr_catv(&new_s, str, args);

  va_end(args);
  return new_s;
}

void strr_cat(string_t *s, char *str, ...) {

  if (!s)
    return;

  va_list args;
  va_start(args, str);

  strr_catv(s, str, args);

  va_end(args);
}

static void strr_catv(string_t *s, char *str, va_list args) {
  string_t formatted = str_from_format(str, args);

  if (!*str_ptr(*s)) {
    // If the destination string is empty, just clone the formatted string
    str_clone(s, formatted);
  } else {
    // Concatenate formatted string to the end
    str_mem_realloc_cpy(s, s->len, str_ptr(formatted), formatted.len);
  }
}

void str_push(string_t *string, char *str, ...) {

  if (!string)
    return;

  va_list args;
  va_start(args, str);
  string_t s = str_from_format(str, args);

  str_mem_realloc_cpy(string, string->len + s.len, str_ptr(s), s.len);

  va_end(args);
}

int str_equals(string_t s1, string_t s2) {
  if (s1.len != s2.len) {
    return 0;
  }
  return !strcmp(str_ptr(s1), str_ptr(s2));
}

int str_equals_ic(string_t s1, string_t s2) {
  if (s1.len != s2.len) {
    return 0;
  }

  char *s1_chr = (char *)str_ptr(s1);
  char *s2_chr = (char *)str_ptr(s2);
  for (; *s1_chr; s1_chr++, s2_chr++) {
    if (!(tolower(*s1_chr) == tolower(*s2_chr))) {
      return 0;
    }
  }

  return 1;
}

string_t str_tolower(string_t s) {
  string_t new;
  str_clone(&new, s);

  strr_tolower(&new);

  return new;
}

void strr_tolower(string_t *s) {

  if (!s || !s->str)
    return;

  for (int i = 0; i < s->len; i++)
    s->str[i] = tolower(s->str[i]);
}

string_t str_toupper(string_t s) {
  string_t new;
  str_clone(&new, s);

  strr_toupper(&new);

  return new;
}

void strr_toupper(string_t *s) {

  if (!s || !s->str)
    return;

  for (int i = 0; i < s->len; i++)
    s->str[i] = toupper(s->str[i]);
}

void str_calc_len(string_t *s) {

  if (!s || !s->str)
    return;

  size_t len = 0;
  char *chr = s->str;
  for (; *chr; chr++)
    len++;

  s->len = len;
}

size_t str_pos(string_t s, string_t search) {
  size_t pos = -1;

  if (!*str_ptr(search))
    return 0;

  if (!*str_ptr(s))
    return -1;

  if (s.len < search.len)
    return -1;

  if (str_equals(s, search))
    return 0;

  char *c = s.str;
  char *cl = c;
  char *search_c = search.str;
  while (*cl) {

    while (*c && *c == *search_c) {
      c++;
      search_c++;
    }

    // found the string
    if (!*search_c) {
      return cl - s.str;
    } else {
      // set the str to the next base str
      c = cl + 1;
      // advance the base str
      cl++;
      // reset search pointer
      search_c = search.str;
      continue;
    }
    cl++;
    c++;
  }

  return pos;
}

size_t str_ipos(string_t s, string_t search) {
  size_t pos = -1;

  if (!*str_ptr(search))
    return 0;

  if (!*str_ptr(s))
    return -1;

  if (s.len < search.len)
    return -1;

  if (str_equals(s, search))
    return 0;

  char *c = s.str;
  char *cl = c;
  char *search_c = search.str;
  while (*cl) {

    while (*c && tolower(*c) == tolower(*search_c)) {
      c++;
      search_c++;
    }

    // found the string
    if (!*search_c) {
      return cl - s.str;
    } else {
      // set the str to the next base str
      c = cl + 1;
      // advance the base str
      cl++;
      // reset search pointer
      search_c = search.str;
      continue;
    }
    cl++;
    c++;
  }

  return pos;
}

void str_explode(string_t s, string_t delimiter, string_t **arr, size_t *len) {

  *arr = malloc(sizeof(string_t));

  if (!*str_ptr(s) || !*str_ptr(delimiter)) {
    *len = 1;
    str_clone(*arr, s);
    return;
  }

  *len = 0;

  string_t s_var;
  str_clone(&s_var, s);
  char *s_var_base = s_var.str;
  size_t pos;
  while ((pos = str_pos(s_var, delimiter)) != -1) {
    // delimiter is not at the very front
    if (pos != 0) {
      string_t entry = str_init_from_chr_s(s_var.str, pos);
      str_clone(*arr + *len, entry);
    } else {
      str_clone(*arr + *len, str_null);
    }
    // advance the str pointer
    s_var.str += pos + delimiter.len;
    s_var.len -= pos + delimiter.len;
    /*str_calc_len(&s_var);*/

    (*len)++;

    *arr = realloc(*arr, sizeof(string_t) * (*len + 1));
    if (!arr)
      return;
  }

  str_clone(*arr + *len, s_var);
  (*len)++;

  s_var.str = s_var_base;
  str_free(&s_var);
}

string_t str_implode(string_t delimiter, string_t *arr, size_t len) {
  string_t s = str_null;

  strr_implode(&s, delimiter, arr, len);

  return s;
}

void strr_implode(string_t *s, string_t delimiter, string_t *arr, size_t len) {

  if (!s)
    return;

  if (len == 0)
    return;

  for (int i = 0; i < len; i++) {
    string_t s_arr = *(arr + i);

    str_push(s, "%S", s_arr);

    if (i != len - 1)
      str_push(s, "%S", delimiter);
  }
}

string_t str_replace(string_t s, string_t search, string_t replace) {
  string_t s_new = str_null;

  if (!*str_ptr(search))
    return s;

  size_t pos;
  while ((pos = str_pos(s, search)) != -1) {

    if (pos != 0) {
      // add everything up to the "search"
      str_mem_realloc_cpy(&s_new, s_new.len + pos, s.str, pos);
    }

    // add the replace
    str_mem_realloc_cpy(&s_new, s_new.len + replace.len, replace.str,
                        replace.len);

    s.str += pos + search.len;
    s.len -= pos + search.len;
  }

  // add the rest
  str_mem_realloc_cpy(&s_new, s_new.len + s.len, s.str, s.len);

  return s_new;
}

void strr_replace(string_t *s, string_t search, string_t replace) {

  if (!s)
    return;

  if (!*str_ptr(search))
    return;

  string_t s_cpy;
  str_clone(&s_cpy, *s);

  size_t pos;
  while ((pos = str_pos(s_cpy, search)) != -1) {

    if (pos != 0) {
      // add everything up to the "search"
      str_mem_realloc_cpy(s, s->len + pos, s_cpy.str, pos);
    }

    // add the replace
    str_mem_realloc_cpy(s, s->len + replace.len, replace.str, replace.len);

    s_cpy.str += pos + search.len;
    s_cpy.len -= pos + search.len;
  }

  // add the rest
  str_mem_realloc_cpy(s, s->len + s_cpy.len, s_cpy.str, s_cpy.len);

  str_free(&s_cpy);
}

string_t str_substr(string_t s, ssize_t offset, ssize_t len) {
  string_t s_new = str_null;

  if (offset >= (ssize_t)s.len || (offset < 0 && len < offset))
    return s_new;

  if (len > (ssize_t)s.len)
    len = (ssize_t)s.len;

  if (offset < 0) {
    offset = (ssize_t)s.len + offset;
    if (offset < 0)
      offset = 0;
  }
  if (len < 0)
    len = (ssize_t)s.len + len - offset;

  s_new = str_init_from_chr_s(s.str + offset, len);

  return s_new;
}

void strr_substr(string_t *s, ssize_t offset, ssize_t len) {

  if (!s)
    return;

  string_t s_cpy;
  str_clone(&s_cpy, *s);

  if (offset >= (ssize_t)s_cpy.len || (offset < 0 && len < offset))
    return;

  if (len > (ssize_t)s_cpy.len)
    len = (ssize_t)s_cpy.len;

  if (offset < 0) {
    offset = (ssize_t)s_cpy.len + offset;
    if (offset < 0)
      offset = 0;
  }
  if (len < 0)
    len = (ssize_t)s_cpy.len + len - offset;

  *s = str_init_from_chr_s(s_cpy.str + offset, len);

  str_free(&s_cpy);
}

string_t str_trim(string_t s) {
  string_t s_new = str_null;

  size_t len = s.len;
  char *c = str_ptr(s);
  char *c_end = str_ptr(s) + len - 1;

  if (!(*c))
    return s_new;

  while (str_chr_is_ws(c)) {
    c++;
    len--;
  }

  while (str_chr_is_ws(c_end)) {
    c_end--;
    len--;
  }

  s_new = str_init_from_chr_s(c, len);

  return s_new;
}

void strr_trim(string_t *s) {

  if (!s)
    return;

  str_heapify(s);

  char *c = s->str;
  char *c_end = str_ptr(*s) + s->len - 1;

  size_t start = 0;
  size_t end = s->len;

  while (str_chr_is_ws(c)) {
    c++;
    start++;
  }

  while (str_chr_is_ws(c_end)) {
    c_end--;
    end--;
  }

  char *tmp = malloc(end - start);

  if (!tmp)
    return;

  memcpy(tmp, c, end - start);

  free(s->str);

  s->str = tmp;
  s->len = end - start;
}

string_t str_rtrim(string_t s) {
  string_t s_new = str_null;

  size_t len = s.len;
  char *c = str_ptr(s);
  char *c_end = str_ptr(s) + len - 1;

  if (!(*c))
    return s_new;

  while (str_chr_is_ws(c_end)) {
    c_end--;
    len--;
  }

  s_new = str_init_from_chr_s(c, len);

  return s_new;
}

string_t str_ltrim(string_t s) {
  string_t s_new = str_null;

  size_t len = s.len;
  char *c = str_ptr(s);

  if (!(*c))
    return s_new;

  while (str_chr_is_ws(c)) {
    c++;
    len--;
  }

  s_new = str_init_from_chr_s(c, len);

  return s_new;
}

string_t str_rev(string_t s) {
  string_t s_new = str_null;

  char *c = str_ptr(s);

  if (!*c)
    return s_new;

  str_clone(&s_new, s);

  int i, j;
  for (i = s.len - 1, j = 0; i >= 0; i--, j++) {
    s_new.str[i] = s.str[j];
  }

  return s_new;
}

/**
 * clones the given string into the pointer
 * clones the char* aswell
 * */
static void str_clone(string_t *target, string_t s) {

  if (!target)
    return;

  char *new_str = malloc(s.len + 1);

  if (!new_str)
    return;

  memcpy(new_str, str_ptr(s), s.len);

  target->str = new_str;
  target->len = s.len;

  target->str[s.len] = 0;
}

static string_t str_from_format(char *str, va_list args) {

  string_t s = {.str = (void *)0, .len = 0};

  size_t len = 0;

  int arg_int;
  long arg_long;
  string_t arg_str;
  char *arg_c_str;

  // e.g "%d test%s%"
  while (*str) {
    if (*str == '%') {
      str++;

      int tmp_len = 0;
      int factor;
      switch (*str) {
      case 'd':
        arg_int = va_arg(args, int);
        tmp_len = 1;
        factor = 10;
        while (factor < arg_int) {
          factor *= 10;
          tmp_len++;
        }

        len += tmp_len;

        s.str = realloc(s.str, len);
        factor /= 10;
        for (int i = 0; i < tmp_len; i++) {
          s.str[s.len + i] = '0' + (arg_int / factor);
          arg_int -= (arg_int / factor) * factor;
          factor /= 10;
        }
        s.len += tmp_len;
        break;
      case 'l':
        arg_long = va_arg(args, long);
        tmp_len = 1;
        factor = 10;
        while (factor < arg_long) {
          factor *= 10;
          tmp_len++;
        }
        len += tmp_len;

        s.str = realloc(s.str, len);
        factor /= 10;
        for (int i = 0; i < tmp_len; i++) {
          s.str[s.len + i] = '0' + (arg_long / factor);
          arg_long -= (arg_long / factor) * factor;
          factor /= 10;
        }
        s.len += tmp_len;
        break;
      case 'S':
        arg_str = va_arg(args, string_t);
        len += arg_str.len;

        str_mem_realloc_cpy(&s, len, arg_str.str, arg_str.len);
        break;
      case 's':
        arg_c_str = va_arg(args, char *);
        while (*arg_c_str) {
          tmp_len++;
          arg_c_str++;
        }
        len += tmp_len;

        str_mem_realloc_cpy(&s, len, arg_c_str - tmp_len, tmp_len);
        break;
      case 'c':
        len++;
        break;

      default:
        len++;
        break;
      }
      str++;
      continue;

    } else {
      char *str_start = str;
      while (*str && *str != '%') {
        str++;
        len++;
        if (!*str || *str == '%') {
          str_mem_realloc_cpy(&s, len, str_start, str - str_start);
        }
      }
    }
  }

  // add null byte
  s.str = realloc(s.str, len + 1);

  if (!s.str)
    return str_null;

  memset(&s.str[s.len], 0, 1);

  return s;
}

/**
 * string: pointer to the string
 * len: new string length
 * data: data to be appended
 * data_len: size of the data to be appended
 * */
static void str_mem_realloc_cpy(string_t *string, size_t len, void *data,
                                size_t data_len) {

  if (!string)
    return;

  if (!(string->flags & STR_HEAP))
    str_heapify(string);

  string->str = realloc(string->str, len);

  if (!string->str)
    return;

  memcpy(string->str + string->len, data, data_len);
  string->len += data_len;
}
