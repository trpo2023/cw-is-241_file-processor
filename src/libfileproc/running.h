#pragma once

// Принимает: files_patterns_list - список файлов, готовых к переименованию
//            files_list - список файлов в текущей директории
//
// Очищает память, использовавшуюся для выполнения
// функции rename_and_get_renamed_list
void clean_memory(GList* files_patterns_list, GList* files_list);

// Принимает: sample - список с шаблонами
//            dir_path -строка, содержащая путь к файлу
//            opt - указатель на структуру Option
//
// Запускает несколько функций,
// которые сначала получают список файлов в директории,
// а после список файлов готовых к переименованию,
// и переименовывает их
//
// Возвращает: список переименнованых файлов
GList* rename_and_get_renamed_list(GList* sample, char* dir_path, Option* opt);
