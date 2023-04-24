#pragma once
#include <ncurses.h>

WINDOW* init_menu();
int item_select(WINDOW* menu, int item_number);
WINDOW* init_sub_window(WINDOW* menu, int max_y, int max_x);
void start(WINDOW* menu);
char* select_dir(WINDOW* menu);
GList* pattern_input(WINDOW* menu, GList** input_strings, GList* samples);
void free_input_string(void* str);