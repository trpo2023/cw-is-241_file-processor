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
    char* filename_copy = NULL;
    char* pattern_copy = NULL;
    while (*filename != '\0') {
        if (*pattern == '*') {
            filename_copy = filename;
            pattern_copy = ++pattern;
        } else if (*pattern == '?' || *pattern == *filename) {
            filename++;
            pattern++;
        } else {
            if (pattern_copy == NULL)
                return false;
            filename = ++filename_copy;
            pattern = pattern_copy;
        }
    }
    while (*pattern == '*')
        pattern++;
    return *pattern == '\0' ? true : false;
}

void list_data(list_part* p, void* filename_data, void* pattern_data)
{
    p->filename = filename_data;
    p->pattern = pattern_data;
}

GList* get_files_patterns_list(GList* filesname, GList* patterns)
{
    GList* files_and_patterns_list = NULL;
    for (GList* filename = filesname; filename != NULL;
         filename = filename->next) {
        for (GList* pattern = patterns; pattern != NULL;
             pattern = pattern->next) {
            if (is_file_match_pattern(filename->data, pattern->data)) {
                list_part* p = malloc(sizeof(list_part));
                list_data(p, filename->data, pattern->data);
                files_and_patterns_list
                        = g_list_append(files_and_patterns_list, p);
            }
        }
    }
    return files_and_patterns_list;
}