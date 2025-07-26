#if defined(OS_WEB)

#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>

#ifdef PATH_MAX
#define OS_PATH_LEN PATH_MAX
#else
#define OS_PATH_LEN 4096
#endif

typedef struct tm OS_web_tm;
typedef struct timespec OS_web_timespec;
typedef struct dirent OS_web_dirent;

typedef struct OS_web_file_iter OS_web_file_iter;
struct OS_web_file_iter {
  DIR *dir;
  OS_web_dirent *dp;
  Str8 path;
};


func Date_time os_web_date_time_from_tm(OS_web_tm in, u32 msec) {
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

func OS_web_tm os_web_tm_from_date_time(Date_time dt) {
  OS_web_tm result = {0};
  result.tm_sec  = dt.sec;
  result.tm_min  = dt.min;
  result.tm_hour = dt.hour;
  result.tm_mday = dt.day+1;
  result.tm_mon  = dt.month;
  result.tm_year = dt.year-1900;

  return result;
}

func Dense_time os_web_dense_time_from_timespec(OS_web_timespec in) {
  Dense_time result = 0;
  {
    OS_web_tm tm_time = {0};
    gmtime_r(&in.tv_sec, &tm_time);
    Date_time date_time = os_web_date_time_from_tm(tm_time, in.tv_nsec/(MILLION(1)));
    result = dense_time_from_date_time(date_time);
  }
  return result;
}

func OS_file_iter* os_file_iter_begin(Arena *a, Str8 path, OS_file_iter_flags flags) {
  return 0;
}

func b32 os_file_iter_next(Arena *a, OS_file_iter *iter, OS_file_info *file_info) {
  return 0;
}

func void os_file_iter_end(OS_file_iter *iter) {

}

func void* os_alloc(u64 size) {
  return malloc(size);
}

func void os_free(void *ptr) {
  free(ptr);
}

func Str8 os_get_current_dir(Arena *a) {
  Str8 result = {0};

  return result;
}

func b32 os_set_current_dir_cstr(char *dir_path_cstr) {
  b32 result = 0;

  result = !chdir(dir_path_cstr);

  return result;
}

func b32 os_move_file(Arena *a, Str8 old_path, Str8 new_path) {
  b32 result = 1;

  arena_scope(a) {
    const char *old_path_cstr = push_cstr_copy_str8(a, old_path); 
    const char *new_path_cstr = push_cstr_copy_str8(a, new_path); 

    result = !rename(old_path_cstr, new_path_cstr);
  }

  return result;
}

func b32 os_remove_file(Arena *a, Str8 path) {
  b32 result = 1;

  arena_scope(a) {
    const char *path_cstr = push_cstr_copy_str8(a, path);
    if(remove(path_cstr) < 0) {
      result = 0;
    }
  }

  return result;
}

func b32 os_file_exists(char *path) {
  b32 result = 0;

  struct stat statbuf;
  if (stat(path, &statbuf) == 0) result = 1;

  return result;
}

func Str8 os_read_entire_file(Arena *a, Str8 path) {
  Str8 result = {0};

  FILE *f;
  arena_scope(a) {
    f = fopen(push_cstr_copy_str8(a, path), "rb");
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

#endif
