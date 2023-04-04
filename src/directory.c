#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "directory.h"

GPtrArray* read_dir(char* path)
{
    GPtrArray* dir_list = g_ptr_array_new();
    struct dirent* file;

    DIR* dir = opendir(path);
    if (!dir) {
        return NULL;
    }

    while ((file = readdir(dir)) != NULL) {
        if ((file->d_type & DT_DIR) == DT_DIR)
            g_ptr_array_add(dir_list, file->d_name);
    }

    free(file);
    closedir(dir);
    return dir_list;
}
