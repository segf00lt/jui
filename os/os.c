#ifndef OS_C
#define OS_C

#if defined(OS_LINUX)

#include "os_linux.c"

#elif defined(OS_WEB)

#include "os_web.c"

#elif defined(OS_MAC)

#include "os_mac.c"

#elif defined(OS_WINDOWS)

#include "os_windows.c"

#endif

b32 os_set_current_dir(Arena *a, Str8 dir_path) {
  b32 result = 0;

  arena_scope(a) {
    char *dir_path_cstr = cstr_copy_str8(a, dir_path); 
    result = os_set_current_dir_cstr(dir_path_cstr);
  }

  return result;
}

#endif
