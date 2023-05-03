#include <glib.h>

#include <libfileproc/lexer.h>

char* skip_space(char* string)
{
    while (*string == ' ')
        string++;
    return string;
}

int check_wrong_symbols(char* input_string)
{
    if (strpbrk(input_string, "/<>|«") != NULL)
        return -1;
    return 0;
}

int check_colon(char* input_string)
{
    if (*input_string == ':')
        return -1;
    input_string = strpbrk(input_string, ":");
    if (input_string == NULL)
        return -1;
    input_string = strpbrk(input_string + 1, ":");
    if (input_string != NULL)
        return -1;
    return 0;
}

int check_star(char** pattern)
{
    char* test_pattern = *pattern;
    if (test_pattern[0] == '*') {
        test_pattern++;
        if (test_pattern[0] == '?')
            return -1;
        char* star = strpbrk(test_pattern, "*");
        char* dot = strpbrk(test_pattern, ".");
        if (star != NULL && dot != NULL)
            if (star - dot < 0)
                return -1;
    }
    *pattern = test_pattern;
    return 0;
}

int check_quest(char* pattern)
{
    if (pattern[0] == '?')
        if (pattern[1] == '*')
            return -1;
    return 0;
}

int check_space(char* pattern)
{
    if (pattern[0] == ' ') {
        pattern = skip_space(pattern);
        if (pattern[0] != '\0')
            return -1;
    }
    return 0;
}

int get_tokens(char** tokens, char* str, char* delim)
{
    tokens[0] = strtok(str, delim);
    tokens[1] = strtok(NULL, delim);
    if (tokens[1] == NULL)
        return -1;
    return 0;
}

int check_pattern(char* pattern)
{
    pattern = skip_space(pattern);
    if (*pattern == '\0')
        return -1;
    while (pattern[0] != '\0') {
        if (check_star(&pattern))
            return -1;
        if (check_quest(pattern))
            return -1;
        if (check_space(pattern))
            return -1;
        pattern++;
    }
    return 0;
}

int check_input_string(char* input_string)
{
    if (check_wrong_symbols(input_string))
        return -1;
    if (check_colon(input_string))
        return -1;
    char* patterns[2];
    char copy_string[MAX_LEN * 2];
    strcpy(copy_string, input_string);
    if (get_tokens(patterns, copy_string, ":"))
        return -1;
    if (check_pattern(patterns[0]))
        return -1;
    if (check_pattern(patterns[1]))
        return -1;
    return 0;
}

char* get_pattern(char* inp_str, char* pattern)
{
    while (*inp_str != ' ' && *inp_str != ':' && *inp_str != '\0') {
        *pattern = *inp_str;
        pattern++;
        inp_str++;
    }
    *pattern = '\0';
    return inp_str;
}

char* to_rename_pattern(char* inp_str)
{
    while (*inp_str == ' ' || *inp_str == ':')
        inp_str++;
    return inp_str;
}

void split_input_string(char* input_string, Splitted_patterns* patterns)
{
    input_string = skip_space(input_string);
    input_string = get_pattern(input_string, patterns->search_pattern);
    input_string = to_rename_pattern(input_string);
    get_pattern(input_string, patterns->rename_pattern);
}

int get_patterns(char* input_string, Splitted_patterns* patterns)
{
    if (check_input_string(input_string))
        return -1;
    patterns->search_pattern = malloc(sizeof(char) * MAX_LEN);
    patterns->rename_pattern = malloc(sizeof(char) * MAX_LEN);
    split_input_string(input_string, patterns);
    return 0;
}

GList* add_patterns(GList* patterns, char* input_string, int* exit_code)
{
    Splitted_patterns* pattern = malloc(sizeof(Splitted_patterns));
    *exit_code = get_patterns(input_string, pattern);
    if (*exit_code == 0)
        patterns = g_list_append(patterns, pattern);
    else
        free(pattern);
    return patterns;
}

void free_Splitted_patterns(void* patterns)
{
    free(((Splitted_patterns*)patterns)->search_pattern);
    free(((Splitted_patterns*)patterns)->rename_pattern);
    free((Splitted_patterns*)patterns);
}
