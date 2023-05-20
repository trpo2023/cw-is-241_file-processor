#pragma once

#include <ncurses.h>

// Принимает: menu - указатель на текущее окно
//
// Функция выводит новое окно с информацией о работе программы
int show_info(WINDOW* menu);