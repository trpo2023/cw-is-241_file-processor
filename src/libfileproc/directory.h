#pragma once
#include <glib.h>
#include <stdbool.h>

enum attributes {
    ndirs, // Директории = 0
    nfiles // Файлы = 1
};

typedef struct list_part {
    char* filename;
    char* pattern;
} list_part;

GList* get_files_or_dirs_list(char* path, int attr);
int is_file_match_pattern(char* filename, char* pattern);
void list_data(list_part* p, void* filename_data, void* pattern_data);
GList* get_files_patterns_list(GList* filesname, GList* patterns);