
// TESTING SKIPPY
struct Str8 {
  u8 ***s;
  int **array[4][3];
  s64 len /* another test */;
  // hello
};

struct Game {

  f32 dt;
  b32 quit;

  u64 frame_index;

  u64 main_arena_reset_pos;
  Arena *main_arena;
  Arena *frame_arena;
  Arena *level_arena;

  void *os_state;
  UI_state *ui_state;

  b32  loaded_files;
  Str8 files[64];
  s32  files_count;
  s32  current_file;

  Item_list item_lists[2];
  Item_node *dragging_item;
  Vector2 dragging_item_pos;
  f32 draggin_item_size[2];

  Image test_bitmap;
  Texture test_bitmap_texture;

};
