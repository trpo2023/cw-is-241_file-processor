#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <libfileproc/directory.h>

GList* get_files_or_dirs_list(char* path, int attr)
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
            if ((file->d_type & DT_DIR) == DT_DIR)
                dir_list = g_list_append(dir_list, file->d_name);
        } else {
            if ((file->d_type & DT_REG) == DT_REG)
                file_list = g_list_append(file_list, file->d_name);
        }
    }

    closedir(dir);
    if (attr == 0)
        return dir_list;
    else
        return file_list;
}
