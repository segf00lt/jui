

func Ctoken_slice lex_ctoken_old(Arena *arena, Str8 src) {
  Ctoken_slice token_slice = {0};

  Arena_scope scratch = scratch_begin(&arena, 1);

  Ctoken_arr token_arr;
  arr_init_ex(token_arr, scratch.arena, 4096);

  s64 cur_col = 0;
  s64 cur_line = 0;

  for(s64 pos = 0; pos < src.len;) {

    Str8 pos_str = str8_slice(src, pos, src.len);

    if(str8_starts_with(pos_str, ctoken_begin_comment_single_line)) {
      s64 end_line = str8_find_char(pos_str, '\n');
      Str8 comment_str = str8_slice(pos_str, 0, end_line + 1);

      Ctoken token =
      {
        .kind = CTOKEN_COMMENT_SINGLE_LINE,
        .str = comment_str,
        .line = cur_line,
        .col = cur_col,
        .comment_end_line = cur_line,
        .comment_end_col = cur_col + comment_str.len,
      };

      arr_push(token_arr, token);

      cur_col = 0;
      cur_line += 1;

      pos += end_line;
      pos++;

      goto continue_loop;
    }

    if(str8_starts_with(pos_str, ctoken_begin_comment_multi_line)) {
      s64 ctoken_end_comment = str8_find(pos_str, ctoken_end_comment_multi_line);
      ctoken_end_comment += ctoken_end_comment_multi_line.len;

      Str8 comment_str = str8_slice(pos_str, 0, ctoken_end_comment);

      // NOTE this is going to test the thread context code
      Str8_find_results new_lines = str8_find_all_chars(comment_str, '\n', scratch.arena);

      s64 comment_end_line = cur_line + new_lines.count;
      s64 comment_end_col;
      if(new_lines.count > 0) {
        s64 last_new_line_pos = new_lines.end_indexes[new_lines.count-1];
        comment_end_col = cur_col + last_new_line_pos; 
      } else {
        comment_end_col = cur_col + comment_str.len; 
      }

      Ctoken token =
      {
        .kind = CTOKEN_COMMENT_MULTI_LINE,
        .str = comment_str,
        .line = cur_line,
        .col = cur_col,
        .comment_end_line = comment_end_line,
        .comment_end_col = comment_end_col,
      };

      arr_push(token_arr, token);

      cur_line = comment_end_line;
      cur_col = comment_end_col;

      pos += ctoken_end_comment;
      pos++;

      goto continue_loop;
    }

    if(is_space(pos_str.s[0])) {
      goto skip_character;
    }

    for(int i = 0; i < ARRLEN(ctoken_keywords); i++) {
      Str8 keyword = ctoken_keywords[i];

      if(str8_starts_with(pos_str, keyword)) {
        u8 end_char = pos_str.s[keyword.len];
        b8 matched =
          (!is_alpha(end_char) && !is_decimal(end_char) && end_char != '_');

        if(matched) {
          Ctoken token =
          {
            .kind = i+CTOKEN_KEYWORDS_BEGIN+1,
            .str = { .s = pos_str.s, .len = keyword.len },
            .line = cur_line,
            .col = cur_col,
          };
          arr_push(token_arr, token);

          cur_col += keyword.len;
          pos += keyword.len;

          goto continue_loop;
        }

      }

    }

    for(int i = 0; i < ARRLEN(ctoken_operators); i++) {
      Str8 operator = ctoken_operators[i];

      if(str8_starts_with(pos_str, operator)) {
        Ctoken token =
        {
          .kind = i+CTOKEN_KEYWORDS_BEGIN+1,
          .str = { .s = pos_str.s, .len = operator.len },
          .line = cur_line,
          .col = cur_col,
        };
        arr_push(token_arr, token);

        cur_col += operator.len;
        pos += operator.len;

        goto continue_loop;
      }

    }

    for(int i = 0; i < ctoken_chars.len; i++) {
      if(pos_str.s[0] == ctoken_chars.s[i]) {
        Ctoken token =
        {
          .kind = ctoken_chars.s[i],
          .str = { .s = pos_str.s, .len = 1 },
          .line = cur_line,
          .col = cur_col,
        };
        arr_push(token_arr, token);

        cur_col++;
        pos++;

        goto continue_loop;
      }
    }

    if(pos_str.s[0] == '_' || is_alpha(pos_str.s[0])) {

      int end_ident = 1;
      for(; end_ident < pos_str.len; end_ident++) {
        u8 end_char = pos_str.s[end_ident];
        if(!is_alpha(end_char) && !is_decimal(end_char) && end_char != '_') {
          break;
        }
      }

      Ctoken token =
      {
        .kind = CTOKEN_IDENT,
        .str = { .s = pos_str.s, .len = end_ident },
        .line = cur_line,
        .col = cur_col,
      };
      arr_push(token_arr, token);

      cur_col += end_ident;
      pos += end_ident;

      goto continue_loop;
    }

skip_character:
    if(pos_str.s[0] == '\n') {
      cur_col = 0;
      cur_line++;
    } else {
      cur_col++;
    }
    pos++;
    continue;

continue_loop:
    ;

  }

  token_slice.d = push_array_no_zero(arena, Ctoken, token_arr.count);
  token_slice.count = token_arr.count;
  memory_copy_typed(token_slice.d, token_arr.d, token_arr.count);

  printf("arena %p pos: %lu\n", scratch.arena, arena_pos(scratch.arena));

  scratch_end(scratch);

  printf("arena %p pos: %lu\n", scratch.arena, arena_pos(scratch.arena));

  return token_slice;
}

func Ctoken_slice lex_ctoken_all(Arena *arena, Str8 src, Clexer_flags flags) {
  Ctoken_slice tokens = {0};

  Clexer lexer = {0};
  lexer.flags = flags;
  lexer.src = src;
  lexer.arena = arena;

  Arena_scope scratch = scratch_begin(&arena, 1);

  Ctoken_arr token_arr = {0};
  arr_init_ex(token_arr, arena, 4096);

  for(; lexer.pos < lexer.src.len; ) {
    Ctoken token = lex_ctoken(&lexer);
#if 0
    Ctoken t = token;
      printf(
          "kind: %i\n"
          "str: %.*s\n"
          "line: %i\n"
          "col: %i\n"
          "comment_end_line: %i\n"
          "comment_end_col: %i\n\n",
          t.kind,
          (int)t.str.len, t.str.s,
          t.line,
          t.col,
          t.comment_end_line,
          t.comment_end_col);
#endif
    if(token.kind != 0) {
      arr_push(token_arr, token);
    }
  }

  tokens.d = push_array_no_zero(arena, Ctoken, token_arr.count);
  tokens.count = token_arr.count;
  memory_copy_typed(tokens.d, token_arr.d, tokens.count);

  scratch_end(scratch);

  return tokens;
}

func Ctoken lex_ctoken(Clexer *lexer) {
  Ctoken token = {0};

  if(lexer->pos >= lexer->src.len) {
    return token;
  }

  Arena_scope scratch = scratch_begin(&(lexer->arena), 1);

begin:

  while(is_space(lexer->src.s[lexer->pos])) {
    if(lexer->src.s[lexer->pos] == '\n') {
      lexer->cur_col = 0;
      lexer->cur_line++;
    } else {
      lexer->cur_col++;
    }
    lexer->pos++;
  }

  Str8 pos_str = str8_slice(lexer->src, lexer->pos, lexer->src.len);

  { /* comments */
    if(str8_starts_with(pos_str, ctoken_begin_comment_single_line)) {
      s64 end_line = str8_find_char(pos_str, '\n');

      if(lexer->flags & CLEXER_FLAG_SKIP_SINGLE_LINE_COMMENTS) {
        lexer->cur_col = 0;
        lexer->cur_line += 1;
        lexer->pos += end_line;
        lexer->pos++;
        goto begin;
      }

      Str8 comment_str = str8_slice(pos_str, 0, end_line + 1);

      token =
        (Ctoken) {
          .kind = CTOKEN_COMMENT_SINGLE_LINE,
          .str = comment_str,
          .line = lexer->cur_line,
          .col = lexer->cur_col,
          .comment_end_line = lexer->cur_line,
          .comment_end_col = lexer->cur_col + comment_str.len,
        };

      lexer->cur_col = 0;
      lexer->cur_line += 1;

      lexer->pos += end_line;
      lexer->pos++;

      goto end;
    }

    if(str8_starts_with(pos_str, ctoken_begin_comment_multi_line)) {
      s64 ctoken_end_comment = str8_find(pos_str, ctoken_end_comment_multi_line);
      ctoken_end_comment += ctoken_end_comment_multi_line.len;

      Str8 comment_str = str8_slice(pos_str, 0, ctoken_end_comment);

      // NOTE this is going to test the thread context code
      Str8_find_results new_lines = str8_find_all_chars(comment_str, '\n', scratch.arena);

      s64 comment_end_line = lexer->cur_line + new_lines.count;
      s64 comment_end_col;
      if(new_lines.count > 0) {
        s64 last_new_line_pos = new_lines.end_indexes[new_lines.count-1];
        comment_end_col = lexer->cur_col + last_new_line_pos; 
      } else {
        comment_end_col = lexer->cur_col + comment_str.len; 
      }

      if(lexer->flags & CLEXER_FLAG_SKIP_MULTI_LINE_COMMENTS) {
        lexer->cur_line = comment_end_line;
        lexer->cur_col = comment_end_col;
        lexer->pos += ctoken_end_comment;
        goto begin;
      }

      token =
        (Ctoken ) {
          .kind = CTOKEN_COMMENT_MULTI_LINE,
          .str = comment_str,
          .line = lexer->cur_line,
          .col = lexer->cur_col,
          .comment_end_line = comment_end_line,
          .comment_end_col = comment_end_col,
        };

      lexer->cur_line = comment_end_line;
      lexer->cur_col = comment_end_col;

      lexer->pos += ctoken_end_comment;

      goto end;
    }
  } /* comments */

  for(int i = 0; i < ARRLEN(ctoken_keywords); i++) {
    Str8 keyword = ctoken_keywords[i];

    if(str8_starts_with(pos_str, keyword)) {
      u8 end_char = pos_str.s[keyword.len];
      b8 matched =
        (!is_alpha(end_char) && !is_decimal(end_char) && end_char != '_');

      if(matched) {
        token =
          (Ctoken) {
            .kind = i+CTOKEN_KEYWORDS_BEGIN+1,
            .str = { .s = pos_str.s, .len = keyword.len },
            .line = lexer->cur_line,
            .col = lexer->cur_col,
          };

        lexer->cur_col += keyword.len;
        lexer->pos += keyword.len;

        goto end;
      }

    }

  }

  for(int i = 0; i < ARRLEN(ctoken_operators); i++) {
    Str8 operator = ctoken_operators[i];
    if(operator.len == 0) continue;

    if(str8_starts_with(pos_str, operator)) {
      token =
        (Ctoken) {
          .kind = i+CTOKEN_KEYWORDS_BEGIN+1,
          .str = { .s = pos_str.s, .len = operator.len },
          .line = lexer->cur_line,
          .col = lexer->cur_col,
        };

      lexer->cur_col += operator.len;
      lexer->pos += operator.len;

      goto end;
    }

  }

  for(int i = 0; i < ctoken_chars.len; i++) {
    if(pos_str.s[0] == ctoken_chars.s[i]) {
      token =
        (Ctoken) {
          .kind = ctoken_chars.s[i],
          .str = { .s = pos_str.s, .len = 1 },
          .line = lexer->cur_line,
          .col = lexer->cur_col,
        };

      lexer->cur_col++;
      lexer->pos++;

      goto end;
    }
  }

  if(pos_str.s[0] == '_' || is_alpha(pos_str.s[0])) {

    int end_ident = 1;
    for(; end_ident < pos_str.len; end_ident++) {
      u8 end_char = pos_str.s[end_ident];
      if(!is_alpha(end_char) && !is_decimal(end_char) && end_char != '_') {
        break;
      }
    }

    token =
      (Ctoken) {
        .kind = CTOKEN_IDENT,
        .str = { .s = pos_str.s, .len = end_ident },
        .line = lexer->cur_line,
        .col = lexer->cur_col,
      };

    lexer->cur_col += end_ident;
    lexer->pos += end_ident;

    goto end;
  }

end:

  scratch_end(scratch);

  return token;
}
