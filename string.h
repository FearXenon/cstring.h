#pragma once
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#define str(c) str_init_from_chr(c)

#define str_null ((string_t){0, 0})

typedef struct string {
  char *str;
  uint64_t len;
} string_t;

void str_init(string_t *s);
void str_init_s(string_t *s);
string_t str_init_from_chr(char *c);
// for non null terminated
string_t str_init_from_chr_s(char* c, uint64_t len);

void str_free(string_t *s);

int str_equals(string_t s1, string_t s2);
int str_equals_ic(string_t s1, string_t s2);

string_t str_tolower(string_t s);
string_t str_toupper(string_t s);

uint64_t str_pos(string_t s, string_t search);

string_t str_cat(string_t *s, char *str, ...);
void str_push(string_t *s, char *str, ...);

void str_explode(string_t s, string_t delimiter, string_t *arr, uint64_t *len);

static void str_calc_len(string_t *s);
static void str_clone(string_t *target, string_t s);
static string_t str_from_format(char *str, va_list args);
static void str_mem_realloc_cpy(string_t *string, uint64_t len, void *data,
                                uint64_t data_len);
