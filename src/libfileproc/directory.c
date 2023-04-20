#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <libfileproc/directory.h>

gint my_comparator(gconstpointer item1, gconstpointer item2)
{
    return g_ascii_strcasecmp(item1, item2);
}

GList* get_files_or_dirs_list(char* path, int attr)
{
    GList* list = NULL;
    struct dirent* file;
    DIR* dir = opendir(path);
    if (!dir) {
        return NULL;
    }

    while ((file = readdir(dir)) != NULL) {
        if ((file->d_type & attr) == attr)
            list = g_list_append(list, file->d_name);
    }

    closedir(dir);
    return list;
}

int is_file_match_pattern(char* filename, char* pattern)
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

void list_data(File_to_rename* p, void* filename_data, void* pattern_data)
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
                File_to_rename* p = malloc(sizeof(File_to_rename));
                list_data(p, filename->data, pattern->data);
                files_and_patterns_list
                        = g_list_append(files_and_patterns_list, p);
            }
        }
    }
    return files_and_patterns_list;
}