#pragma once
#include <ncurses.h>

enum MenuItems {
    INPUT_PATTERN,
    SELECT_DIR,
    PROCESS,
    SELECT_OPT,
    EXIT,
};

enum OptionItems {
    REGISTER,
    CLEAR,
    BACK,
};

WINDOW* init_menu();
int select_menu_items(WINDOW* menu, int item_number);
WINDOW* init_sub_window(WINDOW* menu, int max_y, int max_x);
void start(WINDOW* menu);
char* select_dir(WINDOW* menu, char* current_dir);
GList* pattern_input(WINDOW* menu, GList** input_strings, GList* samples);