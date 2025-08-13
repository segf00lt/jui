#ifndef UI_WIDGET_C
#define UI_WIDGET_C

func void ui_divider(UI_context *ui, UI_size size) {
  UI_box *parent = ui_prop_top(parent);

  b32 horizontal = (parent->child_layout_axis == UI_AXIS_X);

  if(horizontal) {
    ui_semantic_width_set_next(size);
    ui_semantic_height_set_next(ui_percent_of_parent(1, 0));
  } else {
    ui_semantic_height_set_next(size);
    ui_semantic_width_set_next(ui_percent_of_parent(1, 0));
  }

  ui_child_layout_axis_set_next(parent->child_layout_axis);

  UI_box *box = ui_make_transient_box(ui, 0);

  ui_parent(box) ui_semantic_size(ui_percent_of_parent(1, 0))
  {
    ui_make_transient_box(ui, (horizontal) ? (UI_BOX_FLAG_DRAW_SIDE_RIGHT) : (UI_BOX_FLAG_DRAW_SIDE_BOTTOM));
    ui_make_transient_box(ui, 0);
  }

}

func UI_box* ui_row_begin(UI_context *ui) {
  return ui_named_row_begin(ui, str8_lit(""));
}

func UI_signal ui_row_end(UI_context *ui) {
  return ui_named_row_end(ui);
}

#define ui_row(ui) defer_loop(ui_row_begin((ui)), ui_row_end((ui)))

func UI_box* ui_named_row_begin(UI_context *ui, Str8 str) {
  ui_child_layout_axis_set_next(UI_AXIS_X);
  UI_box *box = ui_make_box_from_str(ui, 0, str);
  ui_prop_push(parent, box);
  return box;
}

func UI_signal ui_named_row_end(UI_context *ui) {
  UI_box *box = ui_prop_pop(parent);
  UI_signal sig = ui_signal_from_box(ui, box);
  return sig;
}

#define ui_named_row(ui, name) defer_loop(ui_named_row_begin((ui), (name)), ui_named_row_end((ui)))

func UI_box* ui_column_begin(UI_context *ui) {
  return ui_named_column_begin(ui, str8_lit(""));
}

func UI_signal ui_column_end(UI_context *ui) {
  return ui_named_column_end(ui);
}

#define ui_column(ui) defer_loop(ui_column_begin((ui)), ui_column_end((ui)))

func UI_box* ui_named_column_begin(UI_context *ui, Str8 str) {
  ui_child_layout_axis_set_next(UI_AXIS_Y);
  UI_box *box = ui_make_box_from_str(ui, 0, str);
  ui_prop_push(parent, box);
  return box;
}

func UI_signal ui_named_column_end(UI_context *ui) {
  UI_box *box = ui_prop_pop(parent);
  UI_signal sig = ui_signal_from_box(ui, box);
  return sig;
}

#define ui_named_column(ui, name) defer_loop(ui_named_column_begin((ui), (name)), ui_named_column_end((ui)))

func UI_signal ui_label(UI_context *ui, Str8 str) {
  UI_box *box = ui_make_box_from_key(ui, UI_BOX_FLAG_DRAW_TEXT, ui_key_nil());
  box->text = str;
  UI_signal sig = ui_signal_from_box(ui, box);
  return sig;
}

func UI_signal ui_labelf(UI_context *ui, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  Str8 str = str8fv(ui->temp, fmt, args);
  va_end(args);
  UI_signal sig = ui_label(ui, str);
  return sig;
}

func UI_box* ui_inset_begin(UI_context *ui, UI_size left, UI_size right, UI_size top, UI_size bottom) {
  Str8 str = {0};
  return ui_named_inset_begin(ui, left, right, top, bottom, str);
}

func UI_box* ui_named_inset_begin(UI_context *ui, UI_size left, UI_size right, UI_size top, UI_size bottom, Str8 str) {
  UI_box *container;

  Str8 outer_name     = (str.len > 0) ? str8_cat(ui->temp, str, str8_lit(":outer")) : str8_lit("");
  Str8 inner_name     = (str.len > 0) ? str8_cat(ui->temp, str, str8_lit(":inner")) : str8_lit("");
  Str8 container_name = (str.len > 0) ? str8_cat(ui->temp, str, str8_lit(":container")) : str8_lit("");

  UI_size sum = { .kind = UI_SIZE_CHILDREN_SUM, .strictness = 1.0f };
  UI_size fit = { .kind = UI_SIZE_PERCENT_OF_PARENT, .value = 1.0f };

  UI_box *cur_parent = ui_prop_top(parent);

  UI_axis layout_axis = cur_parent->child_layout_axis;

  UI_size width;
  UI_size height;

  if(layout_axis == UI_AXIS_X) {
    width = sum;
    height = fit;
  } else {
    width = fit;
    height = sum;
  }

  ui_flags_set_next(0);
  ui_semantic_width_set_next(width);
  ui_semantic_height_set_next(height);

  ui_named_column(ui, outer_name)
  {
    ui_spacer(ui, top);

    ui_flags_set_next(0);
    ui_semantic_width_set_next(width);
    ui_semantic_height_set_next(height);

    ui_named_row(ui, inner_name)
    {
      ui_spacer(ui, left);

      ui_semantic_width_set_next(width);
      ui_semantic_height_set_next(height);
      ui_child_layout_axis_set_next(layout_axis);
      container = ui_make_box_from_str(ui, 0, container_name);

      ui_spacer(ui, right);
    }

    ui_spacer(ui, bottom);
  }

  ui_prop_push(parent, container);

  return container; // TODO ??
}

func UI_signal ui_inset_end(UI_context *ui) {
  return ui_named_inset_end(ui);
}

func UI_signal ui_named_inset_end(UI_context *ui) {
  UI_box *box = ui_prop_pop(parent);
  UI_signal sig = ui_signal_from_box(ui, box);
  return sig;
}

func void ui_spacer(UI_context *ui, UI_size size) {
  ui_flags_set_next(0);
  ui_semantic_size_set_next(size);
  ui_make_transient_box(ui, 0);
}

func UI_signal ui_buttonf(UI_context *ui, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  Str8 str = str8fv(ui->temp, fmt, args);
  va_end(args);

  UI_signal sig = ui_button(ui, str);

  return sig;
}

func UI_signal ui_button(UI_context *ui, Str8 label) {

  UI_box *button_box = 0;

    UI_box_flags flags =
      UI_BOX_FLAG_DRAW_BACKGROUND |
      UI_BOX_FLAG_DRAW_TEXT |
      UI_BOX_FLAG_DRAW_BORDER |
      UI_BOX_FLAG_MOUSE_CLICKABLE |
      0;

    button_box = ui_make_box_from_str(ui, flags, label);

  UI_signal sig = ui_signal_from_box(ui, button_box);

  Color background_color = button_box->background_color;

  if(ui_key_match(ui_hot_box_key(ui), button_box->key)) {
    button_box->background_color = ColorBrightness(background_color, 0.12f);
  }

  if(ui_key_match(ui_active_box_key(ui, UI_MOUSE_BUTTON_LEFT), button_box->key)) {
    button_box->background_color = ColorBrightness(background_color, -0.17f);
  }

  return sig;
}

func UI_box* ui_pane_begin(UI_context *ui, f32 x, f32 y, f32 w, f32 h, Str8 str) {
  Vector2 pos = { x, y };
  ui_fixed_position_set_next(pos);
  ui_fixed_width_set_next(w);
  ui_fixed_height_set_next(h);
  ui_child_layout_axis_set_next(UI_AXIS_Y);
  UI_box *box = ui_make_box_from_str(ui,
      UI_BOX_FLAG_CLICKABLE |
      UI_BOX_FLAG_DRAW_BACKGROUND |
      UI_BOX_FLAG_DRAW_BORDER |
      UI_BOX_FLAG_CLIP |
      UI_BOX_FLAG_FLOATING |
      UI_BOX_FLAG_FIXED_SIZE |
      0,
      str);

  ui_prop_push(parent, box);
  ui_prop_push(semantic_width, ui_percent_of_parent(1.0f, 0.0f));
  return box;
}

func UI_box* ui_pane_beginf(UI_context *ui, f32 x, f32 y, f32 w, f32 h, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  Str8 str = str8fv(ui->temp, fmt, args);
  va_end(args);

  UI_box *box = ui_pane_begin(ui, x, y, w, h, str);

  return box;
}

func UI_signal ui_pane_end(UI_context *ui) {
  ui_prop_pop(semantic_width);
  UI_box *box = ui_prop_pop(parent);
  UI_signal sig = ui_signal_from_box(ui, box);
  return sig;
}

#define ui_pane(ui, x, y, w, h, str) defer_loop(ui_pane_begin((ui), (x), (y), (w), (h), (str)), ui_pane_end((ui)))


#endif
