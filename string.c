#include "string.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// str("test").cat

string_t str_init_from_chr(char *c) {
  string_t s = {.str = c};

  str_init(&s);

  return s;
}

string_t str_init_from_chr_s(char *c, uint64_t len) {
  string_t s = {.str = c, len = len}; 

  str_init_s(&s);

  return s;
}

void str_init(string_t *s) {

  if (!s || !s->str)
    return;

  uint64_t len = 0;
  while (*(s->str + len))
    len++;

  s->len = len;
}

void str_init_s(string_t* s) {

  if (!s || !s->str)
    return;
    
  s->str = realloc(s->str, s->len + 1); 

  if (!s->str)
    return;

  s->str[s->len] = 0;

}

void str_free(string_t* s) {

  if (!s || !s->str)
    return;

  free(s->str);
}

string_t str_cat(string_t *string, char *str, ...) {

  va_list args;
  va_start(args, str);

  if (!string || !string->str) {
    string_t s = str_from_format(str, args);

    va_end(args);

    return s;
  }

  string_t new_string;
  str_clone(&new_string, *string);

  string_t s = str_from_format(str, args);

  str_mem_realloc_cpy(&new_string, new_string.len, s.str, s.len);

  va_end(args);

  return new_string;
}

void str_push(string_t *string, char *str, ...) {
  
  if (!string || !string->str)
    return;

  va_list args;
  va_start(args, str);
  string_t s = str_from_format(str, args);

  str_mem_realloc_cpy(string, string->len, s.str, s.len);

  va_end(args);
}

int str_equals(string_t s1, string_t s2) {
  if (s1.len != s2.len) {
    return 0;
  }
  return !strcmp(s1.str, s2.str);
}

int str_equals_ic(string_t s1, string_t s2) {
  if (s1.len != s2.len) {
    return 0;
  }

  for (; *s1.str ; s1.str++, s2.str++) {
    if (!(tolower(*s1.str) == tolower(*s2.str))) {
      return 0;
    } 
  }

  return 1;
}

string_t str_tolower(string_t s) {
  string_t new;
  str_clone(&new, s);

  for (int i = 0; i < new.len; i++)
    new.str[i] = tolower(new.str[i]);
  

  return new;
}

string_t str_toupper(string_t s) {
  string_t new;
  str_clone(&new, s);

  for (int i = 0; i < new.len; i++)
    new.str[i] = toupper(new.str[i]);
  

  return new;
}

void str_calc_len(string_t *s) {

  if (!s || !s->str)
    return;

  uint64_t len = 0;
  char* chr = s->str;
  for(; *chr; chr++)
    len++;

  s->len = len;

}

uint64_t str_pos(string_t s, string_t search) {
  
  uint64_t pos = -1;

  if (!s.str || !search.str)
    return -1;

  if (s.len < search.len) 
    return -1;

  if (str_equals(s, search)) 
    return 0;

  char* c = s.str;
  char* cl = c;
  char* search_c = search.str;
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
      c = cl+1;
      //advance the base str
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

void str_explode(string_t s, string_t delimiter, string_t *arr, uint64_t *len) {
 
  if (!s.str || !delimiter.str)
    return;

  *len = 1;

  char* c = s.str;

  // test;123;abc -> del: ;
  string_t s_var;
  str_clone(&s_var, s);
  uint64_t pos;
  while ((pos = str_pos(s, delimiter)) != -1) {
    if (pos != 0) {
      string_t entry = str_init_from_chr_s(s_var.str, pos - 1);
    }
     
  }

  if (*(len) == 1) {
    string_t str;
    str_clone(&str, s);
    *arr = str;
  }
  
}

// TODO: memory safety
static void str_clone(string_t *target, string_t s) {
  
  string_t new = *((string_t*)malloc(sizeof(string_t)));

  char* new_str = malloc(s.len + 1);

  if (!new_str)
    return;

  memcpy(new_str, s.str, s.len);

  new.str = new_str;
  new.len = s.len;

  new.str[s.len] = 0;

}

static string_t str_from_format(char *str, va_list args) {

  string_t s = {.str = (void *)0, .len = 0};

  uint64_t len = 0;

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
        while (factor < arg_int) {
          factor *= 10;
          tmp_len++;
        }
        len += tmp_len;
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

static void str_mem_realloc_cpy(string_t *string, uint64_t len, void *data, uint64_t data_len) {

  if (!string)
    return;

  string->str = realloc(string->str, len);

  if (!string->str)
    return;

  memcpy(string->str + string->len, data, data_len);
  string->len += data_len;
}
