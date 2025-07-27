#include "base.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "ui.c"


/*
 * constants
 */

#define GAME_TITLE "UI Test"
#define TARGET_FPS 60
#define TARGET_FRAME_TIME ((float)(1.0f / (float)TARGET_FPS))

#define WINDOW_WIDTH  ((float)GetScreenWidth())
#define WINDOW_HEIGHT ((float)GetScreenHeight())

#define WINDOW_SIZE ((Vector2){ WINDOW_WIDTH, WINDOW_HEIGHT })
#define WINDOW_RECT ((Rectangle){0, 0, WINDOW_WIDTH, WINDOW_HEIGHT})

#define MAX_FILES 64


/*
 * tables
 */

/*
 * macros
 */

#define PROFILE(dest) for(f32 __i__ = 0, __prof_time = (f32)GetTime(); __i__ == 0; dest = (f32)GetTime() - __prof_time, __i__ = 1.0f)

/*
 * typedefs
 */

typedef struct Game Game;
typedef struct Item_node Item_node;
typedef struct Item_list Item_list;

DECL_ARR_TYPE(Rectangle);
DECL_SLICE_TYPE(Rectangle);
DECL_SLICE_TYPE(u8);
DECL_ARR_TYPE(s64);
DECL_SLICE_TYPE(s64);
DECL_ARR_TYPE(UI_box_ptr);
DECL_ARR_TYPE(b64);
DECL_ARR_TYPE(Vector2);
DECL_ARR_TYPE(UI_text_align);
DECL_ARR_TYPE(UI_size);
DECL_ARR_TYPE(f32);
DECL_ARR_TYPE(u32);
typedef f32* f32_ptr;
DECL_ARR_TYPE(f32_ptr);


/*
 * struct bodies
 */

struct Item_node {
  Item_node *next;
  Item_node *prev;
  Str8 text;
};

struct Item_list {
  Vector2 pos;
  Str8 id;
  Item_node *first;
  Item_node *last;
  UI_signal sig;
};

struct Game {

  f32 dt;
  b32 quit;

  u64 frame_index;

  Arena *main_arena;
  Arena *frame_arena;
  Arena *level_arena;

  UI_context *ui;

  b32  loaded_files;
  Str8 files[MAX_FILES];
  s32  files_count;
  s32  current_file;

  Item_list item_lists[2];
  Item_node *dragging_item;
  Vector2 dragging_item_pos;
  f32 draggin_item_size[2];
};


/*
 * function headers
 */

Game* game_init(void);
void game_load_assets(Game *gp);
void game_unload_assets(Game *gp);
void game_update_and_draw(Game *gp);
void game_close(Game *gp);
void game_reset(Game *gp);

UI_signal item_button(Game *gp, Item_node *item);
UI_signal ui_button(UI_context *ui, Str8 label);
UI_signal item_list_window(Game *gp, Item_list *list);
void ui_spacer(UI_context *ui, f32 size);

/*
 * entity settings
 */


/*
 * generated code
 */


/*
 * globals
 */

int text_line_spacing = 5;

int _nocheckin = 0;

Vector2 window_pos2 =
{
  .x = 400,
  .y = 40,
};
Vector2 save_window_pos2;

/* 
 * function bodies
 */

#define frame_push_array(T, n) (T*)push_array(gp->frame_arena, T, n)
#define level_push_array(T, n) (T*)push_array(gp->level_arena, T, n)

#define frame_push_struct(T) (T*)push_array(gp->frame_arena, T, 1)
#define level_push_struct(T) (T*)push_array(gp->level_arena, T, 1)

#define frame_push_struct_no_zero(T) (T*)push_array_no_zero(gp->frame_arena, T, 1)
#define level_push_struct_no_zero(T) (T*)push_array_no_zero(gp->level_arena, T, 1)

#define frame_push_array_no_zero(T, n) (T*)push_array_no_zero(gp->frame_arena, T, n)
#define level_push_array_no_zero(T, n) (T*)push_array_no_zero(gp->level_arena, T, n)

func Game* game_init(void) {

  SetRandomSeed(42);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1000, 800, GAME_TITLE);
  //InitAudioDevice();
  //DisableCursor();

  //SetMasterVolume(GetMasterVolume() * 0.5);

  SetTargetFPS(TARGET_FPS);
  SetTextLineSpacing(text_line_spacing);
  SetTraceLogLevel(LOG_DEBUG);
  SetExitKey(0);

  Game *gp = os_alloc(sizeof(Game));
  memory_set(gp, 0, sizeof(Game));

  gp->main_arena  = arena_alloc(MB(3));
  gp->level_arena = arena_alloc(KB(16));
  gp->frame_arena = arena_alloc(KB(16));

  gp->ui = ui_init();

  game_load_assets(gp);

  game_reset(gp);

  return gp;
}

func void game_load_assets(Game *gp) {

}

func void game_unload_assets(Game *gp) {

}

func void game_close(Game *gp) {
  game_unload_assets(gp);

  ui_close(gp->ui);

  CloseWindow();
  //CloseAudioDevice();
}

func void game_reset(Game *gp) {

  arena_clear(gp->main_arena);
  arena_clear(gp->frame_arena);
  arena_clear(gp->level_arena);

  Vector2 list_window_positions[] = {
    { 40, 40 },
    { 300, 100 },
  };

  Str8 list_items[2][6] = {
    {
      str8_lit("the red##item0"),
      str8_lit("fox##item1"),
      str8_lit("jumped over##item2"),
      str8_lit("jumped over##item12"),
      str8_lit("the##item3"),
      str8_lit("cat##item4"),
    },
    {
      str8_lit("Magnum est##item5"),
      str8_lit("Imperium##item6"),
      str8_lit("jumped over##item13"),
      str8_lit("Romanum##item7"),
      str8_lit("Super##item8"),
      str8_lit("fluvius##item9"),
    },
  };

  for(int i = 0; i < ARRLEN(gp->item_lists); i++) {
    Item_list *list = &gp->item_lists[i];

    list->pos = list_window_positions[i];

    list->id = str8f(gp->main_arena, "##item_list_window_%i", i);

    for(int j = 0; j < ARRLEN(list_items[i]); j++) {
      Item_node *node = push_struct(gp->main_arena, Item_node);
      node->text = str8_copy(gp->main_arena, list_items[i][j]);
      dll_push_back(list->first, list->last, node);
    }

  }

}

func void ui_spacer(UI_context *ui, f32 size) {
  ui_flags(0)
    ui_semantic_size(((UI_size){ .kind = UI_SIZE_PIXELS, .value = size, .strictness = 1.0f }))
    ui_make_transient_box(ui);
}

func UI_signal item_button(Game *gp, Item_node *item) {
  UI_context *ui = gp->ui;

  UI_signal sig = {0}; 

  ui_border_size(1.0f)
    ui_text_align(UI_TEXT_ALIGN_CENTER)
    ui_padding(4.0f)
    ui_font_size(20.0f)
    ui_font_spacing(2.0f)

    sig = ui_button(gp->ui, item->text);

  return sig;
}

func UI_signal item_list_window(Game *gp, Item_list *list) {
  UI_context *ui = gp->ui;

  UI_box_flag flags =
    UI_BOX_FLAG_FLOATING |
    UI_BOX_FLAG_DRAW_BACKGROUND |
    UI_BOX_FLAG_MOUSE_CLICKABLE |
    UI_BOX_FLAG_DROP_SITE |
    0;

  UI_box *box;
  UI_size width = { .kind = UI_SIZE_PIXELS, .value = 200.0f, };
  UI_size height = { .kind = UI_SIZE_CHILDREN_SUM };

  ui_child_layout_axis(UI_AXIS_Y)
    ui_semantic_width(width) ui_semantic_height(height)
    ui_floating_position(list->pos)

    box = ui_make_box_from_str(gp->ui, flags, list->id);

  UI_signal sig = ui_signal_from_box(gp->ui, box);

  Color background_color = box->background_color;

  if(gp->dragging_item) {
    if(ui_key_match(ui_hot_box_key(ui), box->key)) {
      box->background_color = ColorBrightness(background_color, 0.15f);
    }
  }

  return sig;
}

func UI_signal ui_button(UI_context *ui, Str8 label) {

  UI_box_flag flags =
    UI_BOX_FLAG_DRAW_BACKGROUND |
    UI_BOX_FLAG_DRAW_TEXT |
    UI_BOX_FLAG_DRAW_BORDER |
    UI_BOX_FLAG_MOUSE_CLICKABLE |
    0;

  UI_box *box = ui_make_box_from_str(ui, flags, label);

  UI_signal sig = ui_signal_from_box(ui, box);

  Color background_color = box->background_color;

  if(ui_key_match(ui_hot_box_key(ui), box->key)) {
    _nocheckin += 1;
    box->background_color = ColorBrightness(background_color, 0.12f);
  }

  if(ui_key_match(ui_active_box_key(ui, UI_MOUSE_BUTTON_LEFT), box->key)) {
    box->background_color = ColorBrightness(background_color, -0.17f);
  }

  return sig;
}

func void game_update_and_draw(Game *gp) {

  if(IsKeyPressed(KEY_F5)) {
    game_reset(gp);
  }

  _nocheckin = 0;

  UI_context *ui = gp->ui;

  gp->dt = GetFrameTime();

  if(WindowShouldClose()) {
    gp->quit = 1;
    return;
  }


  Color background_color = { 83, 82, 99, 255 };
  Color border_color = { 53, 52, 69, 255 };
  Color text_color = ColorBrightness(RAYWHITE, 0.7f);

  ui_build(ui) {

#if 1
    Color window_background_color = ColorBrightness(BLUE, -0.4f);


    for(int list_i = 0; list_i < ARRLEN(gp->item_lists); list_i++) {
      Item_list *list = &gp->item_lists[list_i];

      UI_signal window_sig;

      ui_background_color(window_background_color)
        window_sig = item_list_window(gp, list);

      list->sig = window_sig;

      UI_box *window_box = window_sig.box;

        ui_background_color(background_color) ui_border_color(border_color) ui_text_color(text_color)
        {

          ui_parent(window_box)
            for(Item_node *item = list->first, *next = 0; item; item = next) {
              next = item->next;

              UI_signal item_sig;

              UI_size width = { .kind = UI_SIZE_PERCENT_OF_PARENT, .value = 1.0, .strictness = 0 };
              UI_size height = { .kind = UI_SIZE_TEXT_CONTENT, .value = 4.0, .strictness = 0 };

              ui_semantic_width(width) ui_semantic_height(height)
                item_sig = item_button(gp, item);

              UI_box *item_box = item_sig.box;

              if(item_sig.flags & UI_SIGNAL_FLAG_LEFT_MOUSE_DRAG && Vector2LengthSqr(ui_drag_delta(ui)) > 0) {
                gp->dragging_item = item;
                gp->dragging_item_pos.x = item_box->final_rect_min[0];
                gp->dragging_item_pos.y = item_box->final_rect_min[1];
                gp->draggin_item_size[0] = item_box->fixed_size[0];
                gp->draggin_item_size[1] = item_box->fixed_size[1];
                dll_remove(list->first, list->last, item);
              }
            }

        }

    }

    if(gp->dragging_item) {

      UI_signal dragging_sig;

      ui_flags(UI_BOX_FLAG_FLOATING) ui_floating_position(Vector2Add(gp->dragging_item_pos, ui_drag_delta(ui)))
        ui_background_color(background_color) ui_border_color(border_color) ui_text_color(text_color)
        ui_fixed_width(gp->draggin_item_size[0]) ui_fixed_height(gp->draggin_item_size[1])
        dragging_sig = item_button(gp, gp->dragging_item);

      if(dragging_sig.flags & UI_SIGNAL_FLAG_LEFT_MOUSE_RELEASE) {
        for(int list_i = 0; list_i < ARRLEN(gp->item_lists); list_i++) {
          Item_list *list = &gp->item_lists[list_i];

          if(ui_key_match(list->sig.box->key, ui_drop_hot_box_key(ui))) {
            Item_node *dropped_item = gp->dragging_item;
            dll_push_back(list->first, list->last, dropped_item);
          }

        }
        gp->dragging_item = 0;
      }

    }

#endif

#if 0
#if 1
    Vector2 window_pos1 =
    {
      .x = 20,
      .y = 10,
    };


    ui_flags(UI_BOX_FLAG_FLOATING) ui_floating_position(window_pos1)
      ui_semantic_size(((UI_size){.kind = UI_SIZE_CHILDREN_SUM}))
      ui_child_layout_axis(UI_AXIS_Y)
      {
        UI_box *container1 = ui_make_box_from_str(ui, 0, str8_lit("##container1"));

        ui_parent(container1)
          ui_flags(UI_BOX_FLAG_DRAW_BACKGROUND|UI_BOX_FLAG_DRAW_BORDER|UI_BOX_FLAG_DRAW_TEXT)
          ui_background_color(background_color) ui_border_color(border_color) ui_text_color(text_color)
          ui_semantic_size(((UI_size){ .kind = UI_SIZE_TEXT_CONTENT, .value = 4.0, .strictness = 0 }))
          ui_text_align((UI_text_align){ UI_TEXT_ALIGN_CENTER })
          ui_padding(4.0f)
          ui_font_size(20)
          ui_font_spacing(2.0f)
          ui_border_size(1.0f)
          {
            ui_make_box_from_str(ui, 0, str8_lit("hello world##0_0"));
            ui_make_box_from_str(ui, 0, str8_lit("my##0_1"));
            ui_make_box_from_str(ui, 0, str8_lit("name##0_2"));
            ui_make_box_from_str(ui, 0, str8_lit("is##0_3"));
            ui_make_box_from_str(ui, 0, str8_lit("joao##0_4"));

          }

      }
#endif

#if 1
    ui_child_layout_axis(1)
      ui_semantic_width(((UI_size){.kind = UI_SIZE_PIXELS, .value = 300}))
      ui_semantic_height(((UI_size){.kind = UI_SIZE_PIXELS, .value = 250}))
      ui_border_color(RED) ui_border_size(2.0f)
      ui_flags(UI_BOX_FLAG_FLOATING | UI_BOX_FLAG_CLIP | 0) ui_floating_position(window_pos2)
      {
        UI_box *container2 = ui_make_box_from_str(ui, 0, str8_lit("##container2"));

        UI_size child_width = { .kind = UI_SIZE_PERCENT_OF_PARENT, .value = 1.0f, .strictness = 0.7, };
        //UI_size child_height = { .kind = UI_SIZE_PERCENT_OF_PARENT, .value = 0.23f, .strictness = 0.7, };
        UI_size child_height = { .kind = UI_SIZE_TEXT_CONTENT };
        ui_parent(container2)
          ui_flags(UI_BOX_FLAG_DRAW_BACKGROUND|UI_BOX_FLAG_DRAW_BORDER|UI_BOX_FLAG_DRAW_TEXT|UI_BOX_FLAG_SCROLL)
          ui_background_color(background_color) ui_border_color(border_color) ui_text_color(text_color)
          ui_semantic_width(child_width) ui_semantic_height(child_height)
          ui_text_align(UI_TEXT_ALIGN_CENTER)
          ui_corner_radius(0.5f)
          //ui_corner_radius_0(0.8f)
          //ui_corner_radius_3(0.8f)
          //ui_corner_radius_2(0.8f)
          ui_font_size(20)
          ui_font_spacing(2.0f)
          ui_border_size(2.0f)
          {
            f32 spacing = 8;
            ui_semantic_height(((UI_size){ .kind = UI_SIZE_TEXT_CONTENT, .value = 4.0f, .strictness = 1.0f }))
              ui_background_color(BLUE) ui_text_color(GOLD) ui_text_align(UI_TEXT_ALIGN_LEFT)
              {
                UI_signal sig = ui_button(ui, str8_lit("Drag Me!##drag_bar"));

                if(sig.flags & UI_SIGNAL_FLAG_LEFT_MOUSE_PRESS) {
                  save_window_pos2 = window_pos2;
                }

                if(sig.flags & UI_SIGNAL_FLAG_LEFT_MOUSE_DRAG) {
                  window_pos2 = Vector2Add(save_window_pos2, ui_drag_delta(ui));
                }

              }
            ui_spacer(ui, spacing);

            ui_button(ui, str8_lit("hello world##1_0"));
            ui_spacer(ui, spacing);
            ui_button(ui, str8_lit("my##1_1"));
            ui_spacer(ui, spacing);
            ui_button(ui, str8_lit("name##1_29123891"));
            ui_spacer(ui, spacing);
            ui_button(ui, str8_lit("is##1_3"));
            ui_spacer(ui, spacing);
            ui_button(ui, str8_lit("joao##1_4"));

          }

      }
#endif
#endif

  }

  defer_loop(BeginDrawing(), EndDrawing()) {
    if(ui->took_input_event) {
      ClearBackground(ColorBrightness(RED, -0.4));
    } else {
      ClearBackground(BLACK);
    }

    ui_draw(ui);

#if 1
    { /* debug overlay */
#define DEBUG_OVERLAY_TABLE \
      /* string                                         fmt        expr */  \
      X( frame time,                                    %.7f,      gp->dt) \
      X( screen width,                                  %i,        GetScreenWidth()) \
      X( screen height,                                 %i,        GetScreenHeight()) \
      X( time to calc text and pixel sized boxes,       %f,        ui_prof_time_to_calc_absolute_sized_boxes) \
      X( time to calc percent parent sized boxes,       %f,        ui_prof_time_to_calc_percent_sized_boxes) \
      X( time to calc child sum sized boxes,            %f,        ui_prof_time_to_calc_sum_sized_boxes) \
      X( time to do 1st pass,                           %f,        ui_prof_time_to_do_1st_pass) \
      X( time to solve size violations,                 %f,        ui_prof_time_to_solve_size_violations) \
      X( time to calc final positions,                  %f,        ui_prof_time_to_calc_positions) \
      X( ui hot box key,                                %lu,       ui->hot_box_key.hash) \
      // \


      char *debug_text_fmt =
#define X(string, fmt, expr) #string": "#fmt"\n"
        DEBUG_OVERLAY_TABLE"%c";
#undef X
      Str8 debug_text = str8f(gp->frame_arena, debug_text_fmt,
#define X(string, fmt, expr) (expr),
          DEBUG_OVERLAY_TABLE 0);
#undef X
      Vector2 debug_text_size = MeasureTextEx(GetFontDefault(), (char*)debug_text.s, 20, 1.0);
      DrawText((char*)debug_text.s, 10, GetScreenHeight() - debug_text_size.y + 20, 20, GREEN);
    } /* debug overlay */
#endif

  }

  arena_clear(gp->frame_arena);

}

