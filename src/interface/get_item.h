#pragma once

#include <glib.h>
#include <ncurses.h>

// Принимает: win - указатель на текущее окно
//            items - указатель на массив строк
//            i - введённый символ (число)
//            offset - смещение по координате
//            max - число, показывающая край рабочей области
//
// Функция для выбода списка в интерфейсном окне
int select_items(WINDOW* win, const char* items[], int i, int offset, int max);

void print_str(
        WINDOW* sub,
        char* str,
        int i,
        int x,
        int small,
        int dx,
        int dy,
        int x_offset);

// Принимает: sub - указатель на текущее окно
//            dir_list - указатель на список директорий
//            y - координата по оси y
//            len - длина списка директорий
//            str_cnt - количество директорий на одной странице
//            a - смещение по y
//            b - смещение по x
//
// Функция осуществляет передвежение по списку
// и выбор элемента из него
//
// Возвращает: строку с выбранным элементом
char* get_item(
        WINDOW* sub,
        GList* dir_list,
        int y,
        size_t len,
        int str_cnt,
        int small,
        int a,
        int b);