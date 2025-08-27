
/* here
 * is a 
 * multi line
 * comment
 * 
 */

// here is a single line commmment
// here's another one
//

/* more multi line */

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

typedef struct Str8_find_results Str8_find_results;
struct Str8_find_results {
  s64 *begin_indexes;
  s64 *end_indexes;
  s64 count;
};

#define str8_lit(strlit) ((Str8){ .s = (u8*)(strlit), .len = sizeof(strlit) - 1 })

#define str8_match_lit(a_lit, b) str8_match(str8_lit(a_lit), b)
b32 str8_match(Str8 a_str, Str8 b_str);
b32 str8_starts_with(Str8 str, Str8 start);
b32 str8_ends_with(Str8 str, Str8 end);
b32 str8_contains(Str8 str, Str8 substr);
s64 str8_find(Str8 haystack, Str8 needle);
s64 str8_find_char(Str8 haystack, u8 needle);

Str8_find_results str8_find_all_chars(Str8 haystack, u8 needle, Arena *a);
Str8_find_results str8_find_all(Str8 haystack, Str8 needle, Arena *a);

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

Str8 str8_chop_last_slash(Str8 str);

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

#ifndef ARENA_C
#define ARENA_C


func Arena* arena_alloc_ex(u64 size, b32 cannot_chain, void *backing_buffer) {
  b32 has_backing_buffer = 0;
  void *base = backing_buffer;

  size = ALIGN_UP(size, align_of(void*));

  if(base) {
    cannot_chain = 1;
    has_backing_buffer = 1;
  } else {
    base = os_alloc(size);
    ASSERT(base);
  }

  Arena *arena = (Arena*)base;
  arena->cur = arena;
  arena->prev = 0;
  arena->cannot_chain = cannot_chain;
  arena->has_backing_buffer = has_backing_buffer;
  arena->size = size;
  arena->base_pos = 0;
  arena->pos = JLIB_ARENA_HEADER_SIZE;
  arena->free_size = 0;
  arena->free_last = 0;

  return arena;
}

func void arena_free(Arena *arena) {
  ASSERT(arena);

  if(arena->has_backing_buffer) return;

  for(Arena *a = arena->free_last, *prev = 0; a != 0; a = prev) {
    prev = a->prev;
    os_free((void*)a);
  }

  for(Arena *a = arena->cur, *prev = 0; a != 0; a = prev) {
    prev = a->prev;
    os_free((void*)a);
  }

}

func void *arena_push(Arena *arena, u64 size, u64 align) {
  ASSERT(arena);

  Arena *cur = arena->cur;
  u64 pos = ALIGN_UP(cur->pos, align);
  u64 new_pos = pos + size;

  if(cur->size < new_pos && !cur->cannot_chain) {
    Arena *new_arena = 0;

    Arena *prev_arena;

    for(new_arena = arena->free_last, prev_arena = 0; new_arena != 0; prev_arena = new_arena, new_arena = new_arena->prev) {

      if(new_arena->size >= ALIGN_UP(size, align)) {
        if(prev_arena) {
          prev_arena->prev = new_arena->prev;
        } else {
          arena->free_last = new_arena->prev;
        }
        break;
      }

    }

    if(new_arena == 0) {
      u64 new_arena_size = cur->size;

      if(size + JLIB_ARENA_HEADER_SIZE > new_arena_size) {
        new_arena_size = ALIGN_UP(size + JLIB_ARENA_HEADER_SIZE, align);
      }

      new_arena = arena_alloc_ex(new_arena_size, 0, 0);
    }

    new_arena->base_pos = cur->base_pos + cur->size;

    sll_stack_push_n(arena->cur, new_arena, prev);

    cur = new_arena;
    pos = ALIGN_UP(cur->pos, align);
    new_pos = pos + size;

  }

  void *result = (u8*)cur + pos;
  cur->pos = new_pos;

  return result;
}

func u64 arena_pos(Arena *arena) {
  ASSERT(arena);

  Arena *cur = arena->cur;
  u64 pos = cur->base_pos + cur->pos;
  return pos;
}

func void arena_pop_to(Arena *arena, u64 pos) {
  ASSERT(arena);

  u64 big_pos = CLAMP_BOT(JLIB_ARENA_HEADER_SIZE, pos);
  Arena *cur = arena->cur;

  for(Arena *prev = 0; cur->base_pos >= big_pos; cur = prev) {
    prev = cur->prev;
    cur->pos = JLIB_ARENA_HEADER_SIZE;
    sll_stack_push_n(arena->free_last, cur, prev);
  }

  arena->cur = cur;
  u64 new_pos = big_pos - cur->base_pos;
  ASSERT(new_pos <= cur->pos);
  cur->pos = new_pos;
}

func void arena_clear(Arena *arena) {
  arena_pop_to(arena, 0);
}

func void arena_pop(Arena *arena, u64 amount) {
  u64 old_pos = arena_pos(arena);
  u64 new_pos = old_pos;
  if(amount < old_pos) {
    new_pos = old_pos - amount;
  }
  arena_pop_to(arena, new_pos);
}

func Arena_scope arena_scope_begin(Arena *arena) {
  u64 pos = arena_pos(arena);
  Arena_scope scope = { arena, pos };
  return scope;
}

func void arena_scope_end(Arena_scope scope) {
  arena_pop_to(scope.arena, scope.pos);
}


#endif
