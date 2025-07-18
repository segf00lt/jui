#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "basic.h"
#include "arena.h"
#include "str.h"
#include "context.h"
#include "array.h"
#include "sprite.h"
#include "stb_sprintf.h"

#include "ui.c"

#undef force_inline

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

/*
 * entity settings
 */


/*
 * generated code
 */


/*
 * globals
 */

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

#define frame_scope_begin() scope_begin(gp->frame_arena)
#define frame_scope_end(scope) scope_end(scope)

#define level_scope_begin() scope_begin(gp->level_arena)
#define level_scope_end(scope) scope_end(scope)

Game* game_init(void) {

  SetRandomSeed(42);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1000, 800, GAME_TITLE);
  //InitAudioDevice();
  //DisableCursor();

  //SetMasterVolume(GetMasterVolume() * 0.5);

  SetTargetFPS(TARGET_FPS);
  SetTextLineSpacing(10);
  SetTraceLogLevel(LOG_DEBUG);
  SetExitKey(0);

  Game *gp = os_alloc(sizeof(Game));
  memory_set(gp, 0, sizeof(Game));

  gp->main_arena  = arena_alloc(.size = MB(3));
  gp->level_arena = arena_alloc(.size = KB(16));
  gp->frame_arena = arena_alloc(.size = KB(16));

  gp->ui = ui_init();

  game_load_assets(gp);

  game_reset(gp);

  return gp;
}

void game_load_assets(Game *gp) {

}

void game_unload_assets(Game *gp) {

}

void game_close(Game *gp) {
  game_unload_assets(gp);

  ui_close(gp->ui);

  CloseWindow();
  //CloseAudioDevice();
}

void game_reset(Game *gp) {

  arena_clear(gp->main_arena);
  arena_clear(gp->frame_arena);
  arena_clear(gp->level_arena);

}

void ui_spacer(UI_context *ui, f32 size) {
  ui_flags(0)
    ui_semantic_size(((UI_size){ .kind = UI_SIZE_PIXELS, .value = size, .strictness = 1.0f }))
    ui_make_transient_box(ui);
}

UI_signal ui_button(UI_context *ui, Str8 label) {

  UI_box_flag flags =
    UI_BOX_FLAG_MOUSE_CLICKABLE |
    0;

  UI_box *box = ui_make_box_from_str(ui, flags, label);

  UI_signal sig = ui_signal_from_box(ui, box);

  Color background_color = box->background_color;

  if(ui_key_match(ui_hot_box_key(ui), box->key)) {
    box->background_color = ColorBrightness(background_color, 0.12f);
  }

  if(ui_key_match(ui_active_box_key(ui, UI_MOUSE_BUTTON_LEFT), box->key)) {
    box->background_color = ColorBrightness(background_color, -0.17f);
  }

  return sig;
}

void game_update_and_draw(Game *gp) {

  gp->dt = GetFrameTime();

  if(WindowShouldClose()) {
    gp->quit = 1;
    return;
  }


  //Vector2 mouse_position = GetMousePosition(); Vector2 scroll_delta = GetMouseWheelMoveV();

  Color background_color = { 83, 82, 99, 255 };
  Color border_color = { 53, 52, 69, 255 };
  Color text_color = ColorBrightness(RAYWHITE, 0.8f);

  UI_context *ui = gp->ui;

  ui_build(ui) {

#if 1
    Vector2 window_pos1 =
    {
      .x = 20,
      .y = 10,
    };


    ui_flags(UI_BOX_FLAG_IS_FLOATING) ui_floating_position(window_pos1)
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
      ui_flags(UI_BOX_FLAG_IS_FLOATING | UI_BOX_FLAG_CLIP | 0) ui_floating_position(window_pos2)
      {
        UI_box *container2 = ui_make_box_from_str(ui, 0, str8_lit("##container2"));

        UI_size child_width = { .kind = UI_SIZE_PERCENT_OF_PARENT, .value = 1.0f, .strictness = 0.7, };
        UI_size child_height = { .kind = UI_SIZE_PERCENT_OF_PARENT, .value = 0.23f, .strictness = 0.7, };
        //UI_size child_height = { .kind = UI_SIZE_TEXT_CONTENT };
        ui_parent(container2)
          ui_flags(UI_BOX_FLAG_DRAW_BACKGROUND|UI_BOX_FLAG_DRAW_BORDER|UI_BOX_FLAG_DRAW_TEXT|UI_BOX_FLAG_SCROLL)
          ui_background_color(background_color) ui_border_color(border_color) ui_text_color(text_color)
          ui_semantic_width(child_width) ui_semantic_height(child_height)
          ui_text_align(UI_TEXT_ALIGN_CENTER)
          ui_corner_radius(0.1f)
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
                  window_pos2 = Vector2Add(save_window_pos2, ui_mouse_drag_delta(ui));
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
      // \


      char *debug_text_fmt =
#define X(string, fmt, expr) #string": "#fmt"\n"
        DEBUG_OVERLAY_TABLE"%c";
#undef X
      Str8 debug_text = push_str8f(gp->frame_arena, debug_text_fmt,
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

