#pragma once

#include <dirent.h>
#include <glib.h>

enum Attributes {
    DIRS = DT_DIR, // Директории
    FILES = DT_REG // Файлы
};

typedef struct {
    char* filename; // Имя файла
    char* pattern;  // Шаблон
} File_to_rename;

// Принимает: path - строка с путём к директории
//            attr - атрибут(Директория или файл)

// Возвращает: Cписок файлов или директорий,
//             в зависимости от атрибута
GList* get_files_or_dirs_list(char* path, int attr);

// Принимает: filename - строка с именем файла
//            pattern - строка с шаблоном
//
// Проверяет соответвует ли файл шаблону
// и возвращает true или false
//
// Возвращает: true - если файл соответсвует шаблону
//             false - если файл не соответвует шаблону
int is_file_match_pattern(char* filename, char* pattern);

// Принимает: ready_to_rename_file - указатель на структуру File_to_rename
//            filename_data - Данные, содержащие имя файла
//            pattern_data - Данные, содержащие шаблон
//
// Вносит имя файла и шаблон в нужные поля структуры
void list_data(
        File_to_rename* ready_to_rename_file,
        void* filename_data,
        void* pattern_data);

// Принимает: filesname - список с именами файлов
//            patterns - список с шаблонами
//
// Функция проходит по каждому имени из полученного списка с именами
// и по каждому шаблону, проверяет подходит ли имя под шаблон
// если да, то добавляет имя и шаблон в список с результатом
//
// Возвращает: Список файлов, готовых к переименованию,
// и их шаблонов
GList* get_files_patterns_list(GList* filesname, GList* patterns);

// Функция для работы функции сортировки
gint my_comparator(gconstpointer item1, gconstpointer item2);

// Принимает: file_type - строка, содержащая тип текущего файла
//            file_name - строка, содержащая имя текущего файла
//            path - строка, содержащая путь к текущей директории
//            name - указатель на динамически выделенную память под имя файла
//
// Проверяет тип текущего файла, если он директория, записывает имя,
// а если файл, записывает путь
//
// Возвращает: Строку, содержащую имя директории или путь к файлу
char* copy_file_name_or_path(
        unsigned char file_type, char* file_name, char* path, char* name);

// Принимает: files - список с файлами или путями к файлам
//
// Получает список имён файлов из их путей
//
// Возвращает: список с именами файлов
GList* get_files_names(GList* files);