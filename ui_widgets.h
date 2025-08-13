#ifndef UI_WIDGET_H
#define UI_WIDGET_H

void ui_divider(UI_context *ui, UI_size size);

UI_box* ui_row_begin(UI_context *ui);
UI_box* ui_named_row_begin(UI_context *ui, Str8 str);
UI_signal ui_row_end(UI_context *ui);
UI_signal ui_named_row_end(UI_context *ui);
UI_box* ui_inset_begin(UI_context *ui, UI_size left, UI_size right, UI_size top, UI_size bottom);
UI_box* ui_named_inset_begin(UI_context *ui, UI_size left, UI_size right, UI_size top, UI_size bottom, Str8 str);
UI_signal ui_inset_end(UI_context *ui);
UI_signal ui_named_inset_end(UI_context *ui);
void ui_spacer(UI_context *ui, UI_size size);
UI_signal ui_buttonf(UI_context *ui, char *fmt, ...);
UI_signal ui_button(UI_context *ui, Str8 label);
UI_signal ui_labelf(UI_context *ui, char *fmt, ...);
UI_signal ui_label(UI_context *ui, Str8 str);

UI_box* ui_column_begin(UI_context *ui);
UI_box* ui_named_column_begin(UI_context *ui, Str8 str);
UI_signal ui_column_end(UI_context *ui);
UI_signal ui_named_column_end(UI_context *ui);

UI_box* ui_pane_begin(UI_context *ui, f32 x, f32 y, f32 w, f32 h, Str8 str);

UI_box* ui_pane_beginf(UI_context *ui, f32 x, f32 y, f32 w, f32 h, char *fmt, ...);
UI_signal ui_pane_end(UI_context *ui);

#endif
