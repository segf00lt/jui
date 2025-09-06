#include "base.h"

#include "lexer.h"
#include "lexer.c"

#include "type_info.c"

//#include <pthread.h>

void* do_a_thing(void *arg);
Type_info* gen_type_info_from_struct(Arena *arena, Clexer *lexer);
Type_member* gen_type_info_from_struct_member(Arena *arena, Clexer *lexer);
Type_info* gen_type_info_from_type(Arena *arena, Clexer *lexer);

OS_handle thread_ids[2];

func void* do_a_thing(void *arg) {
  u64 i = 0;
  pthread_t id = pthread_self();

  printf("hello there I am thread %lu\n", (u64)id);

  for(i = 0; i < (0xffffffff); i++);

  return 0;
}

// TODO
// [ ] parse arbitrary C pointers and arrays
// [ ] parse numeric literals
// [ ] parse strings and chars
func Type_info* gen_type_info_from_struct(Arena *arena, Clexer *lexer) {
  Type_info *result = 0;

  Ctoken token;

  u64 save_arena_pos = arena_pos(arena);

  token = lex_ctoken(lexer);

  if(token.kind != CTOKEN_STRUCT) {
    return 0;
  }

  token = lex_ctoken(lexer);

  ASSERT(token.kind == CTOKEN_IDENT);

  result = push_struct(arena, Type_info);
  result->kind = TYPE_KIND_STRUCT;
  result->name = token.str;

  token = lex_ctoken(lexer);

  ASSERT(token.kind == '{');

  u64 offset = 0;

  Arena_scope scratch = scratch_begin(&arena, 1);

  Arr(Type_member*) member_array;
  arr_init_ex(member_array, scratch.arena, 128);

  for(;;) {
    Type_member *member = gen_type_info_from_struct_member(scratch.arena, lexer);
    member->offset = ALIGN_UP(offset, member->type->align);
    offset += member->type->bytes;
    arr_push(member_array, member);

    Clexer save_lexer = *lexer;
    if(lex_ctoken(lexer).kind == '}') {
      break;
    } else {
      *lexer = save_lexer;
    }
  }

  result->members = push_array(arena, Type_member, member_array.count);
  result->member_or_array_count = member_array.count;

  for(int i = 0; i < member_array.count; i++) {
    memory_copy_struct(&result->members[i], member_array.d[i]);
  }

  scratch_end(scratch);

  return result;
}

func Type_member* gen_type_info_from_struct_member(Arena *arena, Clexer *lexer) {
  Type_member *result = 0;

  Ctoken token;

  u64 save_arena_pos = arena_pos(arena);

  Clexer save_lexer = *lexer;
  token = lex_ctoken(lexer);

  if(CTOKEN_U8 <= token.kind && token.kind <= CTOKEN_VOID) {
    result = push_struct(arena, Type_member);

    Type_info *first = push_struct(arena, Type_info);

    switch(token.kind) {
      case CTOKEN_U8:
        {
          first->kind = TYPE_KIND_U8;
          first->bytes = sizeof(u8);
          first->align = alignof(u8);
        } break;
      case CTOKEN_U16:
        {
          first->kind = TYPE_KIND_U16;
          first->bytes = sizeof(u16);
          first->align = alignof(u16);
        } break;
      case CTOKEN_U32:
        {
          first->kind = TYPE_KIND_U32;
          first->bytes = sizeof(u32);
          first->align = alignof(u32);
        } break;
      case CTOKEN_U64:
        {
          first->kind = TYPE_KIND_U64;
          first->bytes = sizeof(u64);
          first->align = alignof(u64);
        } break;
      case CTOKEN_CHAR:
      case CTOKEN_S8:
        {
          first->kind = TYPE_KIND_S8;
          first->bytes = sizeof(s8);
          first->align = alignof(s8);
        } break;
      case CTOKEN_SHORT:
      case CTOKEN_S16:
        {
          first->kind = TYPE_KIND_S16;
          first->bytes = sizeof(s16);
          first->align = alignof(s16);
        } break;
      case CTOKEN_INT:
      case CTOKEN_S32:
        {
          first->kind = TYPE_KIND_S32;
          first->bytes = sizeof(s32);
          first->align = alignof(s32);
        } break;
      case CTOKEN_LONG:
      case CTOKEN_S64:
        {
          first->kind = TYPE_KIND_S64;
          first->bytes = sizeof(s64);
          first->align = alignof(s64);
        } break;
      case CTOKEN_BOOL:
      case CTOKEN_B8:
        {
          first->kind = TYPE_KIND_B8;
          first->bytes = sizeof(b8);
          first->align = alignof(b8);
        } break;
      case CTOKEN_B16:
        {
          first->kind = TYPE_KIND_B16;
          first->bytes = sizeof(b16);
          first->align = alignof(b16);
        } break;
      case CTOKEN_B32:
        {
          first->kind = TYPE_KIND_B32;
          first->bytes = sizeof(b32);
          first->align = alignof(b32);
        } break;
      case CTOKEN_B64:
        {
          first->kind = TYPE_KIND_B64;
          first->bytes = sizeof(b64);
          first->align = alignof(b64);
        } break;
      case CTOKEN_FLOAT:
      case CTOKEN_F32:
        {
          first->kind = TYPE_KIND_F32;
          first->bytes = sizeof(f32);
          first->align = alignof(f32);
        } break;
      case CTOKEN_DOUBLE:
      case CTOKEN_F64:
        {
          first->kind = TYPE_KIND_F64;
          first->bytes = sizeof(f64);
          first->align = alignof(f64);
        } break;
      case CTOKEN_UNSIGNED:
        {
          UNIMPLEMENTED;
        } break;
      case CTOKEN_SIGNED:
        {
          UNIMPLEMENTED;
        } break;
      case CTOKEN_VOID:
        {
          first->kind = TYPE_KIND_VOID;
          first->bytes = 0;
          first->align = 0;
        } break;
    }

    token = lex_ctoken(lexer);


    Type_info *pointer_chain_first = 0;
    if(token.kind == '*') {
      pointer_chain_first = push_struct(arena, Type_info);
      pointer_chain_first->kind = TYPE_KIND_PTR;
      pointer_chain_first->bytes = sizeof(void*);
      pointer_chain_first->align = alignof(void*);
      Type_info *pointer_chain_last = pointer_chain_first;
      pointer_chain_last->child = first;

      token = lex_ctoken(lexer);
      while(token.kind == '*') {
        Type_info *p = push_struct(arena, Type_info);
        p->kind = TYPE_KIND_PTR;
        p->bytes = sizeof(void*);
        p->align = alignof(void*);
        sll_queue_push_front_n(pointer_chain_first, pointer_chain_last, p, child);
        token = lex_ctoken(lexer);
      }
    }
    Type_info *member_type = pointer_chain_first;
    if(!member_type) {
      member_type = first;
    }

    ASSERT(token.kind == CTOKEN_IDENT);

    result->name = token.str;

    token = lex_ctoken(lexer);

#if 0
    Type_info array_chain_begin = {0};
    Type_info *array = &array_chain_begin;
    if(token.kind == '[') {
    }
#endif

    ASSERT(token.kind == ';');

    result->type = member_type;

  } else {
    UNREACHABLE;
    return 0;
  }

  return result;
}

int main(void) {
  void *os_state = os_state_alloc();
  os_state_select(os_state);

  Arena_scope scratch = scratch_begin(0, 0);

  printf("arena %p pos: %lu\n", scratch.arena, arena_pos(scratch.arena));

  //printf("hello %p\n", os_state);


  //for(int i = 0; i < 2; i++) {
  //  thread_ids[i]  = os_thread_launch((OS_thread_func*)&do_a_thing, 0, 0);
  //  printf("thread succesfully created\n");
  //}

  //os_thread_join(thread_ids[0], 0);
  //os_thread_join(thread_ids[1], 0);

  Str8 test_src = os_read_entire_file(scratch.arena, str8_lit("test_lexer_2.c"));

  {
    Clexer lexer =
    {
      .flags = CLEXER_FLAG_SKIP_COMMENTS,
      .arena = scratch.arena,
      .src = test_src,
    };

    Ctoken_slice tokens = lex_ctoken_all(scratch.arena, test_src, 0); 

    printf("arena %p pos: %lu\n", scratch.arena, arena_pos(scratch.arena));

    for(int i = 0; i < tokens.count; i++) {
      Ctoken t = tokens.d[i];
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
    }

    lexer.pos = lexer.cur_col = lexer.cur_line = 0;
    Type_info *tinfo = gen_type_info_from_struct(scratch.arena, &lexer);

    if(tinfo) {
      printf("type name: %.*s\n", (int)tinfo->name.len, tinfo->name.s);
      for(int i = 0; i < tinfo->member_or_array_count; i++) {
        printf("member name: %.*s\nmember offset: %lu\n", (int)tinfo->members[i].name.len, tinfo->members[i].name.s, tinfo->members[i].offset);
      }
    }

  }

  scratch_end(scratch);

  return 0;
}
