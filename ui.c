
#define MAX_TABLE_SLOTS 64
#define UI_ARR_PREFIX UI_Arr_


/*
 * tables
 */

#define UI_CONTEXT_FLAGS \

#define UI_MOUSE_BUTTONS \
  X(LEFT) \
  X(RIGHT) \
  X(MIDDLE) \

#define UI_MOUSE_BUTTON_ACTIONS \
  X(LEFT_MOUSE_PRESS) \
  X(RIGHT_MOUSE_PRESS) \
  X(MIDDLE_MOUSE_PRESS) \
  X(LEFT_MOUSE_RELEASE) \
  X(RIGHT_MOUSE_RELEASE) \
  X(MIDDLE_MOUSE_RELEASE) \
  X(LEFT_MOUSE_CLICK) \
  X(RIGHT_MOUSE_CLICK) \
  X(MIDDLE_MOUSE_CLICK) \

#define UI_INPUT_FLAGS \
  X(MOUSE_PRESS) \
  X(MOUSE_RELEASE) \
  X(MOUSE_CLICK) \
  X(MOUSE_SCROLL) \
  X(KEYBOARD_PRESSED) \

#define UI_PERMISSION_FLAGS \
  X(CLICKS_LEFT) \
  X(CLICKS_RIGHT) \
  X(CLICKS_MIDDLE) \
  X(SCROLL) \
  X(KEYBOARD) \

#define UI_PERMISSION_FLAG_CLICKS (UI_PERMISSION_FLAG_CLICKS_LEFT | UI_PERMISSION_FLAG_CLICKS_RIGHT | UI_PERMISSION_FLAG_CLICKS_MIDDLE)
#define UI_PERMISSION_FLAG_ALL (UI_PERMISSION_FLAG_CLICKS | UI_PERMISSION_FLAG_SCROLL | UI_PERMISSION_FLAG_KEYBOARD)

#define UI_SIGNAL_FLAGS \
  UI_MOUSE_BUTTON_ACTIONS \
  \
  X(LEFT_MOUSE_DRAG) \
  X(RIGHT_MOUSE_DRAG) \
  X(MIDDLE_MOUSE_DRAG) \
  \
  X(SCROLL) \
  \
  X(MOUSE_HOVERING) \
  X(MOUSE_OVER) \

#define UI_MODIFIER_KEYS \
  X(CONTROL) \
  X(SHIFT) \
  X(SUPER) \

#define UI_BOX_FLAGS \
  X(DRAW_TEXT) \
  X(DRAW_BACKGROUND) \
  X(DRAW_BORDER) \
  X(CLIP) \
  X(FIXED_WIDTH) \
  X(FIXED_HEIGHT) \
  X(FLOATING_X) \
  X(FLOATING_Y) \
  X(OVERFLOW_X) \
  X(OVERFLOW_Y) \
  X(MOUSE_CLICKABLE) \
  X(KEYBOARD_CLICKABLE) \
  X(DROP_SITE) \
  X(SCROLL) \
  X(INVERT_SCROLL) \
  X(VIEW_SCROLL_X) \
  X(VIEW_SCROLL_Y) \
  X(CLAMP_VIEW_X) \
  X(CLAMP_VIEW_Y) \
  X(SKIP_VIEW_OFFSET_X) \
  X(SKIP_VIEW_OFFSET_Y) \

#define UI_BOX_FLAG_VIEW_SCROLL (UI_BOX_FLAG_VIEW_SCROLL_X | UI_BOX_FLAG_VIEW_SCROLL_Y)
#define UI_BOX_FLAG_CLAMP_VIEW (UI_BOX_FLAG_CLAMP_VIEW_X | UI_BOX_FLAG_CLAMP_VIEW_Y)
#define UI_BOX_FLAG_SKIP_VIEW_OFFSET (UI_BOX_FLAG_SKIP_VIEW_OFFSET_X | UI_BOX_FLAG_SKIP_VIEW_OFFSET_Y)
#define UI_BOX_FLAG_FLOATING (UI_BOX_FLAG_FLOATING_X | UI_BOX_FLAG_FLOATING_Y)
#define UI_BOX_FLAG_OVERFLOW (UI_BOX_FLAG_OVERFLOW_X | UI_BOX_FLAG_OVERFLOW_Y)
#define UI_BOX_FLAG_FIXED_SIZE (UI_BOX_FLAG_FIXED_WIDTH | UI_BOX_FLAG_FIXED_HEIGHT)


#define UI_STYLE_PROPERTIES \
  /* lower               lower_alt             type              init                                               stack_size */ \
  X( background_color,   background_color,     Color,            ((Color){ 110, 110, 110, 255 }),                   8           ) \
  X( text_color,         text_color,           Color,            ((Color){ 248, 248, 248, 255 }),                   8           ) \
  X( border_color,       border_color,         Color,            ((Color){ 180, 180, 180, 255 }),                   8           ) \
  X( border_size,        border_size,          f32,              ((f32)0.0f),                                       8           ) \
  X( corner_radius_0,    corner_radius[0],     f32,              ((f32)0.0f),                                       8           ) \
  X( corner_radius_1,    corner_radius[1],     f32,              ((f32)0.0f),                                       8           ) \
  X( corner_radius_2,    corner_radius[2],     f32,              ((f32)0.0f),                                       8           ) \
  X( corner_radius_3,    corner_radius[3],     f32,              ((f32)0.0f),                                       8           ) \
  X( font_id,            font_id,              u32,              ((u32)0),                                          4           ) \
  X( font_size,          font_size,            f32,              ((f32)10.0f),                                      8           ) \
  X( font_spacing,       font_spacing,         f32,              ((f32)1.0f),                                       4           ) \
  X( text_align,         text_align,           UI_text_align,    UI_TEXT_ALIGN_LEFT,                                16          ) \
  X( semantic_width,     semantic_size[0],     UI_size,          ((UI_size){ .kind = UI_SIZE_CHILDREN_SUM }),       16          ) \
  X( semantic_height,    semantic_size[1],     UI_size,          ((UI_size){ .kind = UI_SIZE_CHILDREN_SUM }),       16          ) \
  X( padding,            padding,              f32,              ((f32)2.0f),                                       8           ) \
  X( child_layout_axis,  child_layout_axis,    UI_axis,          ((UI_axis)0),                                      64          ) \

#define UI_SIZE_PROPERTIES \
  /* lower           lower_alt         type              init                                stack_size */ \
  X( min_width,      min_size[0],      f32,              ((f32)0),                           4           ) \
  X( min_height,     min_size[1],      f32,              ((f32)0),                           4           ) \
  X( fixed_width,    fixed_size[0],    f32,              ((f32)0),                           4           ) \
  X( fixed_height,   fixed_size[1],    f32,              ((f32)0),                           4           ) \

#define UI_OTHER_PROPERTIES \
  /* lower               lower_alt                  type              init                       stack_size */ \
  X( parent,             parent,                    UI_box_ptr,       0,                         16          ) \
  X( flags,              flags,                     UI_box_flags,     (UI_BOX_FLAG_OVERFLOW),    16          ) \
  X( fixed_position,     fixed_position,            Vector2,          ((Vector2){0}),            4           ) \
  X( exclude_flags,      exclude_flags,             UI_box_flags,     0,                         16          ) \

#define UI_NON_BOX_PROPERTIES \
  /* lower               lower_alt          type                   init        stack_size */ \
  X( permission_flags,   permission_flags,  UI_permission_flags,   0,          4           ) \

#define UI_PROPERTIES \
  UI_STYLE_PROPERTIES \
  UI_SIZE_PROPERTIES \
  UI_OTHER_PROPERTIES \

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

#define ui_fixed_width(value) ui_prop(fixed_width, (value))
#define ui_fixed_height(value) ui_prop(fixed_height, (value))
#define ui_fixed_size(value) ui_fixed_width((value)) ui_fixed_height((value))

#define ui_padding(value) ui_prop(padding, (value))

#define ui_fixed_position(value) ui_prop(fixed_position, (value))

#define ui_child_layout_axis(value) ui_prop(child_layout_axis, (value))

#define ui_parent(value) ui_prop(parent, (value))

#define ui_flags(value) ui_prop(flags, (value))
#define ui_exclude_flags(value) ui_prop(exclude_flags, (value))

#define ui_permission_flags(value) ui_prop(permission_flags, (value))
#define ui_permission_flags_top() ui_prop_top(permission_flags)

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
typedef struct UI_event UI_event;
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
  UI_MOD_INVALID = -1,
#define X(mod) UI_MOD_##mod,
  UI_MODIFIER_KEYS
#undef X
    UI_MOD_COUNT,
} UI_modifier_keys;

typedef u32 UI_modifier_keys_mask;
#define X(mod) const UI_modifier_keys_mask UI_MOD_MASK_##mod = (1u<<UI_MOD_##mod);
UI_MODIFIER_KEYS
#undef X

typedef enum UI_mouse_button {
  UI_MOUSE_BUTTON_INVALID = -1,
#define X(b) UI_MOUSE_BUTTON_##b,
  UI_MOUSE_BUTTONS
#undef X
    UI_MOUSE_BUTTON_COUNT,
} UI_mouse_button;

typedef u32 UI_mouse_button_mask;
#define X(b) const UI_mouse_button_mask UI_MOUSE_BUTTON_MASK_##b = (1u<<UI_MOUSE_BUTTON_##b);
UI_MOUSE_BUTTONS
#undef X

typedef enum UI_box_flags_index {
  UI_BOX_FLAG_INDEX_NONE = -1,
#define X(f) UI_BOX_FLAG_INDEX_##f,
  UI_BOX_FLAGS
#undef X
    UI_BOX_FLAG_INDEX_MAX,
} UI_box_flags_index;

typedef u64 UI_box_flags;
STATIC_ASSERT(UI_BOX_FLAG_INDEX_MAX < 64, UI_BOX_FLAG_INDEX_MAX__is_less_than_64);

#define X(f) const UI_box_flags UI_BOX_FLAG_##f = (1ull<<UI_BOX_FLAG_INDEX_##f);
UI_BOX_FLAGS
#undef X

typedef enum UI_context_flags_index {
  UI_CONTEXT_FLAG_INDEX_NONE = -1,
#define X(f) UI_CONTEXT_FLAG_INDEX_##f,
  UI_CONTEXT_FLAGS
#undef X
    UI_CONTEXT_FLAG_INDEX_MAX,
} UI_context_flags_index;

typedef u64 UI_context_flags;
STATIC_ASSERT(UI_CONTEXT_FLAG_INDEX_MAX < 64, UI_CONTEXT_FLAG_INDEX_MAX__is_less_than_64);

#define X(f) const UI_context_flags UI_CONTEXT_FLAG_##f = (1ull<<UI_CONTEXT_FLAG_INDEX_##f);
UI_CONTEXT_FLAGS
#undef X

typedef enum UI_signal_flags_index {
  UI_SIGNAL_FLAG_INDEX_NONE = -1,
#define X(f) UI_SIGNAL_FLAG_INDEX_##f,
  UI_SIGNAL_FLAGS
#undef X
    UI_SIGNAL_FLAG_INDEX_MAX,
} UI_signal_flags_index;

typedef u64 UI_signal_flags;
STATIC_ASSERT(UI_SIGNAL_FLAG_INDEX_MAX < 64, UI_SIGNAL_FLAG_INDEX_MAX__is_less_than_64);

#define X(f) const UI_signal_flags UI_SIGNAL_FLAG_##f = (1ull<<UI_SIGNAL_FLAG_INDEX_##f);
UI_SIGNAL_FLAGS
#undef X

typedef enum UI_input_flags_index {
  UI_INPUT_FLAG_INDEX_NONE = -1,
#define X(f) UI_INPUT_FLAG_INDEX_##f,
  UI_INPUT_FLAGS
#undef X
    UI_INPUT_FLAG_INDEX_MAX,
} UI_input_flags_index;

typedef u32 UI_input_flags;
STATIC_ASSERT(UI_INPUT_FLAG_INDEX_MAX < (sizeof(UI_input_flags)<<3), UI_INPUT_FLAG_INDEX_MAX__is_within_size);

#define X(f) const UI_input_flags UI_INPUT_FLAG_##f = (1u<<UI_INPUT_FLAG_INDEX_##f);
UI_INPUT_FLAGS
#undef X

typedef enum UI_permission_flags_index {
  UI_PERMISSION_FLAG_INDEX_NONE = -1,
#define X(f) UI_PERMISSION_FLAG_INDEX_##f,
  UI_PERMISSION_FLAGS
#undef X
    UI_PERMISSION_FLAG_INDEX_MAX,
} UI_permission_flags_index;

typedef u32 UI_permission_flags;
STATIC_ASSERT(UI_PERMISSION_FLAG_INDEX_MAX < (sizeof(UI_permission_flags)<<3), UI_PERMISSION_FLAG_INDEX_MAX__is_within_size);

#define X(f) const UI_permission_flags UI_PERMISSION_FLAG_##f = (1u<<UI_PERMISSION_FLAG_INDEX_##f);
UI_PERMISSION_FLAGS
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
DECL_UI_ARR(UI_box_flags);
DECL_UI_ARR(UI_permission_flags);
DECL_UI_ARR(UI_axis);
DECL_UI_ARR(UI_size);
DECL_UI_ARR(UI_text_align);


/*
 * struct bodies
 */

struct UI_event {
  UI_input_flags input_flags;
  UI_mouse_button_mask mouse_buttons_active;
  Vector2 mouse_pos;
  Vector2 scroll_delta;
  UI_modifier_keys_mask modifier_keys;
  u32 keyboard_key_pressed;
};

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
  UI_box_flags flags;
  UI_box_flags exclude_flags;

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
  f32 corner_radius[4];
  u32 font_id;
  f32 font_size;
  f32 font_spacing;
  UI_text_align text_align;
  UI_size semantic_size[UI_AXIS_COUNT];
  f32 padding;
  Vector2 fixed_position;
  UI_axis child_layout_axis;

  Str8 text;

  f32 min_size[2];
  f32 max_size[2];
  f32 fixed_size[2];
  f32 computed_rel_pos[2];
  f32 final_rect_min[2];
  f32 final_rect_max[2];

  u64 first_visited_build_index;
  u64 last_visited_build_index;
  f32 view_offset[2];
  f32 view_offset_target[2];
  f32 view_bounds[2];

  f32 hot_time;
  f32 active_time;
};

struct UI_signal {
  UI_signal_flags flags;
  UI_box *box;
  s16 scroll[2];
  UI_modifier_keys_mask modifier_keys;
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

  UI_context_flags flags;

  UI_event input_event;

  Vector2 mouse_pos;
  Vector2 drag_start_pos;

  b32 took_input_event;
  f32 scroll_rate;

  UI_key active_box_key[UI_MOUSE_BUTTON_COUNT];
  UI_key hot_box_key;
  UI_key drop_hot_box_key;

  UI_box *box_free_list;

#define X(lower, lower_alt, type, init, stack_size) UI_ARR_PREFIX##type lower##_stack;
  UI_PROPERTIES
  UI_NON_BOX_PROPERTIES;
#undef X

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
UI_box* ui_get_box_from_str(UI_context *ui, Str8 s);

UI_signal ui_signal_from_box(UI_context *ui, UI_box *box);

Str8 ui_strip_id_from_text(Str8 text);

Vector2 ui_drag_delta(UI_context *ui);

UI_box* ui_make_box_from_key(UI_context *ui, UI_box_flags flags, UI_key key);
UI_box* ui_make_box_from_str(UI_context *ui, UI_box_flags flags, Str8 str);
UI_box* ui_make_box_from_strf(UI_context *ui, UI_box_flags flags, char *fmt, ...);
UI_box* ui_make_transient_box(UI_context *ui);

UI_box_node* ui_push_box_node(UI_context *ui);

f32 ui_calc_downward_dependent_sizes(UI_box *box, int axis, int layout_axis);

void ui_begin_build(UI_context *ui);
void ui_end_build(UI_context *ui);
void ui_draw(UI_context *ui);
UI_event ui_get_event(UI_context *ui);
void ui_get_frame_input(UI_context *ui);

UI_key ui_hot_box_key(UI_context *ui);
UI_key ui_drop_hot_box_key(UI_context *ui);
UI_key ui_active_box_key(UI_context *ui, UI_mouse_button btn);

/*
 * macros
 */

#define ui_build(ui) defer_loop(ui_begin_build((ui)), ui_end_build((ui)))
#define ui_push_prop(name, value) (arr_push(ui->name##_stack, (value)))
#define ui_pop_prop(name) (arr_pop(ui->name##_stack))
#define ui_clear_prop(name) (arr_clear(ui->name##_stack))
#define ui_prop(name, value) defer_loop(ui_push_prop(name, (value)), ui_pop_prop(name))
#define ui_prop_top(name) (arr_last(ui->name##_stack))

#define ui_mouse_button_mask(button) ((UI_mouse_button_mask)(1<<(button)))

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

func UI_context* ui_init(void) {
  
  UI_context *ui = os_alloc(sizeof(UI_context));

  memory_zero(ui, sizeof(UI_context));

  ui->arena       = arena_alloc(MB(1));
  ui->temp        = arena_alloc(KB(8));
  ui->build_arena = arena_alloc(KB(16));
  ui->draw_arena  = arena_alloc(KB(16));

#define X(lower, lower_alt, type, init, stack_size) \
    arr_init_ex(ui->lower##_stack, ui->arena, stack_size);
  UI_PROPERTIES;
  UI_NON_BOX_PROPERTIES;
#undef X

//#define X(lower, lower_alt, type, init, stack_size) \
//  type _tmp_init_##lower = init; \
//  ui_push_prop(lower, _tmp_init_##lower);
//  UI_STYLE_PROPERTIES;
//  UI_OTHER_PROPERTIES;
//#undef X

  ui->fonts[0] = GetFontDefault();

  ui->dummy = &ui->__dummy;
  ui->scroll_rate = 1.0f;

  return ui;
}

func void ui_close(UI_context *ui) {
  arena_free(ui->arena);
  arena_free(ui->build_arena);
  arena_free(ui->temp);
  arena_free(ui->draw_arena);
  os_free(ui);
}

func UI_key ui_key_nil(void) {
  return (UI_key){0};
}

func UI_key ui_key_from_str(Str8 str) {

  UI_key key = { .hash = 0, .src_str = str };

  { /* hash */
    for(int i = 0; i < str.len; i++) {
      key.hash += str.s[i] * 37;
    }
  } /* hash */

  return key;
}

func b32 ui_key_match(UI_key a, UI_key b) {
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

func UI_box* ui_get_box_from_key(UI_context *ui, UI_key key) {
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

func UI_box* ui_get_box_from_str(UI_context *ui, Str8 s) {
  return ui_get_box_from_key(ui, ui_key_from_str(s));
}

func UI_key ui_hot_box_key(UI_context *ui) {
  return ui->hot_box_key;
}

func UI_key ui_drop_hot_box_key(UI_context *ui) {
  return ui->drop_hot_box_key;
}

func UI_key ui_active_box_key(UI_context *ui, UI_mouse_button btn) {
  return ui->active_box_key[btn];
}

func UI_box* ui_make_transient_box(UI_context *ui) {
  return ui_make_box_from_key(ui, 0, ((UI_key){0}));
}

func UI_box* ui_make_box_from_key(UI_context *ui, UI_box_flags flags, UI_key key) {

#define X(lower, lower_alt, type, init, stack_size) type cur_##lower = ui_prop_top(lower);
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
    u64 slot = key.hash % MAX_TABLE_SLOTS;
    dll_push_back_np(ui->box_table[slot].first, ui->box_table[slot].last, box, hash_next, hash_prev);
  }

  if(cur_parent) {
    dll_push_back(cur_parent->first, cur_parent->last, box);
    cur_parent->child_count += 1;
    box->parent = cur_parent;
  }

  box->key = key;
  box->last_visited_build_index = ui->build_index;

#define X(lower, lower_alt, type, init, stack_size) memory_copy(&box->lower_alt, &cur_##lower, sizeof(cur_##lower));
  UI_STYLE_PROPERTIES;
  UI_OTHER_PROPERTIES;
#undef X

  if(ui->fixed_width_stack.count > 0) {
    box->flags |= UI_BOX_FLAG_FIXED_WIDTH;
    box->fixed_size[0] = cur_fixed_width;
  }

  if(ui->fixed_height_stack.count > 0) {
    box->flags |= UI_BOX_FLAG_FIXED_HEIGHT;
    box->fixed_size[1] = cur_fixed_height;
  }

  box->min_size[0] = cur_min_width;
  box->min_size[1] = cur_min_height;

  box->flags |= flags;
  box->flags &= ~cur_exclude_flags;

  return box;
}

func Str8 ui_strip_id_from_text(Str8 text) {
  s64 index = str8_find(text, str8_lit("##"));
  if(index >= 0) {
    text.len = index;
  }
  return text;
}

func UI_box* ui_make_box_from_str(UI_context *ui, UI_box_flags flags, Str8 str) {
  UI_key key = ui_key_from_str(str);

  UI_box *box = ui_make_box_from_key(ui, flags, key);

  if(box->flags & UI_BOX_FLAG_DRAW_TEXT) {
    box->text = ui_strip_id_from_text(str);
  }

  return box;
}

func UI_box* ui_make_box_from_strf(UI_context *ui, UI_box_flags flags, char *fmt, ...) {
  UI_box *box = NULL;

  arena_scope(ui->temp) {
    va_list args;
    va_start(args, fmt);
    Str8 str = str8fv(ui->temp, fmt, args);
    va_end(args);

    box = ui_make_box_from_str(ui, flags, str);
  }

  return box;
}

func Vector2 ui_drag_delta(UI_context *ui) {
  return Vector2Subtract(ui->mouse_pos, ui->drag_start_pos);
}

func UI_signal ui_signal_from_box(UI_context *ui, UI_box *box) {
  b32 taken = 0;

  UI_signal sig = { .box = box };

  UI_event _event = ui_get_event(ui);
  UI_event *event = &_event;

  Rectangle box_rec =
  {
    .x = box->final_rect_min[0],
    .y = box->final_rect_min[1],
    .width =  box->final_rect_max[0] - box->final_rect_min[0],
    .height = box->final_rect_max[1] - box->final_rect_min[1],
  };

  for(UI_box *parent = box->parent; parent; parent = parent->parent) {
    if(parent->flags & UI_BOX_FLAG_CLIP) {
      Rectangle clip_rec =
      {
        .x = parent->final_rect_min[0], .y = parent->final_rect_min[1],
        .width = parent->final_rect_max[0] - parent->final_rect_min[0],
        .height = parent->final_rect_max[1] - parent->final_rect_min[1],
      };

      box_rec = GetCollisionRec(clip_rec, box_rec);
    }
  }

  b8 mouse_in_bounds = CheckCollisionPointRec(ui->mouse_pos, box_rec);

#if 0
  if((box->flags & UI_BOX_FLAG_MOUSE_CLICKABLE) &&
      mouse_in_bounds)
  {
    sig.flags |= UI_SIGNAL_FLAG_MOUSE_HOVERING | UI_SIGNAL_FLAG_MOUSE_OVER;
    ui->hot_box_key = box->key;
  }
#endif

  for(UI_mouse_button btn = UI_MOUSE_BUTTON_LEFT; btn < UI_MOUSE_BUTTON_COUNT; btn++) {
    b8 btn_on = event->mouse_buttons_active & ui_mouse_button_mask(btn);

    if((box->flags & UI_BOX_FLAG_MOUSE_CLICKABLE) &&
        (event->input_flags & UI_INPUT_FLAG_MOUSE_PRESS) &&
        btn_on &&
        mouse_in_bounds
      )
    {
      taken = 1;
      sig.flags |= (UI_SIGNAL_FLAG_LEFT_MOUSE_PRESS << btn);
      ui->active_box_key[btn] = box->key;
      ui->drag_start_pos = ui->mouse_pos;
    }

    if((box->flags & UI_BOX_FLAG_MOUSE_CLICKABLE) &&
        (event->input_flags & UI_INPUT_FLAG_MOUSE_RELEASE) &&
        btn_on &&
        ui_key_match(ui_active_box_key(ui, btn), box->key) &&
        ui_key_match(ui_hot_box_key(ui), box->key) &&
        mouse_in_bounds
      )
    {
      taken = 1;
      sig.flags |= (UI_SIGNAL_FLAG_LEFT_MOUSE_RELEASE << btn);
      sig.flags |= (UI_SIGNAL_FLAG_LEFT_MOUSE_CLICK << btn);
      ui->active_box_key[btn] = ui_key_nil();
    }

    if((box->flags & UI_BOX_FLAG_MOUSE_CLICKABLE) &&
        (event->input_flags & UI_INPUT_FLAG_MOUSE_RELEASE) &&
        btn_on &&
        ui_key_match(ui_active_box_key(ui, btn), box->key) &&
        !mouse_in_bounds
      )
    {
      taken = 1;
      sig.flags |= (UI_SIGNAL_FLAG_LEFT_MOUSE_RELEASE << btn);
      ui->hot_box_key = ui_key_nil();
      ui->active_box_key[btn] = ui_key_nil();
    }

#if !1
    /* dragging */
    if((box->flags & UI_BOX_FLAG_MOUSE_CLICKABLE) &&
        ((sig.flags & (UI_SIGNAL_FLAG_LEFT_MOUSE_PRESS << btn)) ||
        ui_key_match(ui_active_box_key(ui, btn), box->key))
      )
    {
      taken = 1;
      sig.flags |= (UI_SIGNAL_FLAG_LEFT_MOUSE_DRAG << btn);
    }
#endif
    if((box->flags & UI_BOX_FLAG_MOUSE_CLICKABLE) &&
      ui_key_match(ui->active_box_key[btn], box->key) &&
      !ui_key_match(ui->active_box_key[btn], ui_key_nil()))
    {
      ui->hot_box_key = box->key;
      ui->active_box_key[btn] = box->key;
      sig.flags |= UI_SIGNAL_FLAG_MOUSE_HOVERING | (UI_SIGNAL_FLAG_LEFT_MOUSE_DRAG << btn);
    }

  }

  {
    if(box->flags & UI_BOX_FLAG_MOUSE_CLICKABLE &&
        mouse_in_bounds &&
        (ui_key_match(ui->hot_box_key, ui_key_nil()) || ui_key_match(ui->hot_box_key, box->key)) &&
        (ui_key_match(ui->active_box_key[UI_MOUSE_BUTTON_LEFT], ui_key_nil()) || ui_key_match(ui->active_box_key[UI_MOUSE_BUTTON_LEFT], box->key)) &&
        (ui_key_match(ui->active_box_key[UI_MOUSE_BUTTON_MIDDLE], ui_key_nil()) || ui_key_match(ui->active_box_key[UI_MOUSE_BUTTON_MIDDLE], box->key)) &&
        (ui_key_match(ui->active_box_key[UI_MOUSE_BUTTON_RIGHT], ui_key_nil()) || ui_key_match(ui->active_box_key[UI_MOUSE_BUTTON_RIGHT], box->key)))
    {
      ui->hot_box_key = box->key;
      sig.flags |= UI_SIGNAL_FLAG_MOUSE_HOVERING | UI_SIGNAL_FLAG_MOUSE_OVER;
    }
  }
  
  if(box->flags & UI_BOX_FLAG_DROP_SITE &&
      mouse_in_bounds &&
      (ui_key_match(ui->drop_hot_box_key, ui_key_nil()) || ui_key_match(ui->drop_hot_box_key, box->key)))
  {
    ui->drop_hot_box_key = box->key;
  }


  if(box->flags & UI_BOX_FLAG_DROP_SITE &&
      !mouse_in_bounds &&
      ui_key_match(ui->drop_hot_box_key, box->key))
  {
    ui->drop_hot_box_key = ui_key_nil();
  }

  { /* scrolling */
    s16 delta[2] = {
      (event->scroll_delta.x > 0) ? 1 : ((event->scroll_delta.x < 0) ? -1 : 0), 
      (event->scroll_delta.y > 0) ? 1 : ((event->scroll_delta.y < 0) ? -1 : 0), 
    };

    if(event->modifier_keys & UI_MOD_MASK_SHIFT) {
      SWAP(delta[0], delta[1], s16);
    }

    delta[0] *= ui->scroll_rate;
    delta[1] *= ui->scroll_rate;

    if(box->flags & UI_BOX_FLAG_INVERT_SCROLL) {
      delta[0] *= -1.0f;
      delta[1] *= -1.0f;
    }

    if((box->flags & UI_BOX_FLAG_SCROLL) &&
        (event->input_flags & UI_INPUT_FLAG_MOUSE_SCROLL) &&
        (event->modifier_keys == 0 || event->modifier_keys & UI_MOD_MASK_SHIFT) &&
        mouse_in_bounds)
    {
      taken = 1;

      sig.scroll[0] = delta[0];
      sig.scroll[1] = delta[1];

      sig.flags |= UI_SIGNAL_FLAG_SCROLL;
    }

    b32 view_scrolled = 0;

    if((box->flags & UI_BOX_FLAG_VIEW_SCROLL) &&
        box->first_visited_build_index != box->last_visited_build_index &&
        (event->input_flags & UI_INPUT_FLAG_MOUSE_SCROLL) &&
        (event->modifier_keys == 0 || event->modifier_keys & UI_MOD_MASK_SHIFT) &&
        mouse_in_bounds)
    {
      taken = 1;
      view_scrolled = 1;

      if(!(box->flags & UI_BOX_FLAG_VIEW_SCROLL_X)) {
        delta[0] = 0;
      }

      if(!(box->flags & UI_BOX_FLAG_VIEW_SCROLL_Y)) {
        delta[1] = 0;
      }

      box->view_offset_target[0] += (f32)delta[0];
      box->view_offset_target[1] += (f32)delta[1];

      sig.flags |= UI_SIGNAL_FLAG_SCROLL;
    }

    if(view_scrolled && box->flags & UI_BOX_FLAG_CLAMP_VIEW) {
      f32 max_view_offset_target[2] = {
        MAX(0, box->view_bounds[0] - box->fixed_size[0]),
        MAX(0, box->view_bounds[1] - box->fixed_size[1]),
      };

      if(box->flags & UI_BOX_FLAG_CLAMP_VIEW_X) {
        box->view_offset_target[0] = Clamp(box->view_offset_target[0], 0, max_view_offset_target[0]);
      }

      if(box->flags & UI_BOX_FLAG_CLAMP_VIEW_Y) {
        box->view_offset_target[1] = Clamp(box->view_offset_target[1], 0, max_view_offset_target[1]);
      }

    }

    // TODO animate view scrolling
    box->view_offset[0] = box->view_offset_target[0];
    box->view_offset[1] = box->view_offset_target[1];


  } /* scrolling */

  if(taken) {
    ui->took_input_event = 1;
  }

  return sig;
}

func UI_event ui_get_event(UI_context *ui) {
  UI_permission_flags perms = ui_permission_flags_top();

  UI_event event = ui->input_event;

  UI_permission_flags mouse_button_perms =
    (perms & (UI_PERMISSION_FLAG_CLICKS_LEFT | UI_PERMISSION_FLAG_CLICKS_MIDDLE | UI_PERMISSION_FLAG_CLICKS_RIGHT));

  event.mouse_buttons_active &= ~mouse_button_perms;

  if(mouse_button_perms && event.mouse_buttons_active == 0) {
    event.input_flags &= ~(UI_INPUT_FLAG_MOUSE_PRESS | UI_INPUT_FLAG_MOUSE_RELEASE | UI_INPUT_FLAG_MOUSE_CLICK);
  }

  if(perms & UI_PERMISSION_FLAG_SCROLL) {
    event.input_flags &= ~UI_INPUT_FLAG_MOUSE_SCROLL;
    event.scroll_delta = (Vector2){0};
  }

  if(perms & UI_PERMISSION_FLAG_KEYBOARD) {
    event.input_flags &= ~UI_INPUT_FLAG_KEYBOARD_PRESSED;
    event.modifier_keys = 0;
    event.keyboard_key_pressed = 0;
  }

  return event;
}

func void ui_get_frame_input(UI_context *ui) {
  UI_event *event = &ui->input_event;

  *event = (UI_event){0};

  ui->took_input_event = 0;

  event->input_flags = 0;
  event->mouse_buttons_active = 0;

  event->mouse_pos = GetMousePosition();
  event->scroll_delta = GetMouseWheelMoveV();
  event->keyboard_key_pressed = PeekKeyPressed();

  int raylib_mouse_buttons[] = {
    MOUSE_LEFT_BUTTON,
    MOUSE_RIGHT_BUTTON,
    MOUSE_MIDDLE_BUTTON,
  };

  for(int i = 0; i < ARRLEN(raylib_mouse_buttons); i++) {

    if(IsMouseButtonPressed(raylib_mouse_buttons[i])) {
      event->input_flags |= UI_INPUT_FLAG_MOUSE_PRESS;
      event->mouse_buttons_active |= (UI_mouse_button_mask)(1<<i);
    }

    if(IsMouseButtonReleased(raylib_mouse_buttons[i])) {
      event->input_flags |= UI_INPUT_FLAG_MOUSE_RELEASE;
      event->mouse_buttons_active |= (UI_mouse_button_mask)(1<<i);
    }

  }

  if(event->scroll_delta.x != 0.0f || event->scroll_delta.y != 0.0f) {
    event->input_flags |= UI_INPUT_FLAG_MOUSE_SCROLL;
  }

  /* NOTE(jfd 29/07/25)
   *
   * Temporarily using IsKeyDown() instead of saving the modifier_keys across frames
   * so that I can reset the whole ui->input_event struct each frame.
   */
#define X(mod) \
  if(IsKeyDown(KEY_LEFT_##mod) || IsKeyDown(KEY_RIGHT_##mod)) { \
    event->modifier_keys |= UI_MOD_MASK_##mod; \
  }
  UI_MODIFIER_KEYS;
#undef X

#if 0
#define X(mod) \
  if(IsKeyPressed(KEY_LEFT_##mod) || IsKeyPressed(KEY_RIGHT_##mod)) { \
    event->modifier_keys |= UI_MOD_MASK_##mod; \
  } \
  if(IsKeyReleased(KEY_LEFT_##mod) || IsKeyReleased(KEY_RIGHT_##mod)) { \
    event->modifier_keys &= ~UI_MOD_MASK_##mod; \
  }
  UI_MODIFIER_KEYS;
#undef X
#endif

  if(event->keyboard_key_pressed != 0) {

    if(event->modifier_keys == 0) {
      event->input_flags |= UI_INPUT_FLAG_KEYBOARD_PRESSED;
    }

  }

}

func void ui_begin_build(UI_context *ui) {
  arena_clear(ui->build_arena);

  ui->build_index++;

  UI_box *dummy = ui->dummy;
  memory_zero(dummy, sizeof(UI_box));

  ui->root = dummy;

#define X(lower, lower_alt, type, init, stack_size) \
  ui_push_prop(lower, init);
  UI_STYLE_PROPERTIES;
  UI_OTHER_PROPERTIES;
  UI_NON_BOX_PROPERTIES;
#undef X

#define X(lower, lower_alt, type, init, stack_size) \
  type cur_##lower = ui_prop_top(lower); \
  memory_copy(&ui->root->lower_alt, &cur_##lower, sizeof(type));
  UI_STYLE_PROPERTIES;
  UI_OTHER_PROPERTIES;
#undef X

  ui_push_prop(parent, ui->root);

  ui_get_frame_input(ui);
  ui->mouse_pos = ui->input_event.mouse_pos;

  {
    b32 has_active = 0;
    for(UI_mouse_button btn = UI_MOUSE_BUTTON_LEFT; btn < UI_MOUSE_BUTTON_COUNT; btn++) {
      if(!ui_key_match(ui->active_box_key[btn], ui_key_nil())) {
        has_active = 1;
      }
    }

    if(!has_active) {
      ui->hot_box_key = ui_key_nil();
    }
  }

  {
    ui->drop_hot_box_key = ui_key_nil();
  }

}

// TODO make an iterative version of this
func f32 ui_calc_downward_dependent_sizes(UI_box *box, int axis, int layout_axis) {
  f32 sum = 0;

  if(box->first) {

    sum = ui_calc_downward_dependent_sizes(box->first, axis, box->child_layout_axis);

    if(box->semantic_size[axis].kind == UI_SIZE_CHILDREN_SUM) {
      box->fixed_size[axis] = sum;
    } else {
      sum += box->fixed_size[axis];
    }

  }

  {

    if(axis == layout_axis) {
      if(!(box->flags & (UI_BOX_FLAG_FLOATING_X<<axis))) {
        sum += box->fixed_size[axis];
      }

      if(box->next) {
        sum += ui_calc_downward_dependent_sizes(box->next, axis, layout_axis);
      }
    } else {
      if(!(box->flags & (UI_BOX_FLAG_FLOATING_X<<axis))) {
        sum = MAX(box->fixed_size[axis], sum);
      }

      if(box->next) {
        sum = MAX(ui_calc_downward_dependent_sizes(box->next, axis, layout_axis), sum);
      }
    }

  }

  return sum;
}

func void ui_end_build(UI_context *ui) {

  // TODO why not just clear all the stacks??
#define X(lower, lower_alt, type, init, stack_size) ui_clear_prop(lower);
  UI_PROPERTIES;
  UI_NON_BOX_PROPERTIES;
#undef X

  /* prune box tree */
  for(u64 table_i = 0; table_i < MAX_TABLE_SLOTS; table_i++) {
    UI_box *box = ui->box_table[table_i].first;
    for(; box; box = box->hash_next) {
      if(box->last_visited_build_index < ui->build_index || ui_key_match(box->key, ui_key_nil())) {
        dll_remove_np(
            ui->box_table[table_i].first,
            ui->box_table[table_i].last,
            box,
            hash_next,
            hash_prev);
        sll_stack_push(ui->box_free_list, box);
      }
    }
  }

  for(int axis = 0; axis < 2; axis++) {
    UI_PROFILE(ui_prof_time_to_do_1st_pass) {

      UI_PROFILE(ui_prof_time_to_calc_absolute_sized_boxes) { /* visit nodes with UI_SIZE_PIXELS or UI_SIZE_TEXT_CONTENT */

        for(UI_box *node = ui->root; node;) {

          UI_size size = node->semantic_size[axis];

          switch(size.kind) {
            default:
              break;
            case UI_SIZE_PIXELS:
              {
                node->fixed_size[axis] = size.value;
              } break;
            case UI_SIZE_TEXT_CONTENT:
              arena_scope(ui->build_arena) {

                Vector2 text_size =
                  MeasureTextEx(
                      ui->fonts[node->font_id],
                      cstr_copy_str8(ui->build_arena, node->text),
                      node->font_size,
                      node->font_spacing);

                f32 text_size_on_axis = (&(text_size.x))[axis];

                f32 padding = size.value + TIMES2(node->padding);

                node->fixed_size[axis] = text_size_on_axis + padding;

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
          f32 size_value = 0.0f;

          if(size.kind == UI_SIZE_PERCENT_OF_PARENT) {
            for(UI_box *parent = node->parent; parent; parent = parent->parent) {
              if(parent->flags & (UI_BOX_FLAG_FIXED_WIDTH << axis) ||
                  parent->semantic_size[axis].kind == UI_SIZE_PIXELS ||
                  parent->semantic_size[axis].kind == UI_SIZE_TEXT_CONTENT ||
                  parent->semantic_size[axis].kind == UI_SIZE_PERCENT_OF_PARENT)
              {
                size_value = size.value * parent->fixed_size[axis];
                break;
              }
            }

            node->fixed_size[axis] = size_value;
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
            f32 allowed_size = node->fixed_size[axis];

            for(UI_box *child = node->first; child; child = child->next) {
              if(!(child->flags & (UI_BOX_FLAG_FLOATING_X<<axis))) {
                f32 child_size = child->fixed_size[axis];
                f32 violation = child_size - allowed_size;
                f32 max_fixup = child_size;
                f32 fixup = Clamp(violation, 0, max_fixup);

                if(fixup > 0) {
                  child->fixed_size[axis] -= fixup;
                }
              }
            }
          }

          if(axis == node->child_layout_axis && !(node->flags &(UI_BOX_FLAG_OVERFLOW_X << axis))) {
            f32 total_allowed_size = node->fixed_size[axis];
            f32 total_size = 0;
            f32 total_weighted_size = 0;

            for(UI_box *child = node->first; child; child = child->next) {
              if(!(child->flags & (UI_BOX_FLAG_FLOATING_X<<axis))) {
                total_size += child->fixed_size[axis];
                total_weighted_size += child->fixed_size[axis] * (1-child->semantic_size[axis].strictness);
              }
            }

            f32 violation = total_size - total_allowed_size;
            
            if(violation > 0 && total_weighted_size > 0) arena_scope(ui->build_arena)
            {
              f32 *child_fixups = push_array(ui->build_arena, f32, node->child_count);

              {
                u64 child_index = 0;
                for(UI_box *child = node->first; child; child = child->next, child_index += 1) {
                  if(!(child->flags & (UI_BOX_FLAG_FLOATING_X<<axis))) {
                    f32 fixup_size_this_child = child->fixed_size[axis] * (1-child->semantic_size[axis].strictness);
                    fixup_size_this_child = CLAMP_BOT(0, fixup_size_this_child);
                    child_fixups[child_index] = fixup_size_this_child;
                  }
                }
              }

              {
                u64 child_index = 0;
                for(UI_box *child = node->first; child; child = child->next, child_index += 1) {
                  if(!(child->flags & (UI_BOX_FLAG_FLOATING_X<<axis))) {
                    f32 fixup_percent = (violation / total_weighted_size);
                    fixup_percent = Clamp(fixup_percent, 0, 1);
                    child->fixed_size[axis] -= child_fixups[child_index] * fixup_percent;
                  }
                }
              }

            }

          }

          /* fix the upward dependent sizes */
          if(node->flags & (UI_BOX_FLAG_OVERFLOW_X << axis)) {
            for(UI_box *child = node->first; child; child = child->next) {
              if(child->semantic_size[axis].kind == UI_SIZE_PERCENT_OF_PARENT) {
                child->fixed_size[axis] = node->fixed_size[axis] * child->semantic_size[axis].value;
              }
            }
          }

          for(UI_box *child = node->first; child; child = child->next) {
            child->fixed_size[axis] = MAX(child->fixed_size[axis], child->min_size[axis]);
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
  }

  UI_PROFILE(ui_prof_time_to_calc_positions)
  { /* calculate the relative and then absolute positions of each box in pre order */

    u32 layout_axis = 0;
    u32 axis = 1;

    for(UI_box *node = ui->root; node;) {
      UI_box *parent = node->parent;

      f32 final_layout_axis_min = 0.0f;
      f32 final_axis_min = 0.0f;

      if(node->flags & (UI_BOX_FLAG_FLOATING_X << layout_axis)) {
        final_layout_axis_min = (&(node->fixed_position.x))[layout_axis];
      } else {
        f32 parent_layout_axis_pos = 0.0f;
        if(parent) {
          parent_layout_axis_pos = parent->final_rect_min[layout_axis];
        }

        f32 rel_layout_axis_pos = 0.0f;
        if(node->prev) {
          rel_layout_axis_pos = node->prev->computed_rel_pos[layout_axis] + node->prev->fixed_size[layout_axis];
        }

        node->computed_rel_pos[layout_axis] = rel_layout_axis_pos;

        final_layout_axis_min = parent_layout_axis_pos + rel_layout_axis_pos;
      }

      if(node->flags & (UI_BOX_FLAG_FLOATING_X << axis)) {
        final_axis_min = (&(node->fixed_position.x))[axis];
      } else {
        f32 parent_axis_pos = 0.0f;
        if(parent) {
          parent_axis_pos = parent->final_rect_min[axis];
        }

        final_axis_min = parent_axis_pos;
      }

      f32 layout_view_offset = 0.0f;
      f32 view_offset = 0.0f;

      if(parent) {
        parent->view_bounds[layout_axis] += node->fixed_size[layout_axis];
        parent->view_bounds[axis] = MAX(parent->view_bounds[axis], node->fixed_size[axis]);

        layout_view_offset = floor_f32(node->parent->view_offset[layout_axis]);
        view_offset = floor_f32(node->parent->view_offset[axis]);
      }

      node->final_rect_min[layout_axis] = final_layout_axis_min - layout_view_offset;
      node->final_rect_min[axis] = final_axis_min - view_offset;

      node->final_rect_max[layout_axis] = final_layout_axis_min + node->fixed_size[layout_axis] - layout_view_offset;
      node->final_rect_max[axis] = final_axis_min + node->fixed_size[axis] - view_offset;

      node->final_rect_min[0] = floor_f32(node->final_rect_min[0]);
      node->final_rect_min[1] = floor_f32(node->final_rect_min[1]);
      node->final_rect_max[0] = floor_f32(node->final_rect_max[0]);
      node->final_rect_max[1] = floor_f32(node->final_rect_max[1]);

      node->fixed_position.x = node->final_rect_min[0];
      node->fixed_position.y = node->final_rect_min[1];

      if(node->first) {
        /* NOTE(jfd 28/07/2025)
         *
         * The view bounds are zeroed here because they need to be zeroed immediately before
         * descending in to the children, as the view bounds are basically calculated as the union
         * of child dimensions.
         * 
         */

        node->view_bounds[0] = 0;
        node->view_bounds[1] = 0;

        layout_axis = node->child_layout_axis;
        axis = !layout_axis;

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

func UI_box_node* ui_push_box_node(UI_context *ui) {
  UI_box_node *node = 0;

  node = push_struct(ui->draw_arena, UI_box_node);

  return node;
}

func void dump_UI_box(const UI_box *box) {
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
    printf("  child_layout_axis = %d\n", box->child_layout_axis);

    printf("  text = \"%.*s\"\n", (int)box->text.len, box->text.s);

    printf("  min_size = [%.2f, %.2f]\n",
           box->min_size[0], box->min_size[1]);
    printf("  fixed_size = [%.2f, %.2f]\n",
           box->fixed_size[0], box->fixed_size[1]);

    printf("  fixed_size = [%.2f, %.2f]\n",
           box->fixed_size[0], box->fixed_size[1]);
    printf("  fixed_rel_pos = [%.2f, %.2f]\n",
           box->computed_rel_pos[0], box->computed_rel_pos[1]);

    printf("  final_rect.min = [%.2f, %.2f]\n",
           box->final_rect_min[0], box->final_rect_min[1]);
    printf("  final_rect.max = [%.2f, %.2f]\n",
           box->final_rect_max[0], box->final_rect_max[1]);

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

func void ui_draw(UI_context *ui) {

  for(UI_box *box = ui->root; box;) {

    //TraceLog(LOG_DEBUG, "drawing %s", cstrf(ui->temp, "'%S' %li", box->key.src_str, box->key.hash));

    Rectangle rec =
    {
      .x = box->final_rect_min[0],
      .y = box->final_rect_min[1],
      .width =  box->final_rect_max[0] - box->final_rect_min[0],
      .height = box->final_rect_max[1] - box->final_rect_min[1],
    };

    if(box->flags & UI_BOX_FLAG_DRAW_BACKGROUND) {
      DrawRectangleRoundedPro(rec,
          box->corner_radius[0],
          box->corner_radius[1],
          box->corner_radius[2],
          box->corner_radius[3],
          8,
          box->background_color);
    }

    if(box->flags & UI_BOX_FLAG_DRAW_BORDER) {
      DrawRectangleRoundedLinesPro(rec,
          box->corner_radius[0],
          box->corner_radius[1],
          box->corner_radius[2],
          box->corner_radius[3],
          8,
          box->border_size,
          box->border_color);
    }

    if(box->flags & UI_BOX_FLAG_DRAW_TEXT) arena_scope(ui->draw_arena) {

      char *text_cstr = cstr_copy_str8(ui->draw_arena, box->text);

      f32 box_mid_x = (box->final_rect_max[0] + box->final_rect_min[0]) * 0.5f;
      f32 box_mid_y = (box->final_rect_max[1] + box->final_rect_min[1]) * 0.5f;
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
            text_pos.x = box->final_rect_min[0] + box->padding;
          } break;
        case UI_TEXT_ALIGN_RIGHT:
          {
            text_pos.x = box->final_rect_max[0] - text_size.x - box->padding;
          } break;
      }

      DrawTextEx(
          ui->fonts[box->font_id],
          text_cstr,
          text_pos,
          box->font_size,
          box->font_spacing,
          box->text_color);

    }

    if(box->flags & UI_BOX_FLAG_CLIP) {
      BeginScissorMode((int)rec.x, (int)rec.y, (int)rec.width, (int)rec.height);
    }

#if 1
    arena_scope(ui->temp) {

      Vector2 pos = { rec.x, rec.y };

      SetTextLineSpacing(1);
      DrawTextEx(GetFontDefault(),
          (char*)str8f(ui->temp,
            "hash = %li\t\t\tsrc_str = %S"

            ,
            box->key.hash,
            box->key.src_str
            ).s,
          pos, 10, 1.0, GREEN);

      SetTextLineSpacing(5);
      DrawRectangleLinesEx(rec, 1.0, RED);
    }
#endif

    if(box->first) {
      box = box->first;
    } else {

      if(box->flags & UI_BOX_FLAG_CLIP) {
        EndScissorMode();
      }

      if(box->next) {
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

    }

  } /* for(UI_box_node *node = ui->root; node;) */

  arena_clear(ui->draw_arena);

}

