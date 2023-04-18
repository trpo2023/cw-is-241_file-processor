#include <glib.h>

#include <libfileproc/lexer.h>

char* skip_space(char* string) // пропуск пробелов
{
    while (*string == ' ') {
        string++;
    }
    return string;
}

char* skip_to_colon(char* string) // Пропуск всех символов до ':' или '\0'
{
    while (1) {
        if (*string == ':')
            return string;
        if (*string == '\0')
            return string;
        string++;
    }
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

int check_sample_string(char* string) // проверка полной строки с шаблонами
{
    char* test_string = string;
    int search_sample = check_search_sample(&test_string);
    if (search_sample)
        return search_sample;
    int rename_sample = check_rename_sample(&test_string);
    if (rename_sample)
        return rename_sample;
    return success;
}

// Взятие отдельного паттерна из всей строки
char* get_pattern(char* sample, char** pattern)
{
    char* buffer = *pattern;
    while (1) {
        if (*sample == ' ')
            break;
        if (*sample == ':')
            break;
        if (*sample == '\0')
            break;
        *buffer = *sample;
        buffer++;
        sample++;
    }
    *buffer = '\0';
    return sample;
}

// Сдвинуть указатель к началу следующего паттерна
char* to_rename_pattern(char* sample)
{
    while (1) {
        if (*sample == ' ')
            sample++;
        else if (*sample == ':')
            sample++;
        else
            return sample;
    }
}

// Разбить строку на паттерны
sample_parts split_sample(char* sample, sample_parts* patterns)
{
    sample = skip_space(sample);
    sample = get_pattern(sample, &(patterns->search_pattern));
    sample = to_rename_pattern(sample);
    get_pattern(sample, &(patterns->rename_pattern));
    return *patterns;
}

// Разделить строку и записать в структуру
int get_sample(char* sample, sample_parts* patterns)
{
    int check = check_sample_string(sample);
    if (check)
        return check;
    patterns->search_pattern = malloc(256);
    patterns->rename_pattern = malloc(256);
    *patterns = split_sample(sample, patterns);
    return success;
}

GList* add_sample(GList* sample_list, char* sample, int* exit_code)
{
    sample_parts* pattern = malloc(sizeof(sample_parts));
    *exit_code = get_sample(sample, pattern);
    if (*exit_code == 0) {
        sample_list = g_list_append(sample_list, pattern);
    }

    return sample_list;
}