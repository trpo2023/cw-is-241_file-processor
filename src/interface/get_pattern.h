#pragma once

#include <glib.h>
#include <ncurses.h>

// Принимает: menu - указатель на текущее окно
//            input_strings - указатель на список со входящими строками
//            patterns - указатель на список с шаблонами
//
// Функция получает список со строками, инициализирует новое окно и
// Вызывает функцию для заненсение корректных строк в список шаблонов
//
// Возвращает: указатель на список шаблнов
GList* pattern_input(WINDOW* menu, GList** input_strings, GList* patterns);

// Принимает: sub - указатель на текущее окно
//            cnt - текущее число шаблонов
//            max_ninstr - максимально возможное число шаблонов в одном окне
//
// Функция проверяет текущее кол-во шаблонов с максимальным,
// если их максимальное количество, то выводит сообщение об этом
//
// Возвращает: -1 - число шаблонов максимально возможное
//             0 - число шаблонов не максимальное
int check_max_ninstr(WINDOW* sub, int cnt, int max_ninstr);

// Принимает: sub - указатель на текущее окно
//            cnt - указатель на число кол-во строк
//            max_ninstr - максимально возможное число шаблонов в одном окне
//            x - координата по оси x
//            str - укзатель на строку
//            patterns - указатель на список с шаблонами
//            input_strings - указатель на список со входящими строками
//
// Функция проверяет каждую входящую строку на корректность и вносит её в список
// с шаблонами (Вызывается внутри функции pattern_input)
//
// Возвращает: -1 - в случае возникновение ошибок с выделением памяти
//             0 - если отработала корректно
int get_string(
        WINDOW* sub,
        int* cnt,
        int max_ninstr,
        int x,
        char* str,
        GList** patterns,
        GList** input_strings);