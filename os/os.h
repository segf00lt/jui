#ifndef OS_H
#define OS_H

typedef struct OS_handle OS_handle;
struct OS_handle {
  u64 hand[1];
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

void* os_alloc(u64 size);
void  os_free(void *ptr);

Str8 os_get_current_dir(Arena *a);
b32 os_set_current_dir(Arena *a, Str8 dir_path);
b32 os_set_current_dir_cstr(char *dir_path_cstr);

b32 os_move_file(Arena *a, Str8 old_path, Str8 new_path);
b32 os_remove_file(Arena *a, Str8 path);

b32 os_file_exists(char *path);

Str8 os_read_entire_file(Arena *a, Str8 path);

OS_file_iter* os_file_iter_begin(Arena *a, Str8 path, OS_file_iter_flags flags);
b32 os_file_iter_next(Arena *a, OS_file_iter *iter, OS_file_info *file_info);
void os_file_iter_end(OS_file_iter *iter);

Str8 os_file_name_from_path(Str8 path);

b32 os_make_dir(char *path);

#endif
