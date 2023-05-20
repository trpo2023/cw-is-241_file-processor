#pragma once

#include <glib.h>
#include <ncurses.h>

// Принимает: sub - указатель на окно
//            x - координата по оси x
//            cnt - число страниц
//            max_cnt - максимальное число страниц
//
// Функция выводит текущее число страниц и максимальное
void print_counter(WINDOW* sub, int x, int cnt, int max_cnt);

// Принимает: win - указатель на окно
//            y - координата по оси y
//            x - координата по оси x
//            str - Указатель на строку
//
// Функция выделяет определённую строку в окне
void mvwprintw_highlite(WINDOW* win, int y, int x, const char* str);

// Принимает: win - указатель на окно
//            list - указатель на список
//            str_cnt - число строк
//            a - смещение по y
//            b - смешение по x
//
// Функция выводит строки из указанного списка в окно
void print_items(WINDOW* sub, GList* list, int* str_cnt, int a, int b);

// Принимает: sub - указатель на окно
//            list - указатель на список
//            cnt - указатель на число строк
//            p - номер текущей страницыы
//            max_p - максимально возможная страница
//            a - смещение по y
//            b - смешение по x
//
// Функция выводит на экран число страниц и информацию из указанного списка
void print_new_page(
        WINDOW* sub, GList* list, int* cnt, int p, int max_p, int a, int b);

// Принимает: sub - указатель на окно
//            input_strings - список со строками
//
// Функция выводит на экран введёные строки и считает их кол-во
//
// Возвращает: кол-во введённых строк
int print_input_strings(WINDOW* sub, GList* input_strings);

// Принимает: sub - указатель на окно
//            str_list - список со строками
//            y - координата по оси y
//
// Функция выводит в указанное окно информацию о программе
int print_info(WINDOW* sub, GList* str_list, int y);

int print_list_in_current_dir(WINDOW* menu, char* path);