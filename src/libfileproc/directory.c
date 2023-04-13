#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <libfileproc/directory.h>

GList* get_files_or_dirs_list(
        char* path, int attr) // получает список файлов или директорий, в
                              // зависимости от атрибута
{
    GList* dir_list = NULL;
    GList* file_list = NULL;
    struct dirent* file;
    DIR* dir = opendir(path);
    if (!dir) {
        return NULL;
    }

    while ((file = readdir(dir)) != NULL) {
        if (attr == 0) {
            if ((file->d_type & DT_DIR) == DT_DIR) // проверка на директорию
                dir_list = g_list_append(dir_list, file->d_name);
        } else {
            if ((file->d_type & DT_REG) == DT_REG) // проверка на файл
                file_list = g_list_append(file_list, file->d_name);
        }
    }

    closedir(dir);
    if (attr == 0)
        return dir_list;
    else
        return file_list;
}

int is_file_match_pattern(
        char* filename, char* pattern) // Проверка на соответствие файла шаблону
{
    char* name_copy = NULL;
    char* pattern_copy = NULL;
    while (*name != '\0') {
        if (*pattern == '*') {
            name_copy = name;
            pattern_copy = ++pattern;
        } else if (*pattern == '?' || *pattern == *name) {
            name++;
            pattern++;
        } else {
            if (pattern_copy == NULL)
                return false;
            name = ++name_copy;
            pattern = pattern_copy;
        }
    }
    while (*pattern == '*')
        pattern++;
    return *pattern == '\0' ? true : false;
}
