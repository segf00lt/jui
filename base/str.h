#ifndef STR_H
#define STR_H

typedef struct Str8 Str8;
struct Str8 {
  u8 *s;
  s64 len;
};

typedef struct Str8_node Str8_node;
struct Str8_node {
  Str8 str;
  Str8_node *next;
};

typedef struct Str8_list Str8_list;
struct Str8_list {
  Str8_node *first;
  Str8_node *last;
  s64 count;
  s64 total_len;
};

#define str8_lit(strlit) ((Str8){ .s = (u8*)(strlit), .len = sizeof(strlit) - 1 })

#define str8_match_lit(a_lit, b) str8_match(str8_lit(a_lit), b)
b32 str8_match(Str8 a_str, Str8 b_str);
b32 str8_starts_with(Str8 str, Str8 start);
b32 str8_ends_with(Str8 str, Str8 end);
b32 str8_contains(Str8 str, Str8 substr);
s64 str8_find(Str8 haystack, Str8 needle);

Str8 str8_cat(Arena *a, Str8 str1, Str8 str2);

b32 str8_is_cident(Str8 str);
b32 str8_is_alpha(Str8 str);
b32 str8_is_numeric(Str8 str, int base);
b32 str8_is_decimal(Str8 str);

Str8 str8_to_upper(Arena *a, Str8 str);
Str8 str8_to_lower(Arena *a, Str8 str);

Str8 str8_slice(Str8 str, s64 begin, s64 end);

#define is_space(c) (!!('\0' <= (c) && (c) <= ' '))
#define is_upper(c) (!!('A' <= (c) && (c) <= 'Z'))
#define is_lower(c) (!!('a' <= (c) && (c) <= 'z'))
#define to_lower(c) (is_upper(c) ? ((c) - 'A' + 'a') : (c))
#define to_upper(c) (is_lower(c) ? ((c) - 'a' + 'A') : (c))
#define is_alpha(c) ('a' <= to_lower(c) && to_lower(c) <= 'z')
#define is_decimal(c) (!!('0' <= (c) && (c) <= '9'))
#define letter_index(c) ((s64)(to_lower(c) - 'a'))
#define hexdigit_to_int(c) ((s64)(is_alpha(c) ? (to_lower(c) - 'a' + 0xa) : (c - '0')))

#define str8_split_by_chars_lit(a, str, sep_chars_lit) str8_split_by_chars(a, str, (u8*)sep_chars_lit, (s64)sizeof(sep_chars_lit))
#define str8_split_by_string_lit(a, str, sep) str8_split_by_string(a, str, str8_lit(sep))
Str8_list str8_split_by_string(Arena *a, Str8 str, Str8 sep);
Str8_list str8_split_by_chars(Arena *a, Str8 str, u8 *sep_chars, s64 n_sep_chars);
Str8_list str8_split_by_char(Arena *a, Str8 str, u8 sep_char);

#define str8_list_append_node(list, node) str8_list_append_node_(&(list), node)
void str8_list_append_node_(Str8_list *list, Str8_node *node);

#define str8_list_append_str(a, list, str) str8_list_append_str_(a, &(list), str)
void str8_list_append_str_(Arena *a, Str8_list *list, Str8 str);

Str8 str8_list_join(Arena *a, Str8_list list, Str8 sep);

Str8_list str8_list_copy(Arena *a, Str8_list list);

Str8  str8_copy(Arena *a, Str8 str);
Str8  str8_copy_cstr(Arena *a, char *cstr);
Str8  str8fv(Arena *a, char *fmt, va_list args);
Str8  str8f(Arena *a, char *fmt, ...);
char* cstr_copy_str8(Arena *a, Str8 str);
char* cstrf(Arena *a, char *fmt, ...);

#define str8_list_insert_first_str(a, list, str) str8_list_insert_first_str_(a, &(list), str)
void str8_list_insert_first_str_(Arena *a, Str8_list *list, Str8 str);

#endif
