#include "base.h"

/*
 * tables
 */

#define KEYWORDS_TO_GENERATE_HEADERS_FOR \
  X(FUNC, func) \
  X(STRUCT, struct) \
  X(UNION, union) \

/*
 * types
 */

typedef enum Keyword_kind {
  KEYWORD_INVALID = -1,
#define X(upper, lower) KEYWORD_##upper,
  KEYWORDS_TO_GENERATE_HEADERS_FOR
#undef X
    KEYWORD_MAX,
} Keyword_kind;

Str8 Keyword_kind_str8[KEYWORD_MAX] = {
#define X(upper, lower) str8_lit(#lower),
  KEYWORDS_TO_GENERATE_HEADERS_FOR
#undef X
};


typedef struct Generated_headers Generated_headers;
struct Generated_headers {
  Str8 file_name;
  Str8 unions;
  Str8 structs;
  Str8 functions;
};


DECL_ARR_TYPE(Generated_headers);

/*
 * globals
 */

Arena *main_arena;
Arena *temp_arena;

/*
 * functions
 */

func Generated_headers generate_headers_from_c_source_file(Arena *arena, Str8 path) {

  Generated_headers result = {0};

  Str8 headers_per_keyword[KEYWORD_MAX] = {0};

  Arena_scope scope = arena_scope_begin(arena);

  Str8 src = os_read_entire_file(arena, path);

  for(Keyword_kind keyword_kind = 0; keyword_kind < KEYWORD_MAX; keyword_kind++) {

    Str8 keyword = Keyword_kind_str8[keyword_kind];
    Str8_list headers_list = {0};

    for(s64 i = 0, prev_char = 0; i < src.len; prev_char = src.s[i], i++) {

      if(src.s[i] == '{') {
        while(src.s[i] != '}') i++;
        i++;
      }

      //if(!str8_find(str8_slice(src, i, src.len), str8_lit("#define"))) {
      //  while(src.s[i] != '\n') i++;
      //  i++;
      //}

      if(str8_starts_with(str8_slice(src, i, src.len), str8_lit("/*"))) {
        while(src.s[i] != '*' || src.s[i+1] != '/') i++;
        i++;
        i++;
      }

      if(str8_starts_with(str8_slice(src, i, src.len), str8_lit("//"))) {
        while(src.s[i] != '\n') i++;
        i++;
      }

      if(str8_starts_with(str8_slice(src, i, src.len), str8_lit("typedef"))) {
        while(src.s[i] != ';') i++;
        i++;
      }

      if(src.s[i] != keyword.s[0]) continue;

      Str8 token = str8_slice(src, i, i+keyword.len);

      if(is_space(prev_char) && is_space(token.s[token.len]) && str8_match(keyword, token)) {

        Str8 search_str = str8_slice(src, i + keyword.len, src.len);
        s64 index = str8_find(search_str, str8_lit("{"));

        if(index >= 0) {
          search_str.len = index;
          Str8 stripped_search_str = str8_strip_whitespace(search_str);

          Str8 header = {0};

          if(stripped_search_str.len > 0) {
            switch(keyword_kind) {
              case KEYWORD_FUNC:
                {
                  header = str8_cat(arena, stripped_search_str, str8_lit(";\n"));
                } break;
              case KEYWORD_STRUCT:
                {
                  header = str8f(arena, "typedef struct %S %S;\n", stripped_search_str, stripped_search_str);
                } break;
              case KEYWORD_UNION:
                {
                  header = str8f(arena, "typedef union %S %S;\n", stripped_search_str, stripped_search_str);
                } break;
            }

            str8_list_append_str(arena, headers_list, header);
          }

          s64 index_in_src = i + keyword.len + index;

          s64 curly_braces = 1;
          s64 j = index_in_src + 1;

          for(; j < src.len && curly_braces > 0; j++) {
            switch(src.s[j]) {
              case '{':
                curly_braces += 1;
                break;
              case '}':
                curly_braces -= 1;
                break;
            }
          }

          while(is_space(src.s[j])) j++;

          if(src.s[j] == ';') {
            j++;
          }

          i = j - 1;

        }

      } /* if(is_space(prev_char) && is_space(token.s[token.len]) && str8_match(keyword, token)) */

    }

    Str8 headers_str = str8_list_join(arena, headers_list, str8_lit(""));

    headers_per_keyword[keyword_kind] =  headers_str;

  }

  arena_scope_end(scope);

  result.file_name = str8_copy(arena, os_file_name_from_path(path));
  result.unions = str8_copy(arena, headers_per_keyword[KEYWORD_UNION]);
  result.structs = str8_copy(arena, headers_per_keyword[KEYWORD_STRUCT]);
  result.functions = str8_copy(arena, headers_per_keyword[KEYWORD_FUNC]);

  return result;
}

int main(void) {

  main_arena = arena_alloc(MB(500));
  temp_arena = arena_alloc(MB(250));

  Str8 cur_dir = os_get_current_dir(main_arena);

  OS_file_iter *file_iter = os_file_iter_begin(main_arena, cur_dir, OS_FILE_ITER_SKIP_DIRS | OS_FILE_ITER_SKIP_HIDDEN_FILES);

  OS_file_info file_info = {0};

  Arr_Generated_headers header_arr = {0};
  arr_init_ex(header_arr, main_arena, 128);

  for(; os_file_iter_next(main_arena, file_iter,  &file_info); ) {

    if(!str8_ends_with(file_info.name, str8_lit(".c"))) {
      continue;
    }

    if(str8_match_lit(__FILE__, file_info.name)) {
      continue;
    }

    arena_scope(temp_arena) {
      Generated_headers headers = generate_headers_from_c_source_file(main_arena, str8f(temp_arena, "%S/%S", cur_dir, file_info.name));
      arr_push(header_arr, headers);
    }

  }

  Str8_list strlist = {0};

  str8_list_append_str(temp_arena, strlist,
      str8_lit("///////////////////////////////\n// unions\n\n"));

  for(int i = 0; i < header_arr.count; i++) {
    Generated_headers headers = header_arr.d[i];

    str8_list_append_str(temp_arena, strlist,
        str8f(temp_arena,
          "/*\n"
          " * %S\n"
          "*/\n\n"
          "%S\n",
          headers.file_name, headers.unions));

  }

  str8_list_append_str(temp_arena, strlist,
      str8_lit("///////////////////////////////\n// structs\n\n"));

  for(int i = 0; i < header_arr.count; i++) {
    Generated_headers headers = header_arr.d[i];

    str8_list_append_str(temp_arena, strlist,
        str8f(temp_arena,
          "/*\n"
          " * %S\n"
          "*/\n\n"
          "%S\n",
          headers.file_name, headers.structs));

  }

  str8_list_append_str(temp_arena, strlist,
      str8_lit("///////////////////////////////\n// function headers\n\n"));

  for(int i = 0; i < header_arr.count; i++) {
    Generated_headers headers = header_arr.d[i];

    str8_list_append_str(temp_arena, strlist,
        str8f(temp_arena,
          "/*\n"
          " * %S\n"
          "*/\n\n"
          "%S\n",
          headers.file_name, headers.functions));

  }

  Str8 dump = str8_list_join(temp_arena, strlist, str8_lit(""));

  arena_clear(temp_arena);
  printf("%s", cstr_copy_str8(temp_arena, dump));

  return 0;
}
