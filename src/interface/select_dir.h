#pragma once

#include <ncurses.h>

// Принимает: menu - указатель на окно с меню
//            current_dir - путь к текущей директории
//
// Функция вызывает новое окно для выбора директории
//
// Возвращает: путь к выбранной
// пользователем директории
char* select_dir(WINDOW* menu, char* current_dir);