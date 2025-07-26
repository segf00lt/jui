
#define STB_SPRINTF_IMPLEMENTATION
#include "../third_party/stb/stb_sprintf.h"

#define str_vsnprintf stbsp_vsnprintf

//#if defined(OS_WEB)
//#include <stdio.h>
//#define jlib_str_vsnprintf vsnprintf
//#else
//#endif


func force_inline void str8_list_append_node_(Str8_list *list, Str8_node *node) {
  sll_queue_push(list->first, list->last, node);
  list->count++;
  list->total_len += node->str.len;
}

func Str8 str8_cat(Arena *a, Str8 str1, Str8 str2) {
  Str8 result = {0};
  s64 len = str1.len + str2.len;

  if(len <= 0) {
    return result;
  }

  result.len = len;
  result.s = push_array_no_zero(a, u8, len + 1);

  memory_copy(result.s, str1.s, str1.len);
  memory_copy(result.s + str1.len, str2.s, str2.len);
  result.s[len] = 0;

  return result;
}

func void str8_list_insert_first_str_(Arena *a, Str8_list *list, Str8 str) {
  Str8_node *node = push_struct(a, Str8_node);
  node->str = str;
  sll_queue_push_front(list->first, list->last, node);
  list->count++;
  list->total_len += str.len;
}

func void str8_list_append_str_(Arena *a, Str8_list *list, Str8 str) {
  Str8_node *node = push_struct(a, Str8_node);
  node->str = str;
  sll_queue_push(list->first, list->last, node);
  list->count++;
  list->total_len += str.len;
}

func Str8_list str8_list_copy(Arena *a, Str8_list list) {
  Str8_list result = {0};

  for(Str8_node *node = list.first; node; node = node->next) {
    Str8_node *new_node = push_array_no_zero(a, Str8_node, 1);
    new_node->str = node->str;
    new_node->next = 0;
    str8_list_append_node_(&result, new_node);
  }

  return result;
}

func Str8 str8_list_join(Arena *a, Str8_list list, Str8 sep) {
  Str8 result = {0};

  Str8_node *node = list.first;

  if(list.count <= 0) return result;
  if(!node) return result;

  s64 len = ((sep.len > 0) ? (list.count - 1) : (0)) * sep.len + list.total_len;

  result =
    (Str8) {
      .s = push_array_no_zero(a, u8, len),
      .len = len,
    };

  s64 len_copied = 0;

  while(node && node->str.len <= 0) {
    node = node->next;
  }

  memory_copy(result.s + len_copied, node->str.s, node->str.len);
  len_copied += node->str.len;

  node = node->next;

  for(; node; node = node->next) {
    if(node->str.len <= 0) continue;

    memory_copy(result.s + len_copied, sep.s, sep.len);
    len_copied += sep.len;

    memory_copy(result.s + len_copied, node->str.s, node->str.len);
    len_copied += node->str.len;
  }

  if(len > len_copied) {
    arena_pop(a, len - len_copied);
  }

  result.s[len_copied] = 0;

  return result;
}

func Str8 str8_slice(Str8 str, s64 begin, s64 end) {
  /* NOTE slice end is exclusive */

  end = MIN(end, str.len);

  s64 new_len = end - begin;

  Str8 result = { .s = str.s + MAX(begin, 0), .len = new_len };

  return result;
}

func Str8 str8_strip_whitespace(Str8 str) {
  Str8 result;
  s64 begin = 0;
  s64 end = str.len - 1;

  for(; begin < str.len; begin++) {
    if(!is_space(str.s[begin])) {
      break;
    }
  }

  for(; end >= 0; end--) {
    if(!is_space(str.s[end])) {
      end += 1;
      break;
    }
  }

  if(end < 0) {
    end = str.len;
  }

  result = str8_slice(str, begin, end);

  return result;
}

func b32 str8_match(Str8 a, Str8 b) {
  if(a.len != b.len) {
    return 0;
  } else {
    return (b32)(memory_compare(a.s, b.s, a.len) == 0);
  }
}

func b32 str8_contains(Str8 str, Str8 substr) {
  s64 found = str8_find(str, substr);
  b32 result = (found >= 0);
  return result;
}

func s64 str8_find(Str8 haystack, Str8 needle) {
  s64 found = -1;

  for(s64 i = 0; i < haystack.len - needle.len; i++) {
    for(s64 j = 0; j < needle.len; j++) {
      if(haystack.s[i+j] != needle.s[j]) {
        goto continue_outer;
      }
    }

    found = i;
    goto end;

continue_outer:;
  }

end:;

  return found;
}

func b32 str8_starts_with(Str8 str, Str8 start) {
  b32 result = 0;

  if(str.len >= start.len) {
    Str8 str_start = str;
    str_start.len = start.len;
    result = str8_match(str_start, start);
  }

  return result;
}

func b32 str8_ends_with(Str8 str, Str8 end) {
  b32 result = 0;

  if(str.len >= end.len) {
    Str8 str_end =
    {
      .s = str.s + str.len - end.len,
      .len = end.len,
    };
    result = str8_match(str_end, end);
  }

  return result;
}

func b32 str8_is_cident(Str8 str) {
  b32 result = 1;

  if(!is_alpha(str.s[0]) && str.s[0] != '_') {
    result = 0;
  } else {

    for(int i = 1; i < str.len; i++) {
      if(!is_alpha(str.s[i]) && str.s[i] != '_' && !is_decimal(str.s[i])) {
        result = 0;
        break;
      }
    }

  }

  return result;
}

func b32 str8_is_decimal(Str8 str) {
  b32 result = 1;

  for(int i = 0; i < str.len; i++) {
    if(!is_decimal(str.s[i])) {
      result = 0;
      break;
    }
  }

  return result;
}

func Str8 str8_copy(Arena *a, Str8 str) {
  u8 *s = push_array_no_zero(a, u8, str.len + 1);
  memory_copy(s, str.s, str.len);
  s[str.len] = 0;
  return (Str8){ .s = s, .len = str.len };
}

func force_inline Str8 str8_copy_cstr(Arena *a, char *cstr) {
  Str8 str = { .s = (u8*)cstr, .len = memory_strlen(cstr) };
  return str8_copy(a, str);
}

func force_inline char* cstr_copy_str8(Arena *a, Str8 str) {
  Str8 s_ = str8_copy(a, str);
  char *s = (char*)s_.s;
  return s;
}

func Str8 str8fv(Arena *a, char *fmt, va_list args) {
  va_list args2;
  va_copy(args2, args);
  u32 needed_bytes = str_vsnprintf(0, 0, fmt, args) + 1;
  Str8 result = {0};
  result.s = (u8*)arena_push(a, sizeof(u8) * needed_bytes, align_of(u8));
  result.len = str_vsnprintf((char*)result.s, needed_bytes, fmt, args2);
  result.s[result.len] = 0;
  va_end(args2);
  return result;
}

func Str8 str8f(Arena *a, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  Str8 result = str8fv(a, fmt, args);
  va_end(args);
  return result;
}

func char* cstrf(Arena *a, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  Str8 result = str8fv(a, fmt, args);
  va_end(args);
  return (char*)(result.s);
}

func Str8 str8_to_lower(Arena *a, Str8 str) {
  Str8 lower_str = str8_copy(a, str);

  for(int i = 0; i < lower_str.len; i++) {
    u8 c = lower_str.s[i];
    lower_str.s[i] = to_lower(c);
  }

  return lower_str;
}

func Str8 str8_to_upper(Arena *a, Str8 str) {
  Str8 upper_str = str8_copy(a, str);

  for(int i = 0; i < upper_str.len; i++) {
    u8 c = upper_str.s[i];
    upper_str.s[i] = to_upper(c);
  }

  return upper_str;
}

func Str8_list str8_split_by_chars(Arena *a, Str8 str, u8 *sep_chars, s64 n_sep_chars) {
  Str8_list result = {0};
  Str8_node head = {0};
  Str8_node *node = &head;

  s64 begin = 0;

  s64 i = 0;
  for(;i < str.len;) {
    b8 found_match = 0;

    for(s64 j = 0; j < n_sep_chars; j++) {
      if(str.s[i] == sep_chars[j]) {
        found_match = 1;
        break;
      }
    }

    if(found_match) {
      if(i == 0) {
        i += 1;
        begin = i;
      } else {
        node->next = push_array_no_zero(a, Str8_node, 1);
        node = node->next;
        node->str.s = str.s + begin;
        node->str.len = i - begin;
        node->next = 0;
        i += 1;
        begin = i;

        result.count++;
        result.total_len += node->str.len;
      }

    } else {
      i++;
    }

  }

  if(begin < i) {
    node->next = push_array_no_zero(a, Str8_node, 1);
    node = node->next;
    node->str.s = str.s + begin;
    node->str.len = i - begin;
    node->next = 0;

    result.count++;
    result.total_len += node->str.len;
  }

  result.first = head.next;
  result.last = node;

  return result;
}

func force_inline Str8_list str8_split_by_char(Arena *a, Str8 str, u8 sep_char) {
  return str8_split_by_chars(a, str, &sep_char, 1);
}

func Str8_list str8_split_by_string(Arena *a, Str8 str, Str8 sep) {
  Str8_list result = {0};
  Str8_node head = {0};
  Str8_node *node = &head;

  s64 begin = 0;

  s64 i = 0;
  for(;i < str.len;) {
    s64 j = 0;
    b8 found_match = 1;

    for(;j < sep.len && i+j < str.len; j++) {
      if(str.s[i+j] != sep.s[j]) {
        found_match = 0;
        break;
      }
    }

    if(found_match) {
      if(i == 0) {
        i += j;
        begin = i;
      } else {
        node->next = push_array_no_zero(a, Str8_node, 1);
        node = node->next;
        node->str.s = str.s + begin;
        node->str.len = i - begin;
        node->next = 0;
        i += j;
        begin = i;

        result.count++;
        result.total_len += node->str.len;
      }

    } else {
      i++;
    }

  }

  if(begin < i) {
    node->next = push_array_no_zero(a, Str8_node, 1);
    node = node->next;
    node->str.s = str.s + begin;
    node->str.len = i - begin;
    node->next = 0;

    result.count++;
    result.total_len += node->str.len;
  }

  result.first = head.next;
  result.last = node;

  return result;
}


