#include <libfileproc/clean_memory.h>
#include <libfileproc/directory.h>
#include <libfileproc/rename.h>

GList* rename_and_get_renamed_list(GList* sample, char* dir_path, Option* opt)
{
    GList* renamed_files_list = NULL;
    GList* files_patterns_list = NULL;

    GList* files_list = get_files_or_dirs_list(dir_path, FILES);
    files_patterns_list = get_files_patterns_list(files_list, sample);
    renamed_files_list = rename_files(files_patterns_list, opt);

    clean_memory(files_patterns_list, files_list);

    return renamed_files_list;
}