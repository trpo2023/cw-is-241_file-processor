#include <interface/get_pattern.h>
#include <interface/init.h>
#include <interface/print.h>
#include <libfileproc/lexer.h>

GList* pattern_input(WINDOW* menu, GList** input_strings, GList* patterns)
{
    int y, x;
    WINDOW* sub = open_pattern_window(menu, &x, &y);
    int max_ninstr = y - 3;

    int cnt = print_input_strings(sub, *input_strings);

    print_counter(sub, x, cnt, max_ninstr);
    if (check_max_ninstr(sub, cnt, max_ninstr))
        return patterns;
    char* str = NULL;

    while (cnt < max_ninstr)
        if (get_string(sub, &cnt, max_ninstr, x, str, &patterns, input_strings))
            break;

    close_pattern_window(sub);

    return patterns;
}

int check_max_ninstr(WINDOW* sub, int cnt, int max_ninstr)
{
    if (cnt == max_ninstr) {
        mvwprintw(sub, 1, 1, "Введено максимальное количество шаблонов!");
        curs_set(0);
        noecho();
        wgetch(sub);

        remove_current_window(sub);
        return -1;
    }
    return 0;
}

int get_string(
        WINDOW* sub,
        int* cnt,
        int max_ninstr,
        int x,
        char* str,
        GList** patterns,
        GList** input_strings)
{
    str = malloc(x - 2);
    mvwgetnstr(sub, *cnt + 2, 2, str, x - 3);
    if (str[0] == '\0') {
        free(str);
        return -1;
    }
    int exit_code;
    *patterns = add_patterns(*patterns, str, &exit_code);
    if (exit_code == 0) {
        *input_strings = g_list_append(*input_strings, str);
    } else {
        mvwprintw(sub, *cnt + 2, 1, "╳");
        free(str);
    }
    print_counter(sub, x, ++(*cnt), max_ninstr);
    return 0;
}