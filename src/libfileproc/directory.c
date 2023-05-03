#include <libfileproc/directory.h>

#include <libfileproc/lexer.h>
#include <libfileproc/rename.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

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
        char* name = malloc(sizeof(char) * MAX_LEN);
        name = copy_file_name_or_path(file->d_type, file->d_name, path, name);
        if ((file->d_type & attr) == attr) {
            list = g_list_append(list, name);
        } else {
            free(name);
        }
    }
    closedir(dir);
    return list;
}

char* copy_file_name_or_path(
        unsigned char file_type, char* file_name, char* path, char* name)
{
    if ((file_type & DT_REG) == DT_REG && strcmp(".", path) != 0) {
        strcpy(name, path);
        strcat(name, "/");
        strcat(name, file_name);
    } else {
        strcpy(name, file_name);
    }
    return name;
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

void list_data(
        File_to_rename* ready_to_rename_file,
        void* filename_data,
        void* pattern_data)
{
    char* name = malloc(sizeof(char) * MAX_LEN);
    strcpy(name, filename_data);
    ready_to_rename_file->filename = name;
    ready_to_rename_file->pattern = pattern_data;
}

GList* get_files_patterns_list(GList* filesname, GList* samples)
{
    GList* files_and_patterns_list = NULL;
    for (GList* filename = filesname; filename != NULL;
         filename = filename->next) {
        char* name = get_name(filename->data);
        if (name != filename->data)
            name++;
        for (GList* sample = samples; sample != NULL; sample = sample->next) {
            Splitted_patterns* current_pattern_names = sample->data;
            if (is_file_match_pattern(
                        name, current_pattern_names->search_pattern)) {
                File_to_rename* ready_to_rename_file
                        = malloc(sizeof(File_to_rename));
                list_data(
                        ready_to_rename_file,
                        filename->data,
                        current_pattern_names->rename_pattern);
                files_and_patterns_list = g_list_append(
                        files_and_patterns_list, ready_to_rename_file);
                break;
            }
        }
    }
    return files_and_patterns_list;
}