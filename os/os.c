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

func void os_init(void) {
  os_state_select(os_state_alloc());
}

force_inline b32 os_handle_match(OS_handle a, OS_handle b) {
  return (a.hand == b.hand);
}

force_inline b32 os_handle_is_zero(OS_handle handle) {
  return (handle.hand[0] == 0);
}

b32 os_set_current_dir(Str8 dir_path) {
  b32 result = 0;

  Arena_scope scratch = scratch_begin(0, 0);
  char *dir_path_cstr = cstr_copy_str8(scratch.arena, dir_path); 
  result = os_set_current_dir_cstr(dir_path_cstr);
  scratch_end(scratch);

  return result;
}

#endif
