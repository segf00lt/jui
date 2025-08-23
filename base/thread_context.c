
thread_static Tctx* tctx_thread_local;
//#if !BUILD_SUPPLEMENTARY_UNIT
//C_LINKAGE thread_static Tctx* tctx_thread_local = 0;
//#endif

////////////////////////////////
//~ rjf: Thread Context Functions

//- rjf: thread-context allocation & selection

func Tctx* tctx_alloc(void) {
  Arena *arena = arena_alloc(KB(64));
  Tctx *tctx = push_struct(arena, Tctx);
  tctx->arenas[0] = arena;
  tctx->arenas[1] = arena_alloc(KB(64));
  tctx->lane_ctx.lane_count = 1;
  return tctx;
}

func void tctx_free(Tctx *tctx) {
  arena_free(tctx->arenas[1]);
  arena_free(tctx->arenas[0]);
}

func void tctx_select(Tctx *tctx) {
  tctx_thread_local = tctx;
}

func Tctx * tctx_selected(void) {
  return tctx_thread_local;
}

//- rjf: scratch arenas

func Arena* tctx_get_scratch(Arena **conflicts, u64 count) {
  Tctx *tctx = tctx_selected();
  Arena *result = 0;
  Arena **arena_ptr = tctx->arenas;
  for(u64 i = 0; i < ARRLEN(tctx->arenas); i += 1, arena_ptr += 1) {
    Arena **conflict_ptr = conflicts;
    b32 has_conflict = 0;
    for(u64 j = 0; j < count; j += 1, conflict_ptr += 1) {
      if(*arena_ptr == *conflict_ptr) {
        has_conflict = 1;
        break;
      }
    }
    if(!has_conflict) {
      result = *arena_ptr;
      break;
    }
  }
  return result;
}

//- rjf: lane metadata

#if 0
func void tctx_set_lane_ctx(Lane_ctx lane_ctx) {
  Tctx *tctx = tctx_selected();
  tctx->lane_ctx = lane_ctx;
}

func void tctx_lane_barrier_wait(void) {
  ProfBeginFunction();
  ProfColor(0x00000ff);
  Tctx *tctx = tctx_selected();
  os_barrier_wait(tctx->lane_ctx.barrier);
  ProfEnd();
}

func Rng1u64 tctx_lane_idx_range_from_count(u64 count) {
  u64 main_idxes_per_lane = count/lane_count();
  u64 leftover_idxes_count = count - main_idxes_per_lane*lane_count();
  u64 leftover_idxes_before_this_lane_count = Min(lane_idx(), leftover_idxes_count);
  u64 lane_base_idx = lane_idx()*main_idxes_per_lane + leftover_idxes_before_this_lane_count;
  u64 lane_base_idx__clamped = Min(lane_base_idx, count);
  u64 lane_opl_idx = lane_base_idx__clamped + main_idxes_per_lane + ((lane_idx() < leftover_idxes_count) ? 1 : 0);
  u64 lane_opl_idx__clamped = Min(lane_opl_idx, count);
  Rng1u64 result = r1u64(lane_base_idx__clamped, lane_opl_idx__clamped);
  return result;
}
#endif

//- rjf: thread names

func void tctx_set_thread_name(Str8 string) {
  Tctx *tctx = tctx_selected();
  u64 size = CLAMP_TOP(string.len, sizeof(tctx->thread_name));
  memory_copy(tctx->thread_name, string.s, size);
  tctx->thread_name_size = size;
}

func Str8 tctx_get_thread_name(void) {
  Tctx *tctx = tctx_selected();
  Str8 result = { .s = tctx->thread_name, .len = tctx->thread_name_size };
  return result;
}

//- rjf: thread source-locations

func void tctx_write_srcloc(char *file_name, u64 line_number) {
  Tctx *tctx = tctx_selected();
  tctx->file_name = file_name;
  tctx->line_number = line_number;
}

func void tctx_read_srcloc(char **file_name, u64 *line_number) {
  Tctx *tctx = tctx_selected();
  *file_name = tctx->file_name;
  *line_number = tctx->line_number;
}
