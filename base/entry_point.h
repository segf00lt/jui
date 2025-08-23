#ifndef _ENTRY_POINT_H
#define _ENTRY_POINT_H

#error entry_point module unimplemented

internal void main_thread_base_entry_point(int arguments_count, char **arguments);
internal void supplement_thread_base_entry_point(void (*entry_point)(void *params), void *params);

#endif
