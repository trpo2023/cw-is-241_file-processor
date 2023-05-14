#pragma once

#include <ncurses.h>
#include <string.h>

// Функция создает новое окно с меню интерфейса
//
// Возвращает: указатель на созданное окно
WINDOW* init_menu();

// Принимает: menu - указатель на окно с меню
//            max_y - максимальное координата окна по оси y
//            max_x - максимальное координата окна по оси x
//
// Функция создает "подокно" в меню
//
// Возвращает: указатель на созданное "подокно"
WINDOW* init_sub_window(WINDOW* menu, int max_y, int max_x);

// Принимает: sub - указатель на окно
//
// Функция, которая закрывает указанное окно
void remove_current_window(WINDOW* sub);

// Принимает: menu - указатель на окно с меню
//            x - указатель на координату по оси x
//            y - указатель на координату по оси y
//
// Функци открывает новое окно для ввода шаблонов
//
// Возвращает: указатель на созданное окно
WINDOW* open_pattern_window(WINDOW* menu, int* x, int* y);

// Принимает: menu - указатель на окно с меню
//            x - указатель на координату по оси x
//            y - указатель на координату по оси y
//
// Функци открывает новое окно для вывода информации
//
// Возвращает: указатель на созданное окно
WINDOW* open_info_window(WINDOW* menu, int* y, int* x);

// Принимает: sub - указатель на окно
//
// Функция закрывает окно для ввода шаблонов
void close_pattern_window(WINDOW* sub);