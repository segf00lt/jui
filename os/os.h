#ifndef OS_H
#define OS_H

typedef struct OS_handle OS_handle;
struct OS_handle {
  u64 hand[1];
};

typedef struct OS_system_info OS_system_info;
struct OS_system_info {
  u32 logical_processor_count;
  u64 page_size;
  u64 large_page_size;
  u64 allocation_granularity;
  Str8 machine_name;
};

typedef struct OS_process_info OS_process_info;
struct OS_process_info {
  u32 pid;
  b32 large_pages_allowed;
  Str8 binary_path;
  Str8 initial_path;
  Str8 user_program_data_path;
  Str8_list module_load_paths;
  Str8_list environment;
};

typedef u32 OS_file_iter_flags; 
enum {
  OS_FILE_ITER_SKIP_DIRS         = (1 << 0),
  OS_FILE_ITER_SKIP_FILES        = (1 << 1),
  OS_FILE_ITER_SKIP_HIDDEN_FILES = (1 << 2),
  OS_FILE_ITER_DONE              = (1 << 31),

};

typedef struct OS_file_iter OS_file_iter;
struct OS_file_iter {
  OS_file_iter_flags flags;
  u8 memory[800];
};

typedef struct OS_file_info OS_file_info;
struct OS_file_info {
  Str8 name;
  File_properties props;
};

typedef void* OS_thread_func(void*);

OS_handle os_handle_zero = {0};

void os_init(void);

void* os_state_alloc(void);
void  os_state_free(void *state);
void* os_state_get_selected(void);
void  os_state_select(void *state);

void* os_alloc(u64 size);
void  os_free(void *ptr);

b32 os_handle_match(OS_handle a, OS_handle b);
b32 os_handle_is_zero(OS_handle handle);

Str8 os_get_current_dir(Arena *a);
b32 os_set_current_dir(Str8 dir_path);
b32 os_set_current_dir_cstr(char *dir_path_cstr);

b32 os_move_file(Str8 old_path, Str8 new_path);
b32 os_remove_file(Str8 path);

b32 os_file_exists(Str8 path);

Str8 os_read_entire_file(Arena *a, Str8 path);

OS_file_iter* os_file_iter_begin(Arena *a, Str8 path, OS_file_iter_flags flags);
b32 os_file_iter_next(Arena *a, OS_file_iter *iter, OS_file_info *file_info);
void os_file_iter_end(OS_file_iter *iter);

Str8 os_file_name_from_path(Str8 path);

b32 os_make_dir(Str8 path);

b32 os_sleep_ms(u32 ms);

OS_handle os_thread_launch(OS_thread_func *fn, void *ptr, void *parmas);
b32       os_thread_join(OS_handle handle, u64 end_time_usec);
void      os_thread_detach(OS_handle handle);

OS_handle os_mutex_alloc(void);
void      os_mutex_release(OS_handle mutex);
void      os_mutex_take(OS_handle mutex);
void      os_mutex_drop(OS_handle mutex);

// TODO everything below

OS_handle os_rw_mutex_alloc(void);
void      os_rw_mutex_release(OS_handle rw_mutex);
void      os_rw_mutex_take_r(OS_handle rw_mutex);
void      os_rw_mutex_drop_r(OS_handle rw_mutex);
void      os_rw_mutex_take_w(OS_handle rw_mutex);
void      os_rw_mutex_drop_w(OS_handle rw_mutex);

OS_handle os_cond_var_alloc(void);
void      os_cond_var_release(OS_handle cv);

// returns false on timeout, true on signal, (max_wait_ms = max_U64) -> no timeout
b32       os_cond_var_wait(OS_handle cv, OS_handle mutex, u64 end_time_usec);

b32       os_cond_var_wait_rw_r(OS_handle cv, OS_handle mutex_rw, u64 end_time_usec);
b32       os_cond_var_wait_rw_w(OS_handle cv, OS_handle mutex_rw, u64 end_time_usec);
void      os_cond_var_signal(OS_handle cv);
void      os_cond_var_broadcast(OS_handle cv);

OS_handle os_semaphore_alloc(u32 initial_count, u32 max_count, Str8 name);
void      os_semaphore_release(OS_handle semaphore);
OS_handle os_semaphore_open(Str8 name);
void      os_semaphore_close(OS_handle semaphore);
b32       os_semaphore_take(OS_handle semaphore, u64 end_time_usec);
void      os_semaphore_drop(OS_handle semaphore);

#define os_mutex_scope(mutex) defer_loop(os_mutex_take(mutex), os_mutex_drop(mutex))
#define os_mutex_scope_r(mutex) defer_loop(os_rw_mutex_take_r(mutex), os_rw_mutex_drop_r(mutex))
#define os_mutex_scope_w(mutex) defer_loop(os_rw_mutex_take_w(mutex), os_rw_mutex_drop_w(mutex))
#define os_mutex_scope_rw_promote(mutex) defer_loop((os_rw_mutex_drop_r(mutex), os_rw_mutex_take_w(mutex)), (os_rw_mutex_drop_w(mutex), os_rw_mutex_take_r(mutex)))

OS_handle  os_library_open(Str8 path);
void       os_library_close(OS_handle lib);
Void_func* os_library_load_proc(OS_handle lib, Str8 name);

#endif
