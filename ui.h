#ifndef UI_H
#define UI_H

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
  X(DRAW_SIDE_LEFT) \
  X(DRAW_SIDE_RIGHT) \
  X(DRAW_SIDE_TOP) \
  X(DRAW_SIDE_BOTTOM) \
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
#define UI_BOX_FLAG_DRAW (UI_BOX_FLAG_DRAW_BACKGROUND | UI_BOX_FLAG_DRAW_TEXT | UI_BOX_FLAG_DRAW_BORDER)
#define UI_BOX_FLAG_CLICKABLE (UI_BOX_FLAG_MOUSE_CLICKABLE | UI_BOX_FLAG_KEYBOARD_CLICKABLE)

#define UI_BOX_FLAG_ALL (((UI_box_flags)(1ull<<UI_BOX_FLAG_INDEX_MAX))-1ull)


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
  X( semantic_width,     semantic_size[0],     UI_size,          ((UI_size){ .kind = UI_SIZE_NONE }),               16          ) \
  X( semantic_height,    semantic_size[1],     UI_size,          ((UI_size){ .kind = UI_SIZE_NONE }),               16          ) \
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
  X( flags,              flags,                     UI_box_flags,     (0),                       16          ) \
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

#define ui_background_color(value) ui_prop(background_color, (value))

#define ui_text_color(value) ui_prop(text_color, (value))

#define ui_background_color_set_next(value) ui_prop_set_next(background_color, (value))

#define ui_text_color_set_next(value) ui_prop_set_next(text_color, (value))

#define ui_border_color_set_next(value) ui_prop_set_next(border_color, (value))
#define ui_border_size_set_next(value) ui_prop_set_next(border_size, (value))

#define ui_corner_radius_0_set_next(value) ui_prop_set_next(corner_radius_0, (value))
#define ui_corner_radius_1_set_next(value) ui_prop_set_next(corner_radius_1, (value))
#define ui_corner_radius_2_set_next(value) ui_prop_set_next(corner_radius_2, (value))
#define ui_corner_radius_3_set_next(value) ui_prop_set_next(corner_radius_3, (value))
#define ui_corner_radius_set_next(value) (ui_corner_radius_0_set_next((value)), ui_corner_radius_1_set_next((value)), ui_corner_radius_2_set_next((value)), ui_corner_radius_3_set_next((value)))
#define ui_font_id_set_next(value) ui_prop_set_next(font_id, (value))
#define ui_font_size_set_next(value) ui_prop_set_next(font_size, (value))
#define ui_font_spacing_set_next(value) ui_prop_set_next(font_spacing, (value))

#define ui_text_align_set_next(value) ui_prop_set_next(text_align, (value))

#define ui_semantic_width_set_next(value) ui_prop_set_next(semantic_width, (value))
#define ui_semantic_height_set_next(value) ui_prop_set_next(semantic_height, (value))
#define ui_semantic_size_set_next(value) (ui_semantic_width_set_next((value)), ui_semantic_height_set_next((value)))

#define ui_fixed_width_set_next(value) ui_prop_set_next(fixed_width, (value))
#define ui_fixed_height_set_next(value) ui_prop_set_next(fixed_height, (value))
#define ui_fixed_size_set_next(value) (ui_fixed_width_set_next((value)), ui_fixed_height_set_next((value)))

#define ui_padding_set_next(value) ui_prop_set_next(padding, (value))

#define ui_fixed_position_set_next(value) ui_prop_set_next(fixed_position, (value))

#define ui_child_layout_axis_set_next(value) ui_prop_set_next(child_layout_axis, (value))

#define ui_parent_set_next(value) ui_prop_set_next(parent, (value))

#define ui_flags_set_next(value) ui_prop_set_next(flags, (value))
#define ui_exclude_flags_set_next(value) ui_prop_set_next(exclude_flags, (value))

#define ui_permission_flags_set_next(value) ui_prop_set_next(permission_flags, (value))

#define DECL_UI_ARR(T) DECL_ARR_TYPE_NAME(T, UI_ARR_PREFIX##T)

/*
 * typedefs
 */

typedef struct UI_state UI_state;
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

typedef enum UI_state_flags_index {
  UI_CONTEXT_FLAG_INDEX_NONE = -1,
#define X(f) UI_CONTEXT_FLAG_INDEX_##f,
  UI_CONTEXT_FLAGS
#undef X
    UI_CONTEXT_FLAG_INDEX_MAX,
} UI_state_flags_index;

typedef u64 UI_state_flags;
STATIC_ASSERT(UI_CONTEXT_FLAG_INDEX_MAX < 64, UI_CONTEXT_FLAG_INDEX_MAX__is_less_than_64);

#define X(f) const UI_state_flags UI_CONTEXT_FLAG_##f = (1ull<<UI_CONTEXT_FLAG_INDEX_##f);
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
  u64 debug_id;

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

struct UI_state {
  Arena *arena;
  Arena *temp;
  Arena *build_arena;
  Arena *draw_arena;

  u64 build_index;

  u64 debug_id_counter;

  UI_box __dummy;
  UI_box *dummy;
  UI_box *root;
  u64 build_box_count;

  UI_box_hash_slot box_table[MAX_TABLE_SLOTS];

  Font fonts[8];

  UI_state_flags flags;

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

#define X(lower, lower_alt, type, init, stack_size) b32 lower##_auto_pop;
  UI_PROPERTIES
  UI_NON_BOX_PROPERTIES;
#undef X

};

/*
 * function headers
 */

UI_state* ui_state_alloc(void);
void      ui_state_select(UI_state *state);
UI_state* ui_state_get_selected(void);
void      ui_state_free(UI_state *ui);

UI_key ui_key_nil(void);
UI_key ui_key_from_str(Str8 str);
b32 ui_key_match(UI_key a, UI_key b);

UI_box* ui_get_box_from_key(UI_key key);
UI_box* ui_get_box_from_str(Str8 s);

UI_signal ui_signal_from_box(UI_box *box);

Str8 ui_strip_id_from_text(Str8 text);

Vector2 ui_drag_delta(UI_state *ui);

UI_box* ui_make_box_from_key(UI_box_flags flags, UI_key key);
UI_box* ui_make_box_from_str(UI_box_flags flags, Str8 str);
UI_box* ui_make_box_from_strf(UI_box_flags flags, char *fmt, ...);
UI_box* ui_make_transient_box(UI_box_flags flags);

UI_box_node* ui_push_box_node(void);

f32 ui_calc_downward_dependent_sizes(UI_box *box, int axis, int layout_axis);

void ui_begin_build(void);
void ui_end_build(void);
void ui_draw(void);
UI_event ui_get_event(void);
void ui_get_frame_input(void);

UI_key ui_hot_box_key(void);
UI_key ui_drop_hot_box_key(void);
UI_key ui_active_box_key(UI_mouse_button btn);

/*
 * macros
 */

#define ui_build() defer_loop(ui_begin_build(), ui_end_build())
#define ui_prop_push(name, value) (arr_push(ui_state->name##_stack, (value)))
#define ui_prop_pop(name) (arr_pop(ui_state->name##_stack))
#define ui_prop_set_next(name, value) (arr_push(ui_state->name##_stack, (value)), (ui_state->name##_auto_pop = 1))
#define ui_clear_prop(name) (arr_clear(ui_state->name##_stack))
#define ui_prop(name, value) defer_loop(ui_prop_push(name, (value)), ui_prop_pop(name))
#define ui_prop_top(name) (arr_last(ui_state->name##_stack))

#define ui_mouse_button_mask(button) ((UI_mouse_button_mask)(1<<(button)))


#define ui_pixels(val, strict) ((UI_size){ .kind = UI_SIZE_PIXELS, .value = (f32)(val), .strictness = (f32)(strict) })
#define ui_text_content(strict) ((UI_size){ .kind = UI_SIZE_TEXT_CONTENT, .value = 0.0f, .strictness = (f32)(strict) })
#define ui_percent_of_parent(val, strict) ((UI_size){ .kind = UI_SIZE_PERCENT_OF_PARENT, .value = (f32)(val), .strictness = (f32)(strict) })
#define ui_children_sum(strict) ((UI_size){ .kind = UI_SIZE_CHILDREN_SUM, .value = 0.0f, .strictness = (f32)(strict) })



#endif
