#include <glib.h>

#include <libfileproc/lexer.h>

char* skip_space(char* string) // пропуск пробелов
{
    while (*string == ' ')
        string++;
    return string;
}

char* skip_to_colon(char* string) // Пропуск всех символов до ':' или '\0'
{
    while (*string != ':' && *string != '\0')
        string++;
    return string;
}

int check_search_sample(char** string) // проверка шаблона для поиска
{
    bool star = false;
    bool colon = false;
    bool symbols = false;
    int sample_len = 0;

    char* test_string = skip_space(*string);

    while (1) {
        if (sample_len > MAX_LEN)
            return max_len_error;

        if (*test_string == '/') {
            return wrong_symbol_error;
        }

        if (*test_string == '\0')
            break;

        if (*test_string == '.')
            star = false;

        if (*test_string == ':') {
            test_string++;
            colon = true;
            break;
        }

        if (*test_string == ' ') {
            test_string = skip_space(test_string);
            if (*test_string == ':') {
                test_string++;
                colon = true;
                break;
            }
            test_string = skip_to_colon(test_string);
            if (*test_string == ':') {
                return search_sample_error;
            }
            break;
        }

        symbols = true;
        if (*test_string == '*') {
            if (star == true)
                return more_than_one_star_error;
            if (*(test_string + 1) == '?')
                return star_quest_error;
            star = true;
        }
        if (*test_string == '?') {
            if (*(test_string + 1) == '*')
                return star_quest_error;
        }
        test_string++;
        sample_len++;
    }
    if (!symbols)
        return empty_search_sample_error;
    if (!colon)
        return colon_error;

    *string = test_string;
    return success;
}

int check_rename_sample(char** string) // проверка шаблона для переименовывания
{
    bool star = false;
    bool symbols = false;
    int sample_len = 0;

    char* test_string = skip_space(*string);

    while (1) {
        if (sample_len > MAX_LEN)
            return max_len_error;

        if (*test_string == '/') {
            return wrong_symbol_error;
        }

        if (*test_string == '\0')
            break;

        if (*test_string == '.')
            star = false;

        if (*test_string == ':') {
            return colon_in_sample_error;
        }

        if (*test_string == ' ') {
            test_string = skip_space(test_string);
            if (*test_string == '\0') {
                *string = test_string;
                return success;
            }
            if (*test_string == ':')
                return colon_in_sample_error;
            return rename_sample_error;
        }

        symbols = true;
        if (*test_string == '*') {
            if (star == true)
                return more_than_one_star_error;
            if (*(test_string + 1) == '?')
                return star_quest_error;
            star = true;
        }
        if (*test_string == '?') {
            if (*(test_string + 1) == '*')
                return star_quest_error;
        }
        test_string++;
        sample_len++;
    }
    if (!symbols)
        return empty_rename_sample_error;

    *string = test_string;
    return success;
}

int check_sample_string(char* input_string)
{
    int search_sample = check_search_sample(&input_string);
    if (search_sample)
        return search_sample;
    int rename_sample = check_rename_sample(&input_string);
    if (rename_sample)
        return rename_sample;
    return success;
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
    int check = check_sample_string(input_string);
    if (check)
        return check;
    patterns->search_pattern = malloc(sizeof(char) * MAX_LEN);
    patterns->rename_pattern = malloc(sizeof(char) * MAX_LEN);
    split_sample(input_string, patterns);
    return success;
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
