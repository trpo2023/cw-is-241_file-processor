#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <libfileproc/directory.h>

GSList* read_dir(char* path)
{
    GSList* dir_list = NULL;
    struct dirent* file;

    DIR* dir = opendir(path);
    if (!dir) {
        return NULL;
    }

    while ((file = readdir(dir)) != NULL) {
        if ((file->d_type & DT_DIR) == DT_DIR)
            dir_list = g_slist_append(dir_list, file->d_name);
    }

    free(file);
    closedir(dir);
    return dir_list;
}
