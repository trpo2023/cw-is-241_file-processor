#pragma once
#include <glib.h>
#include <stdbool.h>

GList* get_files_or_dirs_list(char* path, int attr);
int is_file_match_pattern(char* name, char* pattern);