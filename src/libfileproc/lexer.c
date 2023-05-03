#include <glib.h>
#include <string.h>

#include <libfileproc/lexer.h>

char* skip_space(char* string) // пропуск пробелов
{
    while (*string == ' ')
        string++;
    return string;
}

int check_wrong_symbols(char* input_string)
{
    if (strpbrk(input_string, "/") != NULL)
        return 1;
    return 0;
}

int check_colon(char* input_string)
{
    if (*input_string == ':')
        return 1;
    input_string = strpbrk(input_string, ":");
    if (input_string == NULL)
        return 1;
    input_string = strpbrk(input_string + 1, ":");
    if (input_string != NULL)
        return 1;
    return 0;
}

int check_star(char** pattern)
{
    char* test_pattern = *pattern;
    if (test_pattern[0] == '*') {
        test_pattern++;
        if (test_pattern[0] == '?')
            return 1;
        char* star = strpbrk(test_pattern, "*");
        char* dot = strpbrk(test_pattern, ".");
        if (star != NULL && dot != NULL)
            if (star - dot < 0)
                return 1;
    }
    *pattern = test_pattern;
    return 0;
}

int check_quest(char* pattern)
{
    if (pattern[0] == '?')
        if (pattern[1] == '*')
            return 1;
    return 0;
}

int check_space(char* pattern)
{
    if (pattern[0] == ' ') {
        pattern = skip_space(pattern);
        if (pattern[0] != '\0')
            return 1;
    }
    return 0;
}

int get_tokens(char** tokens, char* str, char* delim)
{
    tokens[0] = strtok(str, delim);
    tokens[1] = strtok(NULL, delim);
    if (tokens[1] == NULL)
        return 1;
    return 0;
}

int check_pattern(char* pattern)
{
    pattern = skip_space(pattern);
    if (*pattern == '\0')
        return 1;
    while (pattern[0] != '\0') {
        if (check_star(&pattern))
            return 1;
        if (check_quest(pattern))
            return 1;
        if (check_space(pattern))
            return 1;
        pattern++;
    }
    return 0;
}

int check_sample_string(char* input_string)
{
    if (check_wrong_symbols(input_string))
        return 1;
    if (check_colon(input_string))
        return 1;
    char* patterns[2];
    char copy_string[MAX_LEN * 2];
    strcpy(copy_string, input_string);
    if (get_tokens(patterns, copy_string, ":"))
        return 1;
    if (check_pattern(patterns[0]))
        return 1;
    if (check_pattern(patterns[1]))
        return 1;
    return 0;
}

// Взятие отдельного паттерна из всей строки
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

// Сдвинуть указатель к началу следующего паттерна
char* to_rename_pattern(char* inp_str)
{
    while (*inp_str == ' ' || *inp_str == ':')
        inp_str++;

    return inp_str;
}

// Разбить строку на паттерны
void split_sample(char* input_string, sample_parts* patterns)
{
    input_string = skip_space(input_string);
    input_string = get_pattern(input_string, patterns->search_pattern);
    input_string = to_rename_pattern(input_string);
    get_pattern(input_string, patterns->rename_pattern);
}

// Разделить строку и записать в структуру
int get_sample(char* input_string, sample_parts* patterns)
{
    if (check_sample_string(input_string))
        return 1;
    patterns->search_pattern = malloc(sizeof(char) * MAX_LEN);
    patterns->rename_pattern = malloc(sizeof(char) * MAX_LEN);
    split_sample(input_string, patterns);
    return 0;
}

GList* add_sample(GList* patterns, char* input_string, int* exit_code)
{
    sample_parts* pattern = malloc(sizeof(sample_parts));
    *exit_code = get_sample(input_string, pattern);
    if (*exit_code == 0) {
        patterns = g_list_append(patterns, pattern);
    }

    return patterns;
}

void free_sample_parts(void* patterns)
{
    free(((sample_parts*)patterns)->search_pattern);
    free(((sample_parts*)patterns)->rename_pattern);
    free((sample_parts*)patterns);
}
