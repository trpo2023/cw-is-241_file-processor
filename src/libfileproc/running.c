#include <libfileproc/directory.h>

#include <libfileproc/lexer.h>
#include <libfileproc/rename.h>

void clean_memory(GList* files_patterns_list, GList* files_list)
{
    g_list_free_full(files_patterns_list, free);
    g_list_free_full(files_list, free);
}

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