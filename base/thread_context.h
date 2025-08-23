#ifndef _THREAD_CONTEXT_H
#define _THREAD_CONTEXT_H

////////////////////////////////
//~ rjf: Lane Group Context

typedef struct Lane_ctx Lane_ctx;
struct Lane_ctx {
  u64 lane_idx;
  u64 lane_count;
  //Barrier barrier;
};

////////////////////////////////
//~ rjf: Base Per-Thread State Bundle

typedef struct Tctx Tctx;
struct Tctx {
  // rjf: scratch arenas
  Arena *arenas[2];
  
  // rjf: thread name
  u8 thread_name[32];
  u64 thread_name_size;
  
  // rjf: lane context
  Lane_ctx lane_ctx;
  
  // rjf: source location info
  char *file_name;
  u64 line_number;
};

////////////////////////////////
//~ rjf: Thread Context Functions

//- rjf: thread-context allocation & selection
Tctx *tctx_alloc(void);
void tctx_free(Tctx *tctx);
void tctx_select(Tctx *tctx);
Tctx *tctx_selected(void);

//- rjf: scratch arenas
Arena *tctx_get_scratch(Arena **conflicts, u64 count);
#define scratch_begin(conflicts, count) arena_scope_begin(tctx_get_scratch((conflicts), (count)))
#define scratch_end(scratch) arena_scope_end(scratch)

//- rjf: lane metadata
#if 0
void tctx_set_lane_ctx(Lane_ctx lane_ctx);
void tctx_lane_barrier_wait(void);
Rng1u64 tctx_lane_idx_range_from_count(u64 count);
#define lane_idx() (tctx_selected()->lane_ctx.lane_idx)
#define lane_count() (tctx_selected()->lane_ctx.lane_count)
#define lane_from_task_idx(idx) ((idx)%lane_count())
#define lane_ctx(ctx) tctx_set_lane_ctx((ctx))
#define lane_sync() tctx_lane_barrier_wait()
#define lane_range(count) tctx_lane_idx_range_from_count(count)
#endif

//- rjf: thread names
void tctx_set_thread_name(Str8 name);
Str8 tctx_get_thread_name(void);

//- rjf: thread source-locations
void tctx_write_srcloc(char *file_name, u64 line_number);
void tctx_read_srcloc(char **file_name, u64 *line_number);
#define tctx_write_this_srcloc() tctx_write_srcloc(__FILE__, __LINE__)

#endif // BASE_THREAD_CONTEXT_H
