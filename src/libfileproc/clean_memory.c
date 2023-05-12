#include <libfileproc/clean_memory.h>
#include <libfileproc/lexer.h>

void clean_data(Option* opt, GList** input_strings, GList** patterns)
{
    opt->name_register = R_DEFAULT;
    if (*input_strings != NULL) {
        g_list_free_full(*input_strings, free);
        *input_strings = NULL;
    }
    if (*patterns != NULL) {
        g_list_free_full(*patterns, free_SplittedPattern);
        *patterns = NULL;
    }
}

void free_SplittedPattern(void* patterns)
{
    free(((SplittedPattern*)patterns)->search_pattern);
    free(((SplittedPattern*)patterns)->rename_pattern);
    free((SplittedPattern*)patterns);
}

void clean_memory(GList* files_patterns_list, GList* files_list)
{
    g_list_free_full(files_patterns_list, free);
    g_list_free_full(files_list, free);
}

void free_renamed_files(void* data)
{
    free(((RenamedFile*)data)->old_path);
    free(((RenamedFile*)data)->new_path);
    free(data);
}

void free_renamed_list(GList* renamed_file_list, GList* list)
{
    g_list_free_full(list, free);
    g_list_free_full(renamed_file_list, free_renamed_files);
}