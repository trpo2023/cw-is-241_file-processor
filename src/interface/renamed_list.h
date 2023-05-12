#pragma once

#include <glib.h>
#include <ncurses.h>

#include <libfileproc/rename.h>

// Принимает: sub - указатель на текущее окно
//            renamed_files - список переименованных файлов
//
// Функция получает новое имя файла
//
// Возвращает: список с новыми именами файлов
GList* get_renamed_list(WINDOW* sub, GList* renamed_files);

// Принимает: sub - указатель на текущее окно
//            renamed_files - список переименованных файлов
//            x - координата по оси x
//            y - координата по оси y
//
// Функция выводит на экран новое имя файлов из списка перименнованых файлов
void print_renamed_list(WINDOW* sub, GList* renamed_list, int x, int y);

// Принимает: sub - указатель на текущее окно
//            renamed_file_list - список переименованных файлов
//            x - координата по оси x
//            y - координата по оси y
//
// Функция вызывает функцию для получение новых имён файлов и вывода их на экран
void process_with_renamed_list(
        WINDOW* sub, GList* renamed_file_list, int x, int y);

// Принимает: renamed_file_list - список переименованных файлов
//            sub - указатель на текущее окно
//
// Функция, которая выводит на экран сообщение об ошибки или успешном
// переименование
void print_succes_message(GList* renamed_file_list, WINDOW* sub);

// Принимает: menu - указатель на окно с меню
//            sample - указатель на список с шаблонами
//            dir_path - путь к директории
//            opt - указатель на структуру с опциями
//
// Функция вызывает предыдущие ф-ии из данного файла
// с целью переименования и получения списка
// переименовынных файлов
void process(WINDOW* menu, GList* sample, char* dir_path, Option* opt);