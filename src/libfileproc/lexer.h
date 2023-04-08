#pragma once

#define MAX_LEN 256

enum exit_codes {
    success,                  // 0 успех
    more_than_one_star_error, // 1 больше одной звезды в начале, между точками,
                              // или в конце
    star_quest_error,         // 2 звезда рядом с вопросом
    max_len_error, // 3 длина шаблона больше, чем допустимая
    empty_search_sample_error, // 4 шаблон для поиска отсутствует
    empty_rename_sample_error, // 5 шаблон для переименовывания отсутствует
    colon_error, // 6 отсутствует разделительное двоеточие
    colon_in_sample_error, // 7 повторное использование двоеточия после
                           // разделительного
    search_sample_error, // 8 какая-либо иная ошибка в шаблоне для поиска
    rename_sample_error, // 9 какая-либо иная ошибка в шаблоне для
                         // переименовывания
    wrong_symbol_error // Символ '/' недопустим в имени файла в системе linux
};

char* skip_space(char* string); // пропуск пробелов
char* skip_to_colon(char* string); // Пропуск всех символов до ':' или '\0'
int check_search_sample(char** string); // проверка шаблона для поиска
int check_rename_sample(char** string); // проверка шаблона для переименовывания
int check_sample_string(char* string); // проверка полной строки с шаблонами