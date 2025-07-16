
#define MAX_TABLE_SLOTS 64
#define UI_ARR_PREFIX UI_Arr_


/*
 * tables
 */

#define UI_CONTEXT_FLAGS \

#define UI_MOUSE_BUTTON_ACTIONS \
  X(LEFT_MOUSE_CLICK) \
  X(LEFT_MOUSE_PRESS) \
  X(LEFT_MOUSE_RELEASE) \
  X(RIGHT_MOUSE_CLICK) \
  X(RIGHT_MOUSE_PRESS) \
  X(RIGHT_MOUSE_RELEASE) \
  X(MIDDLE_MOUSE_CLICK) \
  X(MIDDLE_MOUSE_PRESS) \
  X(MIDDLE_MOUSE_RELEASE) \

#define UI_INPUT_FLAGS \
  UI_MOUSE_BUTTON_ACTIONS \
  X(MOUSE_SCROLLED) \
  X(KEYBOARD_PRESSED) \

#define UI_SIGNAL_FLAGS \
  UI_MOUSE_BUTTON_ACTIONS \
  \
  X(LEFT_MOUSE_DRAG) \
  X(RIGHT_MOUSE_DRAG) \
  X(MIDDLE_MOUSE_DRAG) \
  \
  X(MOUSE_HOVERING) \
  X(MOUSE_OVER) \


#define UI_BOX_FLAGS \
  X(IS_FLOATING) \
  X(DRAW_TEXT) \
  X(DRAW_BACKGROUND) \
  X(DRAW_BORDER) \
  X(CLIP) \
  X(OVERFLOW_X) \
  X(OVERFLOW_Y) \


#define UI_STYLE_PROPERTIES \
  /* upper                camel              lower                type              init                                stack_size */ \
  X( BACKGROUND_COLOR,    BackgroundColor,   background_color,    Color,            ((Color){ 110, 110, 110, 255 }),    8           ) \
  X( TEXT_COLOR,          TextColor,         text_color,          Color,            ((Color){ 248, 248, 248, 255 }),    8           ) \
  X( BORDER_COLOR,        BorderColor,       border_color,        Color,            ((Color){ 180, 180, 180, 255 }),    8           ) \
  X( BORDER_SIZE,         BorderSize,        border_size,         f32,              ((f32)1.0f),                        8           ) \
  X( CORNER_RADIUS_0,     CornerRadius0,     corner_radius_0,     f32,              ((f32)0.0f),                        8           ) \
  X( CORNER_RADIUS_1,     CornerRadius1,     corner_radius_1,     f32,              ((f32)0.0f),                        8           ) \
  X( CORNER_RADIUS_2,     CornerRadius2,     corner_radius_2,     f32,              ((f32)0.0f),                        8           ) \
  X( CORNER_RADIUS_3,     CornerRadius3,     corner_radius_3,     f32,              ((f32)0.0f),                        8           ) \
  X( FONT_ID,             FontId,            font_id,             u32,              ((u32)0),                           4           ) \
  X( FONT_SIZE,           FontSize,          font_size,           f32,              ((f32)10.0f),                       8           ) \
  X( FONT_SPACING,        FontSpacing,       font_spacing,        f32,              ((f32)1.0f),                        4           ) \
  X( TEXT_ALIGN,          TextAlign,         text_align,          UI_text_align,    UI_TEXT_ALIGN_LEFT,                 16          ) \
  X( SEMANTIC_WIDTH,      SemanticWidth,     semantic_width,      UI_size,          ((UI_size){0}),                     16          ) \
  X( SEMANTIC_HEIGHT,     SemanticHeight,    semantic_height,     UI_size,          ((UI_size){0}),                     16          ) \
  X( MIN_WIDTH,           MinWidth,          min_width,           f32,              ((f32)0),                           4           ) \
  X( MIN_HEIGHT,          MinHeight,         min_height,          f32,              ((f32)0),                           4           ) \
  X( FIXED_WIDTH,         FixedWidth,        fixed_width,         f32,              ((f32)0),                           4           ) \
  X( FIXED_HEIGHT,        FixedHeight,       fixed_height,        f32,              ((f32)0),                           4           ) \
  X( PADDING,             Padding,           padding,             f32,              ((f32)2.0f),                        8           ) \
  X( FLOATING_POSITION,   FloatingPosition,  floating_position,   Vector2,          ((Vector2){0}),                     4           ) \
  X( CHILD_LAYOUT_AXIS,   ChildLayoutAxis,   child_layout_axis,   UI_axis,          ((UI_axis)0),                       64          ) \

#define UI_PROPERTIES \
  /* upper                camel              lower                type              init                                stack_size */ \
  UI_STYLE_PROPERTIES \
  X( PARENT,              Parent,            parent,              UI_box_ptr,       nil_ui_box,                         16          ) \
  X( FLAGS,               Flags,             flags,               UI_box_flag,      0,                                  16          ) \
  X( EXCLUDE_FLAGS,       ExcludeFlags,      exclude_flags,       UI_box_flag,      0,                                  16          ) \

#define ui_background_color(value) ui_prop(background_color, (value))

#define ui_text_color(value) ui_prop(text_color, (value))

#define ui_border_color(value) ui_prop(border_color, (value))
#define ui_border_size(value) ui_prop(border_size, (value))

#define ui_corner_radius_0(value) ui_prop(corner_radius_0, (value))
#define ui_corner_radius_1(value) ui_prop(corner_radius_1, (value))
#define ui_corner_radius_2(value) ui_prop(corner_radius_2, (value))
#define ui_corner_radius_3(value) ui_prop(corner_radius_3, (value))
#define ui_corner_radius(value) ui_corner_radius_0((value)) ui_corner_radius_1((value)) ui_corner_radius_2((value)) ui_corner_radius_3((value))

#define ui_font_id(value) ui_prop(font_id, (value))
#define ui_font_size(value) ui_prop(font_size, (value))
#define ui_font_spacing(value) ui_prop(font_spacing, (value))

#define ui_text_align(value) ui_prop(text_align, (value))

#define ui_semantic_width(value) ui_prop(semantic_width, (value))
#define ui_semantic_height(value) ui_prop(semantic_height, (value))
#define ui_semantic_size(value) ui_semantic_width((value)) ui_semantic_height((value))

#define ui_padding(value) ui_prop(padding, (value))

#define ui_floating_position(value) ui_prop(floating_position, (value))

#define ui_child_layout_axis(value) ui_prop(child_layout_axis, (value))

#define ui_parent(value) ui_prop(parent, (value))

#define ui_flags(value) ui_prop(flags, (value))
#define ui_exclude_flags(value) ui_prop(exclude_flags, (value))

#define DECL_UI_ARR(T) DECL_ARR_TYPE_NAME(T, UI_ARR_PREFIX##T)

/*
 * typedefs
 */

typedef struct UI_context UI_context;
typedef struct UI_box UI_box;
typedef struct UI_box_node UI_box_node;
typedef struct UI_box_list UI_box_list;
typedef struct UI_box_hash_slot UI_box_hash_slot;
typedef struct UI_key UI_key;
typedef struct UI_size UI_size;
typedef struct UI_signal UI_signal;
typedef u64 UI_input_flag;
typedef u64 UI_box_flag;
typedef u64 UI_context_flag;
typedef u64 UI_signal_flag;
typedef UI_box* UI_box_ptr;

typedef enum UI_size_kind {
  UI_SIZE_NONE = 0,
  UI_SIZE_PIXELS,
  UI_SIZE_TEXT_CONTENT,
  UI_SIZE_PERCENT_OF_PARENT,
  UI_SIZE_CHILDREN_SUM,
} UI_size_kind;

typedef enum UI_text_align {
  UI_TEXT_ALIGN_CENTER = 0,
  UI_TEXT_ALIGN_LEFT,
  UI_TEXT_ALIGN_RIGHT,
} UI_text_align;

typedef enum UI_axis {
  UI_AXIS_X = 0,
  UI_AXIS_Y,
  UI_AXIS_COUNT,
} UI_axis;

typedef enum UI_modifier_keys {
  UI_MOD_CONTROL = 0,
  UI_MOD_SHIFT,
  UI_MOD_SUPER,
  UI_MOD_COUNT,
} UI_modifier_keys;

typedef enum UI_box_flag_index {
  UI_BOX_FLAG_INDEX_NONE = -1,
#define X(f) UI_BOX_FLAG_INDEX_##f,
  UI_BOX_FLAGS
#undef X
    UI_BOX_FLAG_INDEX_MAX,
} UI_box_flags_index;

STATIC_ASSERT(UI_BOX_FLAG_INDEX_MAX < 64, UI_BOX_FLAG_INDEX_MAX__is_less_than_64);

#define X(f) const UI_box_flag UI_BOX_FLAG_##f = (1ull<<UI_BOX_FLAG_INDEX_##f);
UI_BOX_FLAGS
#undef X

typedef enum UI_context_flag_index {
  UI_CONTEXT_FLAG_INDEX_NONE = -1,
#define X(f) UI_CONTEXT_FLAG_INDEX_##f,
  UI_CONTEXT_FLAGS
#undef X
    UI_CONTEXT_FLAG_INDEX_MAX,
} UI_context_flags_index;

STATIC_ASSERT(UI_CONTEXT_FLAG_INDEX_MAX < 64, UI_CONTEXT_FLAG_INDEX_MAX__is_less_than_64);

#define X(f) const UI_context_flag UI_CONTEXT_FLAG_##f = (1ull<<UI_CONTEXT_FLAG_INDEX_##f);
UI_CONTEXT_FLAGS
#undef X

typedef enum UI_signal_flag_index {
  UI_SIGNAL_FLAG_INDEX_NONE = -1,
#define X(f) UI_SIGNAL_FLAG_INDEX_##f,
  UI_SIGNAL_FLAGS
#undef X
    UI_SIGNAL_FLAG_INDEX_MAX,
} UI_signal_flags_index;

STATIC_ASSERT(UI_SIGNAL_FLAG_INDEX_MAX < 64, UI_SIGNAL_FLAG_INDEX_MAX__is_less_than_64);

#define X(f) const UI_signal_flag UI_SIGNAL_FLAG_##f = (1ull<<UI_SIGNAL_FLAG_INDEX_##f);
UI_SIGNAL_FLAGS
#undef X

typedef enum UI_input_flag_index {
  UI_INPUT_FLAG_INDEX_NONE = -1,
#define X(f) UI_INPUT_FLAG_INDEX_##f,
  UI_INPUT_FLAGS
#undef X
    UI_INPUT_FLAG_INDEX_MAX,
} UI_input_flags_index;

STATIC_ASSERT(UI_INPUT_FLAG_INDEX_MAX < 64, UI_INPUT_FLAG_INDEX_MAX__is_less_than_64);

#define X(f) const UI_input_flag UI_INPUT_FLAG_##f = (1ull<<UI_INPUT_FLAG_INDEX_##f);
UI_INPUT_FLAGS
#undef X

DECL_UI_ARR(UI_box_ptr);
DECL_UI_ARR(Color);
DECL_UI_ARR(u8);
DECL_UI_ARR(u16);
DECL_UI_ARR(u32);
DECL_UI_ARR(u64);
DECL_UI_ARR(s8);
DECL_UI_ARR(s16);
DECL_UI_ARR(s32);
DECL_UI_ARR(s64);
DECL_UI_ARR(f32);
DECL_UI_ARR(Vector2);
DECL_UI_ARR(UI_box_flag);
DECL_UI_ARR(UI_axis);
DECL_UI_ARR(UI_size);
DECL_UI_ARR(UI_text_align);


/*
 * struct bodies
 */

struct UI_key {
  u64  hash;
  Str8 src_str;
};

struct UI_size {
  UI_size_kind kind;
  f32 value;
  f32 strictness;
};

struct UI_box {
  UI_box_flag flags;
  UI_box_flag exclude_flags;

  UI_key key;

  UI_box *hash_next;
  UI_box *hash_prev;

  UI_box *parent;
  UI_box *next;
  UI_box *prev;
  UI_box *first;
  UI_box *last;
  u64 child_count;

  /* properties */
  Color background_color;
  Color text_color;
  Color border_color;
  f32 border_size;
  union { // TODO corner radius
    struct {
      f32 corner_radius_0;
      f32 corner_radius_1;
      f32 corner_radius_2;
      f32 corner_radius_3;
    };
    f32 corner_radius[4];
  };
  u32 font_id;
  f32 font_size;
  f32 font_spacing;
  UI_text_align text_align;
  union {
    struct {
      UI_size semantic_width;
      UI_size semantic_height;
    };
    UI_size semantic_size[UI_AXIS_COUNT];
  };
  f32 padding;
  Vector2 floating_position;
  UI_axis child_layout_axis;

  Str8 text;

  union {
    struct {
      f32 min_width;
      f32 min_height;
    };
    f32 min_size[2];
  };

  union {
    struct {
      f32 fixed_width;
      f32 fixed_height;
    };
    f32 fixed_size[2];
  };

  f32 computed_size[2];
  f32 computed_rel_pos[2];
  struct {
    f32 min[2];
    f32 max[2];
  } final_rect;


  u64 first_visited_build_index;
  u64 last_visited_build_index;
  f32 view_offset[2];
  f32 view_offset_target[2];
  f32 view_bounds[2]; // dunno what ~rjf uses this for
  f32 hot_time;
  f32 active_time;
};

struct UI_signal {
  UI_signal_flag flags;
  UI_box *box;
  f32 scroll[2];
  u32 modifier_keys; // using the raylib ones for the moment
};

struct UI_box_node {
  UI_box_node *next;
  UI_box *box;
};

struct UI_box_list {
  UI_box_node *first;
  UI_box_node *last;
  s64 count;
};

struct UI_box_hash_slot {
  UI_box *first;
  UI_box *last;
};

struct UI_context {
  Arena *arena;
  Arena *temp;
  Arena *build_arena;
  Arena *draw_arena;

  u64 build_index;

  UI_box __dummy;
  UI_box *dummy;
  UI_box *root;
  u64 build_box_count;

  UI_box_hash_slot box_table[MAX_TABLE_SLOTS];

  Font fonts[8];

  UI_input_flag input_flags;
  Vector2 mouse_pos;
  Vector2 mouse_pos_delta;
  Vector2 scroll_delta;
  u32 keyboard_key_pressed;
  b64 received_interaction;

#define X(upper, camel, lower, type, init, stack_size) UI_ARR_PREFIX##type lower##_stack;
  struct {
    UI_PROPERTIES
  };
#undef X

  UI_box *box_free_list;
};

/*
 * function headers
 */

UI_context* ui_init(void);
void        ui_close(UI_context *ui);

UI_key ui_key_nil(void);
UI_key ui_key_from_str(Str8 str);
b32 ui_key_match(UI_key a, UI_key b);

UI_box* ui_get_box_from_key(UI_context *ui, UI_key key);
UI_box* ui_get_box_from_str(UI_context *ui, UI_key key);

UI_signal ui_signal_from_box(UI_context *ui, UI_box *box);

Str8 ui_strip_id_from_text(Str8 text);

UI_box* ui_make_box_from_key(UI_context *ui, UI_key key);
UI_box* ui_make_box_from_str(UI_context *ui, Str8 str);
UI_box* ui_make_box_from_strf(UI_context *ui, char *fmt, ...);
UI_box* ui_make_transient_box(UI_context *ui);

UI_box_node* ui_push_box_node(UI_context *ui);

f32 ui_calc_downward_dependent_sizes(UI_box *box, int axis, int layout_axis);

void ui_begin_build(UI_context *ui);
void ui_end_build(UI_context *ui);
void ui_draw(UI_context *ui);
void ui_get_input(UI_context *ui);

void ui_draw_rectangle_rounded(Rectangle rec, f32 r0, f32 r1, f32 r2, f32 r3, Color color);
//void ui_draw_rectangle_lines_rounded(Rectangle rec, f32 roundness, int segments, Color color);

/*
 * macros
 */

#define ui_build(ui) defer_loop(ui_begin_build((ui)), ui_end_build((ui)))
#define ui_push_prop(name, value) arr_push(ui->name##_stack, (value))
#define ui_pop_prop(name) arr_pop(ui->name##_stack)
#define ui_clear_prop(name) arr_clear(ui->name##_stack)
#define ui_prop(name, value) defer_loop(ui_push_prop(name, (value)), ui_pop_prop(name))

/*
 * globals
 */

//s64 max_dumps = 7;

/*
 * profiling
 */

#define UI_PROFILE(dest) for(f32 __i__ = 0, __prof_time = (f32)GetTime(); __i__ == 0; dest = (f32)GetTime() - __prof_time, __i__ = 1.0f)

f32 ui_prof_time_to_do_1st_pass;
f32 ui_prof_time_to_calc_absolute_sized_boxes;
f32 ui_prof_time_to_calc_percent_sized_boxes;
f32 ui_prof_time_to_calc_sum_sized_boxes;
f32 ui_prof_time_to_solve_size_violations;
f32 ui_prof_time_to_calc_positions;


/*
 * function bodies
 */

UI_context* ui_init(void) {
  
  UI_context *ui = os_alloc(sizeof(UI_context));

  memory_zero(ui, sizeof(UI_context));

  ui->arena       = arena_alloc(.size = MB(1));
  ui->temp        = arena_alloc(.size = KB(8));
  ui->build_arena = arena_alloc(.size = KB(16));
  ui->draw_arena  = arena_alloc(.size = KB(16));

#define X(upper, camel, lower, type, init, stack_size) \
  arr_init_ex(ui->lower##_stack, ui->arena, stack_size);
  UI_PROPERTIES;
#undef X

  ui->fonts[0] = GetFontDefault();

  ui->dummy = &ui->__dummy;

  return ui;
}

void ui_close(UI_context *ui) {
  arena_free(ui->arena);
  arena_free(ui->build_arena);
  arena_free(ui->temp);
  arena_free(ui->draw_arena);
  os_free(ui);
}

force_inline UI_key ui_key_nil(void) {
  return (UI_key){0};
}

UI_key ui_key_from_str(Str8 str) {

  UI_key key = { .hash = 0, .src_str = str };

  { /* hash */
    for(int i = 0; i < str.len; i++) {
      key.hash += str.s[i] * 37;
    }
  } /* hash */

  return key;
}

b32 ui_key_match(UI_key a, UI_key b) {
  b32 match = 1;

  if(a.hash != b.hash) {
    match = 0;
  } else {
    if(!str8_match(a.src_str, b.src_str)) {
      match = 0;
    }
  }

  return match;
}

UI_box* ui_get_box_from_key(UI_context *ui, UI_key key) {
  UI_box *result = 0;

  if(!ui_key_match(key, ui_key_nil())) {
    u64 slot = key.hash % MAX_TABLE_SLOTS;

    for(UI_box *b = ui->box_table[slot].first; b; b = b->hash_next) {
      if(ui_key_match(key, b->key)) {
        result = b;
        break;
      }
    }

  }

  return result;
}

UI_box* ui_make_transient_box(UI_context *ui) {
  return ui_make_box_from_key(ui, ((UI_key){0}));
}

UI_box* ui_make_box_from_key(UI_context *ui, UI_key key) {

#define X(upper, camel, lower, type, init, stack_size) type cur_##lower = arr_last(ui->lower##_stack);
  UI_PROPERTIES;
#undef X

  UI_box *box = ui_get_box_from_key(ui, key);

  b32 is_box_first_frame = !box;

  //UI_box_flag last_flags = box->flags;

  b32 box_is_transient = ui_key_match(key, ui_key_nil());

  if(is_box_first_frame) {
    box = !box_is_transient ? ui->box_free_list : 0;
    if(box) {
      sll_stack_pop(ui->box_free_list);
    } else {
      box = push_struct_no_zero(box_is_transient ? ui->build_arena : ui->arena, UI_box);
    }
    memory_zero(box, sizeof(UI_box));
  }

  {
    box->first = box->last = box->next = box->prev = box->parent = 0;
    box->child_count = 0;
    box->flags = 0;
    memory_zero(box->semantic_size, sizeof(box->semantic_size));
  }

  if(is_box_first_frame && !box_is_transient) {
    box->first_visited_build_index = ui->build_index;
    u64 slot = key.hash % ARRLEN(ui->box_table);
    dll_push_back(ui->box_table[slot].first, ui->box_table[slot].last, box);
  }

  if(cur_parent) {
    dll_push_back(cur_parent->first, cur_parent->last, box);
    cur_parent->child_count += 1;
    box->parent = cur_parent;
  }

  box->key = key;
  box->last_visited_build_index = ui->build_index;

#define X(upper, camel, lower, type, init, stack_size) box->lower = cur_##lower;
  UI_PROPERTIES;
#undef X

  return box;
}

Str8 ui_strip_id_from_text(Str8 text) {
  s64 index = str8_find(text, str8_lit("##"));
  if(index >= 0) {
    text.len = index;
  }
  return text;
}

UI_box* ui_make_box_from_str(UI_context *ui, Str8 str) {
  UI_key key = ui_key_from_str(str);

  UI_box *box = ui_make_box_from_key(ui, key);

  if(box->flags & UI_BOX_FLAG_DRAW_TEXT) {
    box->text = ui_strip_id_from_text(str);
  }

  return box;
}

UI_box* ui_make_box_from_strf(UI_context *ui, char *fmt, ...) {
  Arena_scope scope = scope_begin(ui->temp);

  va_list args;
  va_start(args, fmt);
  Str8 str = push_str8fv(ui->temp, fmt, args);
  va_end(args);

  UI_box *box = ui_make_box_from_str(ui, str);

  scope_end(scope);

  return box;
}

UI_signal ui_signal_from_box(UI_context *ui, UI_box *box) {
  UI_signal result = {0};

  // TODO box stacking one on top of the other

  Rectangle box_rec =
  {
    .x = box->final_rect.min[0],
    .y = box->final_rect.min[1],
    .width =  box->final_rect.max[0] - box->final_rect.min[0],
    .height = box->final_rect.max[1] - box->final_rect.min[1],
  };

  for(UI_box *parent = box->parent; parent; parent = parent->parent) {
    if(parent->flags & UI_BOX_FLAG_CLIP) {
      Rectangle clip_rec =
      {
        .x = parent->final_rect.min[0], .y = parent->final_rect.min[1],
        .width = parent->final_rect.max[0] - parent->final_rect.min[0],
        .height = parent->final_rect.max[1] - parent->final_rect.min[1],
      };

      box_rec = GetCollisionRec(box_rec, clip_rec);
    }
  }

  b32 mouse_in_bounds = CheckCollisionPointRec(ui->mouse_pos, box_rec);

  if(mouse_in_bounds) {
    result.flags |= UI_SIGNAL_FLAG_MOUSE_HOVERING | UI_SIGNAL_FLAG_MOUSE_OVER;

#define X(action) if(ui->input_flags & UI_INPUT_FLAG_##action) { result.flags |= UI_SIGNAL_FLAG_##action; }
    UI_MOUSE_BUTTON_ACTIONS;
#undef X

#define X(button) \
    if(ui->input_flags & UI_INPUT_FLAG_##button##_MOUSE_PRESS) { \
      if(ui->mouse_pos_delta.x != 0 && ui->mouse_pos_delta.y != 0) { \
        result.flags |= UI_SIGNAL_FLAG_##button##_MOUSE_DRAG; \
      } \
    }
    X(LEFT);
    X(RIGHT);
    X(MIDDLE);
#undef X

  }

  return result;
}

void ui_get_input(UI_context *ui) {
  ui->mouse_pos = GetMousePosition();
  ui->mouse_pos_delta = GetMouseDelta();
  ui->scroll_delta = GetMouseWheelMoveV();
  ui->keyboard_key_pressed = PeekKeyPressed();

  ui->input_flags =
    (UI_INPUT_FLAG_LEFT_MOUSE_CLICK & -IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) |
    (UI_INPUT_FLAG_LEFT_MOUSE_PRESS & -IsMouseButtonDown(MOUSE_LEFT_BUTTON)) |
    (UI_INPUT_FLAG_LEFT_MOUSE_RELEASE & -IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) |
    (UI_INPUT_FLAG_RIGHT_MOUSE_CLICK & -IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) |
    (UI_INPUT_FLAG_RIGHT_MOUSE_PRESS & -IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) |
    (UI_INPUT_FLAG_RIGHT_MOUSE_RELEASE & -IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) |
    (UI_INPUT_FLAG_MIDDLE_MOUSE_CLICK & -IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) |
    (UI_INPUT_FLAG_MIDDLE_MOUSE_PRESS & -IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) |
    (UI_INPUT_FLAG_MIDDLE_MOUSE_RELEASE & -IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON)) |
    ((ui->scroll_delta.x != 0.0f && ui->scroll_delta.y != 0.0f) ? UI_INPUT_FLAG_MOUSE_SCROLLED : 0) |
    ((ui->keyboard_key_pressed != 0) ? UI_INPUT_FLAG_KEYBOARD_PRESSED : 0) |
    0;

}

void ui_begin_build(UI_context *ui) {
  arena_clear(ui->build_arena);

  ui->build_index++;

  UI_box *dummy = ui->dummy;
  memory_zero(dummy, sizeof(UI_box));

  ui->root = dummy;

  ui_push_prop(parent, dummy);
  ui_push_prop(flags, UI_BOX_FLAG_IS_FLOATING);
  ui_push_prop(semantic_width,  ((UI_size){ .kind = UI_SIZE_NONE, }));
  ui_push_prop(semantic_height, ((UI_size){ .kind = UI_SIZE_NONE, }));
  ui_push_prop(text_align, (UI_text_align){ UI_TEXT_ALIGN_LEFT });
  ui_push_prop(font_size, 10);
  ui_push_prop(font_spacing, 1);

  ui_get_input(ui);

}

// TODO make an iterative version of this
f32 ui_calc_downward_dependent_sizes(UI_box *box, int axis, int layout_axis) {
  f32 sum = 0;

  if(box->first) {

    sum = ui_calc_downward_dependent_sizes(box->first, axis, box->child_layout_axis);

    if(box->semantic_size[axis].kind == UI_SIZE_CHILDREN_SUM) {
      box->computed_size[axis] = sum;
    } else {
      sum += box->computed_size[axis];
    }

  } else {

    if(axis == layout_axis) {
      if(!(box->flags & UI_BOX_FLAG_IS_FLOATING)) {
        sum += box->computed_size[axis];
      }

      if(box->next) {
        sum += ui_calc_downward_dependent_sizes(box->next, axis, layout_axis);
      }
    } else {
      if(!(box->flags & UI_BOX_FLAG_IS_FLOATING)) {
        sum = MAX(box->computed_size[axis], sum);
      }

      if(box->next) {
        sum = MAX(ui_calc_downward_dependent_sizes(box->next, axis, layout_axis), sum);
      }
    }

  }

  return sum;
}

void ui_end_build(UI_context *ui) {

  // TODO why not just clear all the stacks??
#define X(upper, camel, lower, type, init, stack_size) ui_clear_prop(lower);
  UI_PROPERTIES;
#undef X

  /* prune box tree */
  for(u64 table_i = 0; table_i < ARRLEN(ui->box_table); table_i++) {
    UI_box *box = ui->box_table[table_i].first;
    for(; box; box = box->hash_next) {
      if(box->last_visited_build_index < ui->build_index || ui_key_match(box->key, ui_key_nil())) {
        UNREACHABLE;
        dll_remove(
            ui->box_table[table_i].first,
            ui->box_table[table_i].last,
            box);
        sll_stack_push(ui->box_free_list, box);
      }
    }
  }

  for(int axis = 0; axis < 2; axis++) 
    UI_PROFILE(ui_prof_time_to_do_1st_pass) {

      UI_PROFILE(ui_prof_time_to_calc_absolute_sized_boxes) { /* visit nodes with UI_SIZE_PIXELS or UI_SIZE_TEXT_CONTENT */

        for(UI_box *node = ui->root; node;) {

          UI_size size = node->semantic_size[axis];

          switch(size.kind) {
            default:
              break;
            case UI_SIZE_PIXELS:
              {
                node->computed_size[axis] = size.value;
              } break;
            case UI_SIZE_TEXT_CONTENT:
              {
                Arena_scope scope = scope_begin(ui->build_arena);

                Vector2 text_size =
                  MeasureTextEx(
                      ui->fonts[node->font_id],
                      push_cstr_copy_str8(ui->build_arena, node->text),
                      node->font_size,
                      node->font_spacing);

                f32 text_size_on_axis = (&(text_size.x))[axis];

                f32 padding = size.value + TIMES2(node->padding);

                node->computed_size[axis] = text_size_on_axis + padding;

                scope_end(scope);
              } break;
          }

          if(node->first) {
            node = node->first;
          } else if(node->next) {
            node = node->next;
          } else {
            while(node && !(node->next)) {
              node = node->parent;
            }
            if(node) node = node->next;
          }

        }

      } /* visit nodes with UI_SIZE_PIXELS or UI_SIZE_TEXT_CONTENT */

      UI_PROFILE(ui_prof_time_to_calc_percent_sized_boxes) { /* visit nodes with UI_SIZE_PERCENT_OF_PARENT in pre order */

        for(UI_box *node = ui->root; node;) {

          UI_size size = node->semantic_size[axis];

          if(size.kind == UI_SIZE_PERCENT_OF_PARENT) {
            if(node->parent) {
              node->computed_size[axis] = size.value * node->parent->computed_size[axis];
            }
          }

          if(node->first) {
            node = node->first;
          } else if(node->next) {
            node = node->next;
          } else {
            while(node && !(node->next)) {
              node = node->parent;
            }
            if(node) node = node->next;
          }

        }

      } /* visit nodes with UI_SIZE_PERCENT_OF_PARENT in pre order */

      UI_PROFILE(ui_prof_time_to_calc_sum_sized_boxes) {
        ui_calc_downward_dependent_sizes(ui->root, axis, ui->root->child_layout_axis);
      }

      UI_PROFILE(ui_prof_time_to_solve_size_violations)
      { /* solve size violations in a pre order traversal */

        for(UI_box *node = ui->root; node;) {

          if(axis != node->child_layout_axis && !(node->flags & (UI_BOX_FLAG_OVERFLOW_X << axis))) {
            f32 allowed_size = node->computed_size[axis];

            for(UI_box *child = node->first; child; child = child->next) {
              // TODO floating on x and y axis separately
              if(!(child->flags & (UI_BOX_FLAG_IS_FLOATING))) {
                f32 child_size = child->computed_size[axis];
                f32 violation = child_size - allowed_size;
                f32 max_fixup = child_size;
                f32 fixup = Clamp(violation, 0, max_fixup);

                if(fixup > 0) {
                  child->computed_size[axis] -= fixup;
                }
              }
            }
          }

          if(axis == node->child_layout_axis && !(node->flags &(UI_BOX_FLAG_OVERFLOW_X << axis))) {
            f32 total_allowed_size = node->computed_size[axis];
            f32 total_size = 0;
            f32 total_weighted_size = 0;

            for(UI_box *child = node->first; child; child = child->next) {
              // TODO floating on x and y axis separately
              if(!(child->flags & (UI_BOX_FLAG_IS_FLOATING))) {
                total_size += child->computed_size[axis];
                total_weighted_size += child->computed_size[axis] * (1-child->semantic_size[axis].strictness);
              }
            }

            f32 violation = total_size - total_allowed_size;
            
            Arena_scope tmp_scope;
            if(violation > 0 && total_weighted_size > 0) defer_loop(tmp_scope = scope_begin(ui->build_arena), scope_end(tmp_scope))
            {
              f32 *child_fixups = push_array(ui->build_arena, f32, node->child_count);

              {
                u64 child_index = 0;
                for(UI_box *child = node->first; child; child = child->next, child_index += 1) {
                  // TODO floating on x and y axis separately
                  if(!(child->flags & (UI_BOX_FLAG_IS_FLOATING))) {
                    f32 fixup_size_this_child = child->computed_size[axis] * (1-child->semantic_size[axis].strictness);
                    fixup_size_this_child = CLAMP_BOT(0, fixup_size_this_child);
                    child_fixups[child_index] = fixup_size_this_child;
                  }
                }
              }

              {
                u64 child_index = 0;
                for(UI_box *child = node->first; child; child = child->next, child_index += 1) {
                  // TODO floating on x and y axis separately
                  if(!(child->flags & (UI_BOX_FLAG_IS_FLOATING))) {
                    f32 fixup_percent = (violation / total_weighted_size);
                    fixup_percent = Clamp(fixup_percent, 0, 1);
                    child->computed_size[axis] -= child_fixups[child_index] * fixup_percent;
                  }
                }
              }

            }

          }

          /* fix the upward dependent sizes */
          if(node->flags & (UI_BOX_FLAG_OVERFLOW_X << axis)) {
            for(UI_box *child = node->first; child; child = child->next) {
              if(child->semantic_size[axis].kind == UI_SIZE_PERCENT_OF_PARENT) {
                child->computed_size[axis] = node->computed_size[axis] * child->semantic_size[axis].value;
              }
            }
          }

          for(UI_box *child = node->first; child; child = child->next) {
            child->computed_size[axis] = MAX(child->computed_size[axis], child->min_size[axis]);
          }

          if(node->first) {
            node = node->first;
          } else if(node->next) {
            node = node->next;
          } else {
            while(node && !(node->next)) {
              node = node->parent;
            }
            if(node) node = node->next;
          }
        }

      } /* solve size violations in a pre order traversal */
    }

  UI_PROFILE(ui_prof_time_to_calc_positions) { /* calculate the relative and then absolute positions of each box in pre order */

    for(UI_box *node = ui->root; node;) {
      u32 layout_axis = node->child_layout_axis;
      u32 axis = !layout_axis;

      f32 final_layout_axis_min = 0.0f;
      f32 final_axis_min = 0.0f;
      if(node->flags & UI_BOX_FLAG_IS_FLOATING) {
        final_layout_axis_min = (&(node->floating_position.x))[layout_axis];
        final_axis_min = (&(node->floating_position.x))[axis];
      } else {

        f32 parent_layout_axis_pos = 0.0f;
        f32 parent_axis_pos = 0.0f;
        if(node->parent) {
          parent_layout_axis_pos = node->parent->final_rect.min[layout_axis];
          parent_axis_pos = node->parent->final_rect.min[axis];
        }

        f32 rel_layout_axis_pos = 0.0f;
        if(node->prev) {
          rel_layout_axis_pos = node->prev->computed_rel_pos[layout_axis] + node->prev->computed_size[layout_axis];
        }

        node->computed_rel_pos[layout_axis] = rel_layout_axis_pos;

        final_layout_axis_min = parent_layout_axis_pos + rel_layout_axis_pos;
        final_axis_min = parent_axis_pos;
      }

      node->final_rect.min[layout_axis] = final_layout_axis_min;
      node->final_rect.min[axis] = final_axis_min;
      node->final_rect.max[layout_axis] = final_layout_axis_min + node->computed_size[layout_axis];
      node->final_rect.max[axis] = final_axis_min + node->computed_size[axis];

      if(node->first) {
        node = node->first;
      } else if(node->next) {
        node = node->next;
      } else {
        while(node && !(node->next)) {
          node = node->parent;
        }
        if(node) node = node->next;
      }

    }

  } /* calculate the relative and then absolute positions of each box in pre order */



}

UI_box_node* ui_push_box_node(UI_context *ui) {
  UI_box_node *node = 0;

  node = push_struct(ui->draw_arena, UI_box_node);

  // I don't think this is necessary
  //
  //if(ui->box_node_free_list) {
  //  node = sll_stack_pop(ui->box_node_free_list);
  //} else {
  //}

  return node;
}

void dump_UI_box(const UI_box *box) {
    printf("UI_box {\n");

    printf("  flags = 0x%lX\n", box->flags);
    printf("  exclude_flags = 0x%lX\n", box->exclude_flags);
    printf("  key = %.*s, %lu\n", (int)box->key.src_str.len, box->key.src_str.s, box->key.hash);

    printf("  hash_next = %p\n", (void*)box->hash_next);
    printf("  hash_prev = %p\n", (void*)box->hash_prev);
    printf("  parent = %p\n", (void*)box->parent);
    printf("  next = %p\n", (void*)box->next);
    printf("  prev = %p\n", (void*)box->prev);
    printf("  first = %p\n", (void*)box->first);
    printf("  last = %p\n", (void*)box->last);
    printf("  child_count = %llu\n", (unsigned long long)box->child_count);

    printf("  background_color = rgba(%u, %u, %u, %u)\n",
           box->background_color.r, box->background_color.g,
           box->background_color.b, box->background_color.a);
    printf("  text_color = rgba(%u, %u, %u, %u)\n",
           box->text_color.r, box->text_color.g,
           box->text_color.b, box->text_color.a);
    printf("  border_color = rgba(%u, %u, %u, %u)\n",
           box->border_color.r, box->border_color.g,
           box->border_color.b, box->border_color.a);

    printf("  border_size = %.2f\n", box->border_size);
    printf("  corner_radius = [%.2f, %.2f, %.2f, %.2f]\n",
           box->corner_radius[0], box->corner_radius[1],
           box->corner_radius[2], box->corner_radius[3]);

    printf("  font_id = %u\n", box->font_id);
    printf("  font_size = %.2f\n", box->font_size);
    printf("  font_spacing = %.2f\n", box->font_spacing);
    printf("  text_align = %d\n", box->text_align);  // You can print a name if you have an enum->string

    printf("  semantic_size = [%.2f, %.2f]\n",
           box->semantic_size[0].value, box->semantic_size[1].value);

    printf("  padding = %.2f\n", box->padding);
    printf("  floating_position = {%.2f, %.2f}\n",
           box->floating_position.x, box->floating_position.y);
    printf("  child_layout_axis = %d\n", box->child_layout_axis);

    printf("  text = \"%.*s\"\n", (int)box->text.len, box->text.s);

    printf("  min_size = [%.2f, %.2f]\n",
           box->min_size[0], box->min_size[1]);
    printf("  fixed_size = [%.2f, %.2f]\n",
           box->fixed_size[0], box->fixed_size[1]);

    printf("  computed_size = [%.2f, %.2f]\n",
           box->computed_size[0], box->computed_size[1]);
    printf("  computed_rel_pos = [%.2f, %.2f]\n",
           box->computed_rel_pos[0], box->computed_rel_pos[1]);

    printf("  final_rect.min = [%.2f, %.2f]\n",
           box->final_rect.min[0], box->final_rect.min[1]);
    printf("  final_rect.max = [%.2f, %.2f]\n",
           box->final_rect.max[0], box->final_rect.max[1]);

    printf("  first_visited_build_index = %llu\n",
           (unsigned long long)box->first_visited_build_index);
    printf("  last_visited_build_index = %llu\n",
           (unsigned long long)box->last_visited_build_index);

    printf("  view_offset = [%.2f, %.2f]\n",
           box->view_offset[0], box->view_offset[1]);
    printf("  view_offset_target = [%.2f, %.2f]\n",
           box->view_offset_target[0], box->view_offset_target[1]);
    printf("  view_bounds = [%.2f, %.2f]\n",
           box->view_bounds[0], box->view_bounds[1]);

    printf("  hot_time = %.2f\n", box->hot_time);
    printf("  active_time = %.2f\n", box->active_time);

    printf("}\n");
}

void ui_draw(UI_context *ui) {

  for(UI_box *box = ui->root; box;) {

    Rectangle rec =
    {
      .x = box->final_rect.min[0],
      .y = box->final_rect.min[1],
      .width =  box->final_rect.max[0] - box->final_rect.min[0],
      .height = box->final_rect.max[1] - box->final_rect.min[1],
    };

    if(box->flags & UI_BOX_FLAG_DRAW_BACKGROUND) {
      DrawRectangleRoundedPro(rec,
          box->corner_radius_0,
          box->corner_radius_1,
          box->corner_radius_2,
          box->corner_radius_3,
          8,
          box->background_color);
    }

    if(box->flags & UI_BOX_FLAG_DRAW_BORDER) {
      DrawRectangleRoundedLinesPro(rec,
          box->corner_radius_0,
          box->corner_radius_1,
          box->corner_radius_2,
          box->corner_radius_3,
          8,
          box->border_size,
          box->border_color);
    }

    if(box->flags & UI_BOX_FLAG_DRAW_TEXT) {
      Arena_scope scope = scope_begin(ui->draw_arena);

      char *text_cstr = push_cstr_copy_str8(ui->draw_arena, box->text);

      f32 box_mid_x = (box->final_rect.max[0] + box->final_rect.min[0]) * 0.5f;
      f32 box_mid_y = (box->final_rect.max[1] + box->final_rect.min[1]) * 0.5f;
      Vector2 text_size =
        MeasureTextEx(
            ui->fonts[box->font_id],
            text_cstr,
            box->font_size,
            box->font_spacing);

      Vector2 text_pos =
      {
        .y = box_mid_y - text_size.y*0.5f,
      };

      switch(box->text_align) {
        case UI_TEXT_ALIGN_CENTER:
          {
            text_pos.x = box_mid_x - text_size.x*0.5;
          } break;
        case UI_TEXT_ALIGN_LEFT:
          {
            text_pos.x = box->final_rect.min[0] + box->padding;
          } break;
        case UI_TEXT_ALIGN_RIGHT:
          {
            text_pos.x = box->final_rect.max[0] - text_size.x - box->padding;
          } break;
      }

      DrawTextEx(
          ui->fonts[box->font_id],
          text_cstr,
          text_pos,
          box->font_size,
          box->font_spacing,
          box->text_color);

      scope_end(scope);

    }

    if(box->flags & UI_BOX_FLAG_CLIP) {
      BeginScissorMode((int)rec.x, (int)rec.y, (int)rec.width, (int)rec.height);
    }

    if(box->first) {
      box = box->first;
    } else if(box->next) {
      box = box->next;
    } else {
      while(box && !(box->next)) {
        box = box->parent;

        if(box && box->flags & UI_BOX_FLAG_CLIP) {
          EndScissorMode();
        }
      }

      if(box) {
        box = box->next;
      }
    }

  } /* for(UI_box_node *node = ui->root; node;) */

  arena_clear(ui->draw_arena);

}

