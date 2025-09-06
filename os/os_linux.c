#if defined(OS_LINUX)

#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#ifdef PATH_MAX
#define OS_PATH_LEN PATH_MAX
#else
#define OS_PATH_LEN 4096
#endif

typedef struct OS_linux_file_iter OS_linux_file_iter;
struct OS_linux_file_iter {
  DIR *dir;
  struct dirent *dp;
  Str8 path;
};

typedef struct OS_linux_thread OS_linux_thread;
struct OS_linux_thread {
  pthread_t handle;
  OS_thread_func *fn;
  void *ptr;
};

typedef struct OS_linux_cond_var OS_linux_cond_var;
struct OS_linux_cond_var {
  pthread_cond_t cond;
  pthread_mutex_t rwlock_mutex;
};

typedef enum OS_linux_entity_kind {
  OS_LINUX_ENTITY_THREAD = 0,
  OS_LINUX_ENTITY_MUTEX,
  OS_LINUX_ENTITY_RWMUTEX,
  OS_LINUX_ENTITY_CONDITION_VARIABLE,
} OS_linux_entity_kind;

typedef union OS_linux_entity_data OS_linux_entity_data;
union OS_linux_entity_data {
  OS_linux_thread thread;
  pthread_mutex_t mutex;
  pthread_rwlock_t rwmutex;
  OS_linux_cond_var cv;
};

typedef struct OS_linux_entity OS_linux_entity;
struct OS_linux_entity {
  OS_linux_entity *next;
  OS_linux_entity_kind kind;
  OS_linux_entity_data e;
};

typedef struct OS_linux_state OS_linux_state;
struct OS_linux_state {
  Arena *arena;
  OS_system_info system_info;
  OS_process_info process_info;
  pthread_mutex_t entity_mutex;
  Arena *entity_arena;
  OS_linux_entity *entity_free_list;
};

/*
 * function headers
 */

b32 os_sleep_ms(u32 ms);
Str8 os_file_name_from_path(Str8 path);
Date_time os_linux_date_time_from_tm(struct tm in, u32 msec);
struct tm os_linux_tm_from_date_time(Date_time dt);
Dense_time os_linux_dense_time_from_timespec(struct timespec in);
OS_file_iter* os_file_iter_begin(Arena *a, Str8 path, OS_file_iter_flags flags);
File_properties os_linux_file_properties_from_stat(struct stat *s);
b32 os_file_iter_next(Arena *a, OS_file_iter *iter, OS_file_info *file_info);
void os_file_iter_end(OS_file_iter *iter);
void* os_alloc(u64 size);
void os_free(void *ptr);
Str8 os_get_current_dir(Arena *a);
b32 os_set_current_dir_cstr(char *dir_path_cstr);
b32 os_move_file(Str8 old_path, Str8 new_path);
b32 os_remove_file(Str8 path);
b32 os_file_exists(Str8 path);
Str8 os_read_entire_file(Arena *a, Str8 path);
b32 os_make_dir(Str8 path);
OS_linux_entity* os_linux_entity_alloc(OS_linux_entity_kind kind);
void os_linux_entity_release(OS_linux_entity *entity);
OS_handle os_thread_launch(OS_thread_func *fn, void *ptr, void *params);
b32 os_thread_join(OS_handle handle, u64 end_time_usec);
void os_thread_detach(OS_handle handle);
OS_handle os_mutex_alloc(void);
void os_mutex_release(OS_handle mutex);
void os_mutex_take(OS_handle mutex);
void os_mutex_drop(OS_handle mutex);
OS_handle os_rw_mutex_alloc(void);
void os_rw_mutex_release(OS_handle rw_mutex);
void os_rw_mutex_take_r(OS_handle rw_mutex);
void os_rw_mutex_drop_r(OS_handle rw_mutex);
void os_rw_mutex_take_w(OS_handle rw_mutex);
void os_rw_mutex_drop_w(OS_handle rw_mutex);
OS_handle os_cond_var_alloc(void);
void os_cond_var_release(OS_handle cv);
b32 os_cond_var_wait(OS_handle cv, OS_handle mutex, u64 end_time_usec);
b32 os_cond_var_wait_rw_r(OS_handle cv, OS_handle mutex_rw, u64 end_time_usec);
b32 os_cond_var_wait_rw_w(OS_handle cv, OS_handle mutex_rw, u64 end_time_usec);
void os_cond_var_signal(OS_handle cv);
void os_cond_var_broadcast(OS_handle cv);
//OS_handle os_semaphore_alloc(u32 initial_count, u32 max_count, Str8 name);
//void  os_semaphore_release(OS_handle semaphore);
//OS_handle os_semaphore_open(Str8 name);
//void os_semaphore_close(OS_handle semaphore);
//b32 os_semaphore_take(OS_handle semaphore, u64 end_time_usec);
//void os_semaphore_drop(OS_handle semaphore);
OS_handle os_library_open(Str8 path);
void os_library_close(OS_handle lib);
Void_func* os_library_load_proc(OS_handle lib, Str8 name);


OS_linux_state *os_linux_state;


func b32 os_sleep_ms(u32 ms) {
  b32 result = 1;
  struct timespec ts = {0};
  ts.tv_nsec = MILLION(1)*ms;
  int i = nanosleep(&ts, 0);
  if(i != 0) {
    result = 0;
  }
  return result;
}

func Str8 os_file_name_from_path(Str8 path) {
  Str8 result;

  s64 i;

  for(i = path.len - 1; i >= 0; i--) {
    if(path.s[i] == '/') break;
  }

  result = str8_slice(path, i + 1, path.len);

  return result;
}

func Date_time os_linux_date_time_from_tm(struct tm in, u32 msec) {
  Date_time dt = {0};
  dt.sec  = in.tm_sec;
  dt.min  = in.tm_min;
  dt.hour = in.tm_hour;
  dt.day  = in.tm_mday-1;
  dt.month  = in.tm_mon;
  dt.year = in.tm_year+1900;
  dt.msec = msec;

  return dt;
}

func struct tm os_linux_tm_from_date_time(Date_time dt) {
  struct tm result = {0};
  result.tm_sec  = dt.sec;
  result.tm_min  = dt.min;
  result.tm_hour = dt.hour;
  result.tm_mday = dt.day+1;
  result.tm_mon  = dt.month;
  result.tm_year = dt.year-1900;

  return result;
}

func Dense_time os_linux_dense_time_from_timespec(struct timespec in) {
  Dense_time result = 0;
  {
    struct tm tm_time = {0};
    gmtime_r(&in.tv_sec, &tm_time);
    Date_time date_time = os_linux_date_time_from_tm(tm_time, in.tv_nsec/(MILLION(1)));
    result = dense_time_from_date_time(date_time);
  }
  return result;
}

func OS_file_iter* os_file_iter_begin(Arena *a, Str8 path, OS_file_iter_flags flags) {
  OS_file_iter *base_iter = push_struct(a, OS_file_iter);
  base_iter->flags = flags;

  OS_linux_file_iter *iter = (OS_linux_file_iter*)base_iter->memory;

  Str8 path_copy = str8_copy(a, path);
  iter->dir = opendir((char*)path_copy.s);
  iter->path = path_copy;

  return base_iter;
}

func File_properties os_linux_file_properties_from_stat(struct stat *s) {
  File_properties props = {0};
  props.size     = s->st_size;
  //props.created  = dense_time_from_unix_time(s->st_ctime);
  //props.modified = dense_time_from_unix_time(s->st_mtime);
  // NOTE this is not portable????
  props.created  = os_linux_dense_time_from_timespec(s->st_ctim);
  props.modified = os_linux_dense_time_from_timespec(s->st_mtim);
  if(S_ISDIR(s->st_mode)) {
    props.flags |= FILE_PROPERTY_IS_DIR;
  }
  return props;
}

func b32 os_file_iter_next(Arena *a, OS_file_iter *iter, OS_file_info *file_info) {
  b32 good = 0;

  OS_linux_file_iter *linux_iter = (OS_linux_file_iter*)iter->memory;

  for(; linux_iter->dir != 0;) {
    linux_iter->dp = readdir(linux_iter->dir);

    good = (linux_iter->dp != 0);

    struct stat st = {0};
    int stat_result = 0;

    if(good) arena_scope(a) {
      Str8 full_path = str8f(a, "%S/%s", linux_iter->path, linux_iter->dp->d_name);
      stat_result = stat((char*)full_path.s, &st);
    }

    b32 filtered = 0;

    if(good) {
      b8 is_dir = S_ISDIR(st.st_mode);
      b8 is_file = S_ISREG(st.st_mode);
      b8 is_dot =
        (linux_iter->dp->d_name[0] == '.' && linux_iter->dp->d_name[1] == 0);
      b8 is_dot_dot =
        (linux_iter->dp->d_name[0] == '.' && linux_iter->dp->d_name[1] == '.' && linux_iter->dp->d_name[2] == 0);
      b8 is_hidden = (!is_dot && !is_dot_dot && linux_iter->dp->d_name[0] == '.');

      filtered =
        ((is_dir && iter->flags & OS_FILE_ITER_SKIP_DIRS) ||
         (is_file && iter->flags & OS_FILE_ITER_SKIP_FILES) ||
         (is_hidden && iter->flags & OS_FILE_ITER_SKIP_HIDDEN_FILES) ||
         (is_dot) ||
         (is_dot_dot));
    }

    if(good && !filtered) {
      if(stat_result != -1) {
        file_info->name = str8_copy_cstr(a, linux_iter->dp->d_name);
        file_info->props = os_linux_file_properties_from_stat(&st);
      }
      break;
    }

    if(!good) {
      break;
    }

  }

  return good;
}

func void os_file_iter_end(OS_file_iter *iter) {
  OS_linux_file_iter *linux_iter = (OS_linux_file_iter*)iter->memory;
  closedir(linux_iter->dir);
}

func void* os_alloc(u64 size) {
  return malloc(size);
}

func void os_free(void *ptr) {
  free(ptr);
}

func Str8 os_get_current_dir(Arena *a) {
  size_t buf_size = OS_PATH_LEN;

  Str8 result = {0};

  u64 pos = arena_pos(a);
  char *buf = push_array_no_zero(a, char, buf_size);

  char *s = getcwd(buf, buf_size);

  while(s == NULL && buf_size < 4*OS_PATH_LEN) {
    arena_pop_to(a, pos);
    buf_size <<= 1;
    buf = push_array_no_zero(a, char, buf_size);
    s = getcwd(buf, buf_size);
  }

  result.s = (u8*)s;
  result.len = memory_strlen(s);

  arena_pop(a, buf_size - (result.len - 1));

  return result;
}

func b32 os_set_current_dir_cstr(char *dir_path_cstr) {
  b32 result = 0;

  result = !chdir(dir_path_cstr);

  return result;
}

func b32 os_move_file(Str8 old_path, Str8 new_path) {
  b32 result = 0;

  Arena_scope scratch = scratch_begin(0, 0);

  const char *old_path_cstr = cstr_copy_str8(scratch.arena, old_path); 
  const char *new_path_cstr = cstr_copy_str8(scratch.arena, new_path); 

  result = !rename(old_path_cstr, new_path_cstr);

  scratch_end(scratch);

  return result;
}

func b32 os_remove_file(Str8 path) {
  b32 result = 1;

  Arena_scope scratch = scratch_begin(0, 0);

  const char *path_cstr = cstr_copy_str8(scratch.arena, path);
  if(remove(path_cstr) < 0) {
    result = 0;
  }

  scratch_end(scratch);

  return result;
}

func b32 os_file_exists(Str8 path) {
  b32 result = 0;

  Arena_scope scratch = scratch_begin(0, 0);

  struct stat statbuf;
  if(stat(cstr_copy_str8(scratch.arena, path), &statbuf) == 0) result = 1;

  scratch_end(scratch);

  return result;
}

func Str8 os_read_entire_file(Arena *a, Str8 path) {
  Str8 result = {0};

  FILE *f;
  arena_scope(a) {
    f = fopen(cstr_copy_str8(a, path), "rb");
  }

  if(f == NULL) {
    goto end;
  }

  if(fseek(f, 0, SEEK_END) < 0) {
    goto end;
  }

  long m = ftell(f);

  if(m < 0) {
    goto end;
  }

  if(fseek(f, 0, SEEK_SET) < 0) {
    goto end;
  }

  u8 *p = push_array_no_zero(a, u8, (u64)m);

  fread((void*)p, 1, m, f);

  if(ferror(f)) {
    goto end;
  }

  result.s = p;
  result.len = (s64)m;

end:

  if(f) {
    fclose(f);
  }

  return result;
}

func b32 os_make_dir(Str8 path) {
  Arena_scope scratch = scratch_begin(0, 0);

  int result = mkdir(cstr_copy_str8(scratch.arena, path), 0755);

  scratch_end(scratch);

  if(result < 0) {
    if(errno == EEXIST) {
      return 1;
    }

    return 0;
  }

  return 1;
}

func OS_linux_entity* os_linux_entity_alloc(OS_linux_entity_kind kind) {
  OS_linux_entity *entity = 0;

  defer_loop(
      pthread_mutex_lock(&os_linux_state->entity_mutex),
      pthread_mutex_unlock(&os_linux_state->entity_mutex)
      )
  {
    entity = os_linux_state->entity_free_list;
    if(entity) {
      sll_stack_pop(os_linux_state->entity_free_list);
    } else {
      entity = push_struct_no_zero(os_linux_state->entity_arena, OS_linux_entity);
    }
  }

  memory_zero(entity, sizeof(OS_linux_entity));
  entity->kind = kind;

  return entity;
}

func void os_linux_entity_release(OS_linux_entity *entity) {

  defer_loop(
      pthread_mutex_lock(&os_linux_state->entity_mutex),
      pthread_mutex_unlock(&os_linux_state->entity_mutex)
      )
  {
    sll_stack_push(os_linux_state->entity_free_list, entity);
  }

}

void* os_linux_thread_entry_point(void *ptr) {
  OS_linux_entity *entity = (OS_linux_entity *)ptr;
  OS_thread_func *fn = entity->e.thread.fn;
  void *thread_ptr = entity->e.thread.ptr;
  // TODO thread local context ??
  fn(thread_ptr);
  return 0;
}

// TODO threads
func OS_handle os_thread_launch(OS_thread_func *fn, void *ptr, void *params) {
  OS_handle handle;

  OS_linux_entity *entity = os_linux_entity_alloc(OS_LINUX_ENTITY_THREAD);

  entity->e.thread.fn = fn;
  entity->e.thread.ptr = ptr;

  int pthread_result = pthread_create(&entity->e.thread.handle, 0, os_linux_thread_entry_point, (void*)entity);
  if(pthread_result == -1) {
    os_linux_entity_release(entity);
    entity = 0;
  }

  handle = (OS_handle){(u64)entity};

  return handle;
}

func b32 os_thread_join(OS_handle handle, u64 end_time_usec) {
  ASSERT(!os_handle_is_zero(handle));
  //if(os_handle_is_zero(handle)) return 0;

  OS_linux_entity *entity = (OS_linux_entity*)handle.hand[0];
  int join_result = pthread_join(entity->e.thread.handle, 0);
  b32 result = (join_result == 0);

  os_linux_entity_release(entity);

  return result;
}

func void os_thread_detach(OS_handle handle) {
  //if(os_handle_is_zero(handle)) return;
  ASSERT(!os_handle_is_zero(handle));

  OS_linux_entity *entity = (OS_linux_entity*)handle.hand[0];
  os_linux_entity_release(entity);
}

func OS_handle os_mutex_alloc(void) {
  OS_linux_entity *entity = os_linux_entity_alloc(OS_LINUX_ENTITY_MUTEX);
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  int init_result = pthread_mutex_init(&entity->e.mutex, &attr);
  pthread_mutexattr_destroy(&attr);
  if(init_result == -1)
  {
    os_linux_entity_release(entity);
    entity = 0;
  }
  OS_handle handle = {(u64)entity};
  return handle;
}

func void os_mutex_release(OS_handle mutex) {
  ASSERT(mutex.hand[0] != 0);
  OS_linux_entity *entity = (OS_linux_entity*)mutex.hand[0];
  pthread_mutex_destroy(&entity->e.mutex);
}

func void os_mutex_take(OS_handle mutex) {
  ASSERT(mutex.hand[0] != 0);
  OS_linux_entity *entity = (OS_linux_entity*)mutex.hand[0];
  pthread_mutex_lock(&entity->e.mutex);
}

func void os_mutex_drop(OS_handle mutex) {
  ASSERT(mutex.hand[0] != 0);
  OS_linux_entity *entity = (OS_linux_entity*)mutex.hand[0];
  pthread_mutex_unlock(&entity->e.mutex);
}

// TODO everything below

func OS_handle os_rw_mutex_alloc(void) {
  OS_linux_entity *entity = os_linux_entity_alloc(OS_LINUX_ENTITY_RWMUTEX);

  int init_result = pthread_rwlock_init(&entity->e.rwmutex, 0);

  if(init_result == -1) {
    os_linux_entity_release(entity);
    entity = 0;
  }

  OS_handle handle = { (u64)entity };

  return handle;
}

func void os_rw_mutex_release(OS_handle rw_mutex) {
  ASSERT(rw_mutex.hand[0] != 0);

  OS_linux_entity *entity = (OS_linux_entity*)rw_mutex.hand[0];

  pthread_rwlock_destroy(&entity->e.rwmutex);

  os_linux_entity_release(entity);
}

func void os_rw_mutex_take_r(OS_handle rw_mutex) {
  ASSERT(rw_mutex.hand[0] != 0);

  OS_linux_entity *entity = (OS_linux_entity*)rw_mutex.hand[0];

  pthread_rwlock_rdlock(&entity->e.rwmutex);
}

func void os_rw_mutex_drop_r(OS_handle rw_mutex) {
  ASSERT(rw_mutex.hand[0] != 0);

  OS_linux_entity *entity = (OS_linux_entity*)rw_mutex.hand[0];

  pthread_rwlock_unlock(&entity->e.rwmutex);
}

func void os_rw_mutex_take_w(OS_handle rw_mutex) {
  ASSERT(rw_mutex.hand[0] != 0);

  OS_linux_entity *entity = (OS_linux_entity*)rw_mutex.hand[0];

  pthread_rwlock_wrlock(&entity->e.rwmutex);
}

func void os_rw_mutex_drop_w(OS_handle rw_mutex) {
  ASSERT(rw_mutex.hand[0] != 0);

  OS_linux_entity *entity = (OS_linux_entity*)rw_mutex.hand[0];

  pthread_rwlock_unlock(&entity->e.rwmutex);
}


func OS_handle os_cond_var_alloc(void) {
  OS_linux_entity *entity = os_linux_entity_alloc(OS_LINUX_ENTITY_CONDITION_VARIABLE);

  int init_result = pthread_cond_init(&entity->e.cv.cond, 0);

  if(init_result == -1) {
    os_linux_entity_release(entity);
    entity = 0;
  }

  int init_mutex_result = pthread_mutex_init(&entity->e.cv.rwlock_mutex, 0);

  if(init_mutex_result == -1) {
    pthread_cond_destroy(&entity->e.cv.cond);
    os_linux_entity_release(entity);
    entity = 0;
  }

  OS_handle handle = { (u64)entity };

  return handle;
}

func void os_cond_var_release(OS_handle cv) {
  ASSERT(cv.hand[0] != 0);

  OS_linux_entity *entity = (OS_linux_entity*)cv.hand[0];

  pthread_cond_destroy(&entity->e.cv.cond);
  pthread_mutex_destroy(&entity->e.cv.rwlock_mutex);

  os_linux_entity_release(entity);
}

// NOTE returns false on timeout, true on signal, (max_wait_ms = max_U64) -> no timeout
func b32 os_cond_var_wait(OS_handle cv, OS_handle mutex, u64 end_time_usec) {
  if(os_handle_is_zero(cv)) return 0;
  if(os_handle_is_zero(mutex)) return 0;

  OS_linux_entity *cv_entity = (OS_linux_entity*)cv.hand[0];
  OS_linux_entity *mutex_entity = (OS_linux_entity*)mutex.hand[0];

  struct timespec end_time_timespec;
  end_time_timespec.tv_sec = end_time_usec / MILLION(1);
  end_time_timespec.tv_nsec = THOUSAND(1) * (end_time_usec - ((end_time_usec / MILLION(1)) * MILLION(1)));

  int wait_result = pthread_cond_timedwait(&cv_entity->e.cv.cond, &mutex_entity->e.mutex, &end_time_timespec);

  b32 result = (wait_result != ETIMEDOUT);

  return result;
}

func b32 os_cond_var_wait_rw_r(OS_handle cv, OS_handle mutex_rw, u64 end_time_usec) {
  // TODO(rjf): because pthread does not supply cv/rw natively, I had to hack
  // this together, but this would probably just be a lot better if we just
  // implemented the primitives ourselves with e.g. futexes
  //

  if(os_handle_is_zero(cv)) return 0;
  if(os_handle_is_zero(mutex_rw)) return 0;

  OS_linux_entity *cv_entity = (OS_linux_entity *)cv.hand[0];
  OS_linux_entity *rw_mutex_entity = (OS_linux_entity *)mutex_rw.hand[0];

  struct timespec end_time_timespec;
  end_time_timespec.tv_sec = end_time_usec/MILLION(1);
  end_time_timespec.tv_nsec = THOUSAND(1) * (end_time_usec - (end_time_usec/MILLION(1))*MILLION(1));

  b32 result = 0;
  pthread_mutex_lock(&cv_entity->e.cv.rwlock_mutex);
  pthread_rwlock_unlock(&rw_mutex_entity->e.rwmutex);

  for(;;) {
    int wait_result =
      pthread_cond_timedwait(&cv_entity->e.cv.cond, &cv_entity->e.cv.rwlock_mutex, &end_time_timespec);
    if(wait_result != ETIMEDOUT)
    {
      pthread_rwlock_rdlock(&rw_mutex_entity->e.rwmutex);
      result = 1;
      break;
    }
    if(wait_result == ETIMEDOUT)
    {
      pthread_rwlock_rdlock(&rw_mutex_entity->e.rwmutex);
      break;
    }
  }

  pthread_mutex_unlock(&cv_entity->e.cv.rwlock_mutex);

  return result;
}

func b32 os_cond_var_wait_rw_w(OS_handle cv, OS_handle mutex_rw, u64 end_time_usec) {
  // TODO(rjf): because pthread does not supply cv/rw natively, I had to hack
  // this together, but this would probably just be a lot better if we just
  // implemented the primitives ourselves with e.g. futexes
  //

  if(os_handle_is_zero(cv)) return 0;
  if(os_handle_is_zero(mutex_rw)) return 0;

  OS_linux_entity *cv_entity = (OS_linux_entity *)cv.hand[0];
  OS_linux_entity *rw_mutex_entity = (OS_linux_entity *)mutex_rw.hand[0];

  struct timespec end_time_timespec;
  end_time_timespec.tv_sec = end_time_usec/MILLION(1);
  end_time_timespec.tv_nsec = THOUSAND(1) * (end_time_usec - (end_time_usec/MILLION(1))*MILLION(1));

  b32 result = 0;
  pthread_mutex_lock(&cv_entity->e.cv.rwlock_mutex);
  pthread_rwlock_unlock(&rw_mutex_entity->e.rwmutex);

  for(;;) {
    int wait_result =
      pthread_cond_timedwait(&cv_entity->e.cv.cond, &cv_entity->e.cv.rwlock_mutex, &end_time_timespec);
    if(wait_result != ETIMEDOUT)
    {
      pthread_rwlock_wrlock(&rw_mutex_entity->e.rwmutex);
      result = 1;
      break;
    }
    if(wait_result == ETIMEDOUT)
    {
      pthread_rwlock_wrlock(&rw_mutex_entity->e.rwmutex);
      break;
    }
  }

  pthread_mutex_unlock(&cv_entity->e.cv.rwlock_mutex);

  return result;
}

func void os_cond_var_signal(OS_handle cv) {
  if(os_handle_is_zero(cv)) return;

  OS_linux_entity *entity = (OS_linux_entity*)cv.hand[0];

  pthread_cond_signal(&entity->e.cv.cond);
}

func void os_cond_var_broadcast(OS_handle cv) {
  if(os_handle_is_zero(cv)) return;

  OS_linux_entity *entity = (OS_linux_entity*)cv.hand[0];

  pthread_cond_broadcast(&entity->e.cv.cond);
}


#if 0
func OS_handle os_semaphore_alloc(u32 initial_count, u32 max_count, Str8 name) {
  OS_handle result = {0};

  if(name.len > 0) {
    // TODO: we need to allocate shared memory to store sem_t
    UNIMPLEMENTED;
  } else {
    //sem_t *s = mmap(0, sizeof(*s), PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    // NOTE if we decide to use virtual memory on linux maybe change this
    sem_t *s = malloc(sizeof(*s));
    ASSERT_ALWAYS(s != 0);
    int err = sem_init(s, 0, initial_count);
    if(err == 0) {
      result.hand[0] = (u64)s;
    }
  }

  return result;
}

func void  os_semaphore_release(OS_handle semaphore) {
  // NOTE if we decide to use virtual memory on linux maybe change this
  free((void*)semaphore.hand[0]);
}

func OS_handle os_semaphore_open(Str8 name) {
  // TODO thread local context for temporary allocations and copies
  UNIMPLEMENTED;
}

func void os_semaphore_close(OS_handle semaphore) {
  // TODO thread local context for temporary allocations and copies
  UNIMPLEMENTED;
  ASSERT_ALWAYS(sem_close((sem_t*)semaphore.hand[0]) == 0);
}

func b32 os_semaphore_take(OS_handle semaphore, u64 end_time_usec) {
  ASSERT_ALWAYS(end_time_usec == MAX_U64);

  struct timespec end_time_timespec;
  end_time_timespec.tv_sec = end_time_usec/MILLION(1);
  end_time_timespec.tv_nsec = THOUSAND(1) * (end_time_usec - (end_time_usec/MILLION(1))*MILLION(1));

  for(;;) {
    int err = sem_timedwait((sem_t*)semaphore.hand[0], &end_time_timespec);

    if(err == 0) {
      break;
    } else if(errno == EAGAIN) {
      continue;
    }

    break;

  }

  return 1;
}

func void os_semaphore_drop(OS_handle semaphore) {
  for(;;) {
    int err = sem_post((sem_t*)semaphore.hand[0]);

    if(err == 0) {
      break;
    } else {
      if(errno == EAGAIN) {
        continue;
      }
    }

    break;
  }
}
#endif


func OS_handle os_library_open(Str8 path) {
  Arena_scope scratch = scratch_begin(0, 0);
  char *path_cstr = cstr_copy_str8(scratch.arena, path);
  void *so = dlopen(path_cstr, RTLD_LAZY|RTLD_LOCAL);
  OS_handle lib = { (u64)so };
  scratch_end(scratch);
  return lib;
}

func void os_library_close(OS_handle lib) {
  void *so = (void*)lib.hand;
  dlclose(so);
}

func Void_func* os_library_load_proc(OS_handle lib, Str8 name) {
  Arena_scope scratch = scratch_begin(0, 0);
  void *so = (void*)lib.hand[0];
  char *name_cstr = cstr_copy_str8(scratch.arena, name);
  Void_func *fn = (Void_func*)dlsym(so, name_cstr);
  return fn;
}

func void os_state_free(void *state) {
  OS_linux_state *linux_state = (OS_linux_state*)state;
  pthread_mutex_destroy(&linux_state->entity_mutex);
  arena_free(linux_state->entity_arena);
  arena_free(linux_state->arena);
}

func void* os_state_get_selected(void) {
  return (void*)os_linux_state;
}

func void os_state_select(void *state) {
  os_linux_state = (OS_linux_state*)state;
}

func void* os_state_alloc(void) {
  {
    Arena *linux_state_arena = arena_alloc(KB(8));
    os_linux_state = push_struct(linux_state_arena, OS_linux_state);
    os_linux_state->arena = linux_state_arena;
    os_linux_state->entity_arena = arena_alloc(KB(16));
    pthread_mutex_init(&os_linux_state->entity_mutex, 0);
  }

  {
    OS_system_info *info = &os_linux_state->system_info;
    info->logical_processor_count = (u32)sysconf(_SC_NPROCESSORS_ONLN);
    info->page_size = (u64)sysconf(_SC_PAGESIZE);
    info->large_page_size = MB(2);
    info->allocation_granularity = info->page_size;
  }

  {
    OS_process_info *info = &os_linux_state->process_info;
    info->pid = (u32)getpid();
  }

  Tctx *tctx = tctx_alloc();
  tctx_select(tctx);

  {
    Arena_scope scratch = scratch_begin(0, 0);
    OS_system_info *info = &os_linux_state->system_info;

    b32 got_final_result = 0;
    u8 *buffer = 0;
    int size = 0;
    for(s64 cap = 4096, r = 0; r < 4; cap *= 2, r += 1) {
      scratch_end(scratch);
      buffer = push_array_no_zero(scratch.arena, u8, cap);
      size = gethostname((char*)buffer, cap);
      if(size < cap) {
        got_final_result = 1;
        break;
      }
    }

    // rjf: save name to info
    if(got_final_result && size > 0) {
      info->machine_name.len = size;
      info->machine_name.s = push_array_no_zero(os_linux_state->arena, u8, info->machine_name.len + 1);
      memory_copy(info->machine_name.s, buffer, info->machine_name.len);
      info->machine_name.s[info->machine_name.len] = 0;
    }

    scratch_end(scratch);
  }

  //- rjf: grab dynamically allocated process info
  {
    Arena_scope scratch = scratch_begin(0, 0);
    OS_process_info *info = &os_linux_state->process_info;

    // rjf: grab binary path
    {
      // rjf: get self string
      b32 got_final_result = 0;
      u8 *buffer = 0;
      int size = 0;
      for(s64 cap = PATH_MAX, r = 0; r < 4; cap *= 2, r += 1) {
        scratch_end(scratch);
        buffer = push_array_no_zero(scratch.arena, u8, cap);
        size = readlink("/proc/self/exe", (char*)buffer, cap);
        if(size < cap) {
          got_final_result = 1;
          break;
        }
      }

      // rjf: save
      if(got_final_result && size > 0) {
        Str8 full_name = { .s = buffer, .len = size };
        Str8 name_chopped = str8_chop_last_slash(full_name);
        info->binary_path = str8_copy(os_linux_state->arena, name_chopped);
      }
    }

    // rjf: grab initial directory
    {
      info->initial_path = os_get_current_dir(os_linux_state->arena);
    }

    // rjf: grab home directory
    {
      char *home = getenv("HOME");
      info->user_program_data_path = (Str8){ .s = (u8*)home, .len = memory_strlen(home), };

      scratch_end(scratch);
    }
  }

  return (void*)os_linux_state;
}

#endif
