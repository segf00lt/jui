
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

continue_outer:
    ;
  }

end:

  return found;
}

func s64 str8_find_char(Str8 haystack, u8 needle) {
  s64 found = -1;

  for(s64 i = 0; i < haystack.len; i++) {
    if(haystack.s[i] != needle) {
      continue;
    }

    found = i;
    break;
  }

  return found;
}

func Str8_find_results str8_find_all_chars(Str8 haystack, u8 needle, Arena *a) {
  Str8_find_results results = {0};

  Arena_scope scratch = scratch_begin(0, 0);

  Arr(s64) begin_indexes;
  Arr(s64) end_indexes;
  arr_init_ex(begin_indexes, scratch.arena, 64);
  arr_init_ex(end_indexes, scratch.arena, 64);

  for(s64 i = 0; i < haystack.len; i++) {
    if(haystack.s[i] != needle) {
      continue;
    }

    arr_push(begin_indexes, i);
    arr_push(end_indexes, i);
  }

  if(begin_indexes.count > 0) {
    ASSERT(end_indexes.count > 0);

    results.begin_indexes = push_array_no_zero(a, s64, begin_indexes.count);
    memory_copy(results.begin_indexes, begin_indexes.d, begin_indexes.count * arr_stride(begin_indexes));

    results.end_indexes = push_array_no_zero(a, s64, end_indexes.count);
    memory_copy(results.end_indexes, end_indexes.d, end_indexes.count * arr_stride(end_indexes));

    results.count = begin_indexes.count;
  }

  scratch_end(scratch);

  return results;
}

func Str8_find_results str8_find_all(Str8 haystack, Str8 needle, Arena *a) {
  Str8_find_results results = {0};

  Arena_scope scratch = scratch_begin(0, 0);

  Arr(s64) begin_indexes;
  Arr(s64) end_indexes;
  arr_init_ex(begin_indexes, scratch.arena, 64);
  arr_init_ex(end_indexes, scratch.arena, 64);

  for(s64 i = 0; i < haystack.len - needle.len; i++) {
    for(s64 j = 0; j < needle.len; j++) {
      if(haystack.s[i+j] != needle.s[j]) {
        goto continue_outer;
      }
    }

    arr_push(begin_indexes, i);
    arr_push(end_indexes, i + needle.len);

continue_outer:
    ;
  }

  if(begin_indexes.count > 0) {
    ASSERT(end_indexes.count > 0);

    results.begin_indexes = push_array_no_zero(a, s64, begin_indexes.count);
    memory_copy(results.begin_indexes, begin_indexes.d, begin_indexes.count * arr_stride(begin_indexes));

    results.end_indexes = push_array_no_zero(a, s64, end_indexes.count);
    memory_copy(results.end_indexes, end_indexes.d, end_indexes.count * arr_stride(end_indexes));

    results.count = begin_indexes.count;
  }

  scratch_end(scratch);

  return results;
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
  return (Str8) { .s = s, .len = str.len };
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
  result.s = (u8*)arena_push(a, sizeof(u8) * needed_bytes, alignof(u8));
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

func Str8 str8_chop_last_slash(Str8 str) {
  if(str.len > 0) {
    u8 *ptr = str.s + str.len - 1;
    for(;ptr >= str.s; ptr -= 1) {
      if(*ptr == '/' || *ptr == '\\')
      {
        break;
      }
    }
    if(ptr >= str.s) {
      str.len = (u64)(ptr - str.s);
    }
    else
    {
      str.len = 0;
    }
  }
  return str;
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

func Str8_list str8_split_by_str(Arena *a, Str8 str, Str8 sep) {
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

func Str8 str8_cstr_capped(void *cstr, void *cap) {
  char *ptr = (char *)cstr;
  char *opl = (char *)cap;
  for(;ptr < opl && *ptr != 0; ptr += 1);
  u64 size = (u64)(ptr - (char *)cstr);
  Str8 result = (Str8){ (u8*)cstr, size };
  return result;
}

func void str8_serial_begin(Arena *arena, Str8_list *srl) {
  Str8_node *node = push_array(arena, Str8_node, 1);
  node->str.s = push_array_no_zero(arena, u8, 0);
  srl->first = srl->last = node;
  srl->count = 1;
  srl->total_len = 0;
}

func Str8 str8_serial_end(Arena *arena, Str8_list *srl) {
  u64 len = srl->total_len;
  u8 *out = push_array_no_zero(arena, u8, len);
  str8_serial_write_to_dst(srl, out);
  Str8 result = { out, len };
  return result;
}

func void str8_serial_write_to_dst(Str8_list *srl, void *out) {
  u8 *ptr = (u8*)out;
  for(Str8_node *node = srl->first; node != 0; node = node->next) {
    u64 len = node->str.len;
    memory_copy(ptr, node->str.s, len);
    ptr += len;
  }
}

func u64 str8_serial_push_align(Arena *arena, Str8_list *srl, u64 align) {
  ASSERT(IS_POW_2(align));
  
  u64 pos = srl->total_len;
  u64 new_pos = ALIGN_UP(pos, align);
  u64 len = (new_pos - pos);
  
  if(len != 0) {
    u8 *buf = push_array(arena, u8, len);
    
    Str8 *str = &srl->last->str;
    if(str->s + str->len == buf) {
      srl->last->str.len += len;
      srl->total_len += len;
    } else {
      str8_list_append_str_(arena, srl, (Str8){ buf, len });
    }
  }
  return len;
}

func void* str8_serial_push_len(Arena *arena, Str8_list *srl, u64 len) {
  void *result = 0;
  if(len != 0) {
    u8 *buf = push_array_no_zero(arena, u8, len);
    Str8 *str = &srl->last->str;
    if(str->s + str->len == buf) {
      srl->last->str.len += len;
      srl->total_len += len;
    } else {
      str8_list_append_str_(arena, srl, (Str8){ buf, len });
    }
    result = buf;
  }
  return result;
}

func void* str8_serial_push_data(Arena *arena, Str8_list *srl, void *data, u64 len) {
  void *result = str8_serial_push_len(arena, srl, len);
  if(result != 0) {
    memory_copy(result, data, len);
  }
  return result;
}

func void str8_serial_push_data_list(Arena *arena, Str8_list *srl, Str8_node *first) {
  for(Str8_node *node = first; node != 0; node = node->next) {
    str8_serial_push_data(arena, srl, node->str.s, node->str.len);
  }
}

func void str8_serial_push_u64(Arena *arena, Str8_list *srl, u64 x) {
  u8 *buf = push_array_no_zero(arena, u8, 8);
  memory_copy(buf, &x, 8);
  Str8 *str = &srl->last->str;
  if(str->s + str->len == buf) {
    srl->last->str.len += 8;
    srl->total_len += 8;
  } else {
    str8_list_append_str_(arena, srl, (Str8){ buf, 8 });
  }
}

func void str8_serial_push_u32(Arena *arena, Str8_list *srl, u32 x) {
  u8 *buf = push_array_no_zero(arena, u8, 4);
  memory_copy(buf, &x, 4);
  Str8 *str = &srl->last->str;
  if(str->s + str->len == buf) {
    srl->last->str.len += 4;
    srl->total_len += 4;
  } else {
    str8_list_append_str_(arena, srl, (Str8){ buf, 4 });
  }
}

func void str8_serial_push_u16(Arena *arena, Str8_list *srl, u16 x) {
  str8_serial_push_data(arena, srl, &x, sizeof(x));
}

func void str8_serial_push_u8(Arena *arena, Str8_list *srl, u8 x) {
  str8_serial_push_data(arena, srl, &x, sizeof(x));
}

func void str8_serial_push_cstr(Arena *arena, Str8_list *srl, Str8 str) {
  str8_serial_push_data(arena, srl, str.s, str.len);
  str8_serial_push_u8(arena, srl, 0);
}

func void str8_serial_push_str(Arena *arena, Str8_list *srl, Str8 str) {
  str8_serial_push_data(arena, srl, str.s, str.len);
}

////////////////////////////////
//~ rjf: Deserialization Helpers

func u64 str8_deserial_read(Str8 str, u64 off, void *read_dst, u64 read_len, u64 granularity) {
  u64 bytes_left = str.len-MIN(off, str.len);
  u64 actually_readable_len = MIN(bytes_left, read_len);
  u64 legally_readable_len = actually_readable_len - actually_readable_len%granularity;
  if(legally_readable_len > 0) {
    memory_copy(read_dst, str.s+off, legally_readable_len);
  }
  return legally_readable_len;
}

func u64 str8_deserial_find_first_match(Str8 str, u64 off, u16 scan_val) {
  u64 cursor = off;
  for(;;) {
    u16 val = 0;
    str8_deserial_read_struct(str, cursor, &val);
    if(val == scan_val) {
      break;
    }
    cursor += sizeof(val);
  }
  return cursor;
}

func void* str8_deserial_get_raw_ptr(Str8 str, u64 off, u64 len) {
  void *raw_ptr = 0;
  if(off + len <= str.len) {
    raw_ptr = str.s + off;
  }
  return raw_ptr;
}

func u64 str8_deserial_read_cstr(Str8 str, u64 off, Str8 *cstr_out) {
  u64 cstr_len = 0;
  if(off < str.len) {
    u8 *ptr = str.s + off;
    u8 *cap = str.s + str.len;
    *cstr_out = str8_cstr_capped(ptr, cap);
    cstr_len = (cstr_out->len + 1);
  }
  return cstr_len;
}

func u64 str8_deserial_read_windows_utf16_str16(Str8 str, u64 off, Str16 *str_out) {
  u64 null_off = str8_deserial_find_first_match(str, off, 0);
  u64 len = null_off - off;
  u16 *s = (u16 *)str8_deserial_get_raw_ptr(str, off, len);
  u64 count = len / sizeof(*s);
  *str_out = (Str16){ s, count };
  
  u64 read_len_with_null = len + sizeof(*s);
  return read_len_with_null;
}

func u64 str8_deserial_read_block(Str8 str, u64 off, u64 len, Str8 *block_out) {
  *block_out = str8_slice(str, off, off + len);
  return block_out->len;
}
