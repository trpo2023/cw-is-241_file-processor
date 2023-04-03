#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "directory.h"
#include "list.h"

List read_dir(char* path)
{
    List dir_list;
    init_list(&dir_list);
    struct dirent* file;

    DIR* dir = opendir(path);
    if (!dir) {
        return NULL;
    }

    while ((file = readdir(dir)) != NULL) {
        if ((file->d_type & DT_DIR) == DT_DIR)
            list_add(&dir_list, file->d_name);
    }

    free(dir);
    free(file);
    return dir_list;
}
