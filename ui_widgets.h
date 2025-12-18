#ifndef UI_WIDGET_H
#define UI_WIDGET_H

void ui_divider(UI_size size);

UI_box* ui_named_inset_begin(UI_size left, UI_size right, UI_size top, UI_size bottom, Str8 str);
UI_signal ui_inset_end(void);
UI_signal ui_named_inset_end(void);
void ui_spacer(UI_size size);
UI_signal ui_buttonf(char *fmt, ...);
UI_signal ui_button(Str8 label);
UI_signal ui_labelf(char *fmt, ...);
UI_signal ui_label(Str8 str);

UI_box* ui_column_begin(void);
UI_box* ui_named_column_begin(Str8 str);
UI_signal ui_column_end(void);
UI_signal ui_named_column_end(void);

UI_box* ui_row_begin(void);
UI_box* ui_named_row_begin(Str8 str);
UI_signal ui_row_end(void);
UI_signal ui_named_row_end(void);
UI_box* ui_inset_begin(UI_size left, UI_size right, UI_size top, UI_size bottom);

UI_box* ui_pane_begin(f32 x, f32 y, f32 w, f32 h, Str8 str);

UI_box* ui_pane_beginf(f32 x, f32 y, f32 w, f32 h, char *fmt, ...);
UI_signal ui_pane_end(void);

#endif
