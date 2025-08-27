#include "base.h"

#include "lexer.h"
#include "lexer.c"

#include <pthread.h>


OS_handle thread_ids[2];

func void* do_a_thing(void *arg) {
  u64 i = 0;
  pthread_t id = pthread_self();

  printf("hello there I am thread %lu\n", (u64)id);

  for(i = 0; i < (0xffffffff); i++);

  return 0;
}

typedef struct Foo Foo;
struct Foo {
  int a;
  float b;
  char c;
};

typedef struct Type_info Type_info;
struct Type_info {
  // TODO
};

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

  Str8 test_src = os_read_entire_file(scratch.arena, str8_lit("test_lexer.c"));

  Ctoken_slice tokens = lex_ctoken(scratch.arena, test_src); 

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

  scratch_end(scratch);

  return 0;
}
