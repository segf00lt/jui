#if defined(OS_MAC)

#error mac port unfinished

#include <limits.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <mach/mach_time.h>

#define OS_PATH_LEN MAXPATHLEN

typedef struct tm OS_mac_tm;
typedef struct timespec OS_mac_timespec;
typedef struct dirent OS_mac_irent;

typedef struct OS_mac_file_iter OS_mac_file_iter;
struct OS_mac_file_iter {
  DIR *dir;
  OS_mac_dirent *dp;
  Str8 path;
};


func Date_time os_mac_date_time_from_tm(OS_mac_tm in, u32 msec) {
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

func OS_mac_tm os_mac_tm_from_date_time(Date_time dt) {
  OS_mac_tm result = {0};
  result.tm_sec  = dt.sec;
  result.tm_min  = dt.min;
  result.tm_hour = dt.hour;
  result.tm_mday = dt.day+1;
  result.tm_mon  = dt.month;
  result.tm_year = dt.year-1900;

  return result;
}

func OS_mac_timespec os_mac_timespec_from_date_time(Date_time dt) {
  OS_mac_tm tm_val = os_mac_tm_from_date_time(dt);
  time_t seconds = timegm(&tm_val);
  OS_mac_timespec result = {0};
  result.tv_sec = seconds;
  return result;
}

func Dense_time os_mac_dense_time_from_timespec(OS_mac_timespec in) {
  Dense_time result = 0;
  {
    OS_mac_m tm_time = {0};
    gmtime_r(&in.tv_sec, &tm_time);
    Date_time date_time = os_mac_date_time_from_tm(tm_time, in.tv_nsec/(1000000));
    result = dense_time_from_date_time(date_time);
  }
  return result;
}

func OS_file_iter* os_file_iter_begin(Arena *a, Str8 path, OS_file_iter_flags flags) {
  OS_file_iter *base_iter = push_struct(a, OS_file_iter);
  base_iter->flags = flags;

  OS_mac_file_iter *iter = (OS_mac_file_iter*)base_iter->memory;

  Str8 path_copy = push_str8_copy(a, path);
  iter->dir = opendir((char*)path_copy.s);
  iter->path = path_copy;

  return base_iter;
}

func File_properties os_mac_file_properties_from_stat(OS_mac_stat *s) {
  File_properties props = {0};
  props.size     = s->st_size;
  // TODO this is not portable
  props.created  = os_mac_dense_time_from_timespec(s->st_ctim);
  props.modified = os_mac_dense_time_from_timespec(s->st_mtim);
  if(S_ISDIR(s->st_mode)) {
    props.flags |= FILE_PROPERTY_IS_DIR;
  }
  return props;
}

func b32 os_file_iter_next(Arena *a, OS_file_iter *iter, OS_file_info *file_info) {
  b32 good = 0;

  OS_mac_file_iter *mac_iter = (OS_mac_file_iter*)iter->memory;

  for(; mac_iter->dir != 0;) {
    mac_iter->dp = readdir(mac_iter->dir);

    good = (mac_iter->dp != 0);

    struct stat st = {0};
    int stat_result = 0;

    if(good) arena_scope(a) {
      Str8 full_path = push_str8f(a, "%S/%s", mac_iter->path, mac_iter->dp->d_name);
      stat_result = stat((char*)full_path.s, &st);
    }

    b32 filtered = 0;

    if(good) {

      b8 is_dir = S_ISDIR(st.st_mode);
      b8 is_file = S_ISREG(st.st_mode);
      b8 is_dot =
        (mac_iter->dp->d_name[0] == '.' && mac_iter->dp->d_name[1] == 0);
      b8 is_dot_dot =
        (mac_iter->dp->d_name[0] == '.' && mac_iter->dp->d_name[1] == '.' && mac_iter->dp->d_name[2] == 0);
      b8 is_hidden = (!is_dot && !is_dot_dot && mac_iter->dp->d_name[0] == '.');

      filtered =
        ((is_dir && iter->flags & OS_FILE_ITER_SKIP_DIRS) ||
         (is_file && iter->flags & OS_FILE_ITER_SKIP_FILES) ||
         (is_hidden && iter->flags & OS_FILE_ITER_SKIP_HIDDEN_FILES) ||
         (is_dot) ||
         (is_dot_dot));
    }

    if(good && !filtered) {
      if(stat_result != -1) {
        file_info->name = push_str8_copy_cstr(a, mac_iter->dp->d_name);
        file_info->props = os_mac_file_properties_from_stat(&st);
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
  OS_mac_file_iter *mac_iter = (OS_mac_file_iter*)iter->memory;
  closedir(mac_iter->dir);
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

func b32 os_move_file(Arena *a, Str8 old_path, Str8 new_path) {
  b32 result = 0;

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
