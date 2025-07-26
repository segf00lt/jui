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
