#pragma once

#include <glib.h>
#include <ncurses.h>

#include <libfileproc/rename.h>

enum OptionItems {
    REGISTER, // Выбор регистра
    CLEAR,    // Очистить опции
    BACK,     // назад
};

// Принимает: menu - указатель на окно с меню
//            opt - указатель на структуру с опциями
//            inp_s - список с входящими строками
//            samp -  список с шаблонами
//            curr_dir - путь к текущей директории
//
// Функция создает окно с выбором опций
// и в зависимости от введёной строки выбирает нужную
// и выполняет соответсвующие данной опции действия
int select_option(
        WINDOW* menu, Option* opt, GList** inp_s, GList** samp, char* curr_dir);

// Принимает: sub - указатель на текущее окно
//            opt - указатель на структуру с опциями
//            i - номер выбранной опции
//
// Функция выводит в указанное окно регистр,
// в зависимости от номера опции
void print_opt(WINDOW* sub, Option* opt, int i);

// Принимает: menu - указатель на окно с меню
//            opt - указатель на структуру с опциями
//            inp_s - список с входящими строками
//            samp -  список с шаблонами
//            curr_dir - путь к текущей директории
//
// Функция возвращает настройки опций к стандартным,
// очищает шаблоны
void set_default_settings(
        WINDOW* menu, Option* opt, GList** inp_s, GList** samp, char* curr_dir);