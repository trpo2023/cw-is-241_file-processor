#pragma once

#include <glib.h>

#include <libfileproc/rename.h>

// Принимает: opt - указатель на структуру с опциями
//            input_strings - указатель на массив с входящими строками
//            patterns - указатель на массив с шаблонами
//
// Функция освобождает память, выделенную под массив с шаблонами
// и под массив с входящими строками
// Возвращает опции к обыным настройкам
void clean_data(Option* opt, GList** input_strings, GList** patterns);

// Принимает: patterns - указатель на структуру с шаблонами
//
// Функция освобождает память, выделенную под поля структуры
// и под саму структуру
void free_SplittedPattern(void* patterns);

// Принимает: files_patterns_list - список файлов, готовых к переименованию
//            files_list - список файлов в текущей директории
//
// Очищает память, использовавшуюся для выполнения
// функции rename_and_get_renamed_list
void clean_memory(GList* files_patterns_list, GList* files_list);

// Принимает: data - указатель на структуру в которой лежат
// старый и новый путь к файлу
//
// Очищает поля структуры data, а после и память выделенную под структуру
void free_renamed_files(void* data);

// Принимает: renamed_file_list - указатель на список файлов готовых к переименованию
//            list - указатель на список переименнованых файлов
//
// Очищает память, выделенную на renamed_file_list и на list
void free_renamed_list(GList* renamed_file_list, GList* list);