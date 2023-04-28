#pragma once
#include <ncurses.h>

#define INPUT_PATTERN 0
#define SELECT_DIR 1
#define PROCESS 2
#define SELECT_OPT 3
#define EXIT 4

WINDOW* init_menu();
int select_menu_items(WINDOW* menu, int item_number);
WINDOW* init_sub_window(WINDOW* menu, int max_y, int max_x);
void start(WINDOW* menu);
char* select_dir(WINDOW* menu, char* current_dir);
GList* pattern_input(WINDOW* menu, GList** input_strings, GList* samples);
void free_input_string(void* str);