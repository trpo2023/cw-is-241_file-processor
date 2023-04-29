#pragma once
#include <dirent.h>
#include <glib.h>
#include <stdbool.h>

enum Attributes {
    DIRS = DT_DIR, // Директории
    FILES = DT_REG // Файлы
};

typedef struct {
    char* filename;
    char* pattern;
} File_to_rename;

// Получает список файлов или директорий, в зависимости от атрибута
GList* get_files_or_dirs_list(char* path, int attr);
// Проверка на соответствие файла шаблону
int is_file_match_pattern(char* filename, char* pattern);
// Вносит данные в структуру File_to_rename
void list_data(File_to_rename* p, void* filename_data, void* pattern_data);
// Получает список файлов, готовых к переименованию,  и их шаблонов
GList* get_files_patterns_list(GList* filesname, GList* patterns);
// Функция для работы функции сортировки
gint my_comparator(gconstpointer item1, gconstpointer item2);