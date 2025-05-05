#pragma once
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

// #define str(c) ((string_t){.str = c, .len = strlen(c) })
#define str(c) str_init_from_chr(c)

#define str_null ((string_t){0, 0, 0})

#define STR_HEAP 0x01
#define STR_OWNER 0x02

typedef struct string {
  char *str;
  size_t len;
  char flags;
} string_t;

void str_init(string_t *s);
void str_init_s(string_t *s);
string_t str_init_from_chr(char *c);
// for non null terminated
string_t str_init_from_chr_s(char *c, size_t len);

void str_set_owner(string_t *dest, string_t *source);
void str_heapify(string_t *s);

void str_free(string_t *s);

int str_chr_is_ws(char *c);

size_t str_len(string_t s);

int str_equals(string_t s1, string_t s2);
int str_equals_ic(string_t s1, string_t s2);

size_t str_pos(string_t s, string_t search);
size_t str_ipos(string_t s, string_t search);

void str_explode(string_t s, string_t delimiter, string_t **arr, size_t *len);
string_t str_implode(string_t delimiter, string_t *arr, size_t len);

string_t str_tolower(string_t s);
string_t str_toupper(string_t s);

string_t str_cat(string_t s, char *str, ...);
void str_push(string_t *s, char *str, ...);

string_t str_replace(string_t s, string_t search, string_t replace);

string_t str_substr(string_t s, ssize_t offset, ssize_t len);

string_t str_trim(string_t s);
string_t str_rtrim(string_t s);
string_t str_ltrim(string_t s);

string_t str_rev(string_t s);

/*
 * string_t reference functions -> modify the string passed
 * */

void strr_implode(string_t *s, string_t delimiter, string_t *arr, size_t len);

void strr_tolower(string_t *s);
void strr_toupper(string_t *s);

void strr_cat(string_t *s, char *str, ...);
static void strr_catv(string_t *s, char *str, va_list args);

void strr_replace(string_t *s, string_t search, string_t replace);

void strr_substr(string_t *s, ssize_t offset, ssize_t len);

void strr_trim(string_t *s);
void strr_rtrim(string_t *s);
void strr_ltrim(string_t *s);

void strr_rev(string_t *s);

static inline char *str_ptr(const string_t s) {
  extern const char *const str_empty_chr;

  return s.str ? s.str : (char *)str_empty_chr;
}
static void str_calc_len(string_t *s);
static void str_clone(string_t *target, string_t s);
static string_t str_from_format(char *str, va_list args);
static void str_mem_realloc_cpy(string_t *string, size_t len, void *data,
                                size_t data_len);
