#pragma once
#include <glib.h>
#include <stdbool.h>

enum attributes {
    ndirs, // Директории = 0
    nfiles // Файлы = 1
};

GList* get_files_or_dirs_list(char* path, int attr);
int is_file_match_pattern(char* filename, char* pattern);