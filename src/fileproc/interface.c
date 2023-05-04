#include <glib.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"
#include <libfileproc/directory.h>
#include <libfileproc/lexer.h>
#include <libfileproc/rename.h>
#include <libfileproc/running.h>

const char* menu_items[] = {
        "1) Ввести шаблоны",
        "2) Выбрать каталог",
        "3) Запустить массовое переименование",
        "4) Опции",
        "5) Информация",
        "6) Выход из приложения (F10)",
};

const char* options[] = {
        "Регистр", "Восстановить значения по умолчанию", "<-Вернуться в меню"};

void clean_data(Option* opt, GList** input_strings, GList** patterns);
void print_counter(WINDOW* sub, int x, int cnt, int max_cnt);

void check_term_size(int x, int y)
{
    if (x < 120 || y < 30) {
        delwin(stdscr);
        endwin();
        printf("\e[1;31mОшибка\e[0m: текущий размер терминала %dx%d\n", x, y);
        printf("Минимальный размер: 120x30 (x, y)\n");
        exit(EXIT_FAILURE);
    }
}

void mvwprintw_highlite(WINDOW* win, int y, int x, const char* str)
{
    wattron(win, A_STANDOUT);
    mvwprintw(win, y, x, "%s", str);
    wattroff(win, A_STANDOUT);
}

WINDOW* init_menu()
{
    initscr();
    int y, x;
    getmaxyx(stdscr, y, x);
    check_term_size(x, y);

    char* top_str = "File processor";
    mvprintw(0, (x - strlen(top_str)) / 2, "%s", top_str);
    refresh();

    WINDOW* win = newwin(y - 1, x, 1, 0);
    box(win, 0, 0);
    getmaxyx(win, y, x);

    mvwprintw_highlite(win, 1, 2, menu_items[0]);

    for (int i = 1; i < 6; i++) {
        mvwprintw(win, i + 1, 2, "%s", menu_items[i]);
    }

    mvwprintw(win, y - 2, 2, "%s", "Выбранный каталог: .");
    mvwprintw(win, y - 3, 2, "%s", menu_items[0]);

    noecho();
    curs_set(0);
    keypad(win, true);
    wrefresh(win);
    return win;
}

int select_items(WINDOW* win, const char* items[], int i, int offset, int max)
{
    int y, x;
    getmaxyx(win, y, x);
    int ch;
    while ((ch = wgetch(win)) != KEY_F(10)) {
        mvwprintw(win, i + offset, 2, "%s", items[i]);
        switch (ch) {
        case KEY_UP:
            i--;
            i = (i < 0) ? max : i;
            break;
        case KEY_DOWN:
            i++;
            i = (i > max) ? 0 : i;
            break;
        case 10: // KEY_ENTER
            return i;
        }
        mvwprintw_highlite(win, i + offset, 2, items[i]);
        if (max == 4) // menu options
        {
            mvwprintw(win, y - 3, 2, "%-*s", x / 2, items[i]);
        }
    }

    return KEY_F(10);
}

void remove_current_window(WINDOW* sub)
{
    wclear(sub);
    wrefresh(sub);
    delwin(sub);
}

WINDOW* init_sub_window(WINDOW* menu, int max_y, int max_x)
{
    WINDOW* sub = subwin(menu, max_y - 2, max_x / 2 - 1, 2, max_x / 2);
    wclear(sub);
    box(sub, 0, 0);
    keypad(sub, true);
    wrefresh(sub);

    return sub;
}

void print_opt(WINDOW* sub, Option* opt, int i)
{
    switch (i) {
    case 0: // REGISTER
        switch (opt->name_register) {
        case R_DEFAULT:
            mvwprintw(sub, i + 3, 15, "%-20s", "Стандартный");
            break;
        case R_LOW:
            mvwprintw(sub, i + 3, 15, "%-20s", "Нижний");
            break;
        case R_HIGH:
            mvwprintw(sub, i + 3, 15, "%-20s", "Верхний");
            break;
        }
    }
}

void set_default_settings(
        WINDOW* menu, Option* opt, GList** inp_s, GList** samp, char* curr_dir)
{
    int y = getmaxy(menu);
    strcpy(curr_dir, ".");
    mvwprintw(menu, y - 2, 2, "Выбранный каталог: %-30s", curr_dir);
    clean_data(opt, inp_s, samp);
}

void select_option(
        WINDOW* menu, Option* opt, GList** inp_s, GList** samp, char* curr_dir)
{
    int options_cnt = 2;
    int y, x, i;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    mvwprintw(sub, 1, 1, "Выберите опцию:");

    mvwprintw_highlite(sub, 3, 2, options[0]);
    print_opt(sub, opt, 0);

    for (i = 1; i <= options_cnt; i++) {
        mvwprintw(sub, i + 3, 2, "%s", options[i]);
    }
    i = 0;
    while ((i = select_items(sub, options, i, 3, 2)) != BACK
           && i != KEY_F(10)) {
        if (i == REGISTER) {
            opt->name_register++;
            opt->name_register %= 3;
            print_opt(sub, opt, i);
            wrefresh(sub);
        } else {
            set_default_settings(menu, opt, inp_s, samp, curr_dir);
            break;
        }
        mvwprintw_highlite(sub, 3, 2, options[i]);
    }

    remove_current_window(sub);
}

void free_renamed_files(void* data)
{
    free(((RenamedFile*)data)->old_path);
    free(((RenamedFile*)data)->new_path);
    free(data);
}

// a -> y offset
// b -> x offset
void print_items(WINDOW* sub, GList* list, int* str_cnt, int a, int b)
{
    int y = getmaxy(sub) - a;
    int x = getmaxx(sub) - a;
    int cnt = 0;
    for (GList* i = list; i != NULL && cnt < y; i = i->next) {
        mvwprintw(sub, cnt + b, 2, "%-*s", x, (char*)i->data);
        cnt++;
    }
    *str_cnt = cnt;

    while (cnt < y) {
        mvwprintw(sub, cnt + b, 2, "%-*s", x + 1, " ");
        cnt++;
    }

    wrefresh(sub);
}

// p -> current page
// max_p -> max_page
// a -> y offset
// b -> x offset
void print_new_page(
        WINDOW* sub, GList* list, int* cnt, int p, int max_p, int a, int b)
{
    int x = getmaxx(sub);
    print_items(sub, list, cnt, a, b);
    print_counter(sub, x, p + 1, max_p + 1);
}

// a -> y offset
// b -> x offset
char* get_item(
        WINDOW* sub,
        GList* dir_list,
        int y,
        size_t len,
        int str_cnt,
        int a,
        int b)
{
    int ch, i = 0, page = 0, x = getmaxx(sub);
    size_t max_pages = len / y;
    GList* pages[max_pages];

    // page counter
    print_counter(sub, x, page + 1, max_pages + 1);

    while ((ch = wgetch(sub)) != 10) { // 10 - KEY_ENTER
        mvwprintw(sub, i + b, 2, "%s", (char*)dir_list->data);
        if (i == 0)
            pages[page] = dir_list;

        switch (ch) {
        case KEY_UP:
            i--;
            if (i < 0 && page >= 1) {
                page--;
                print_new_page(
                        sub, pages[page], &str_cnt, page, max_pages, a, b);
                i = y - 1;
            } else if (i < 0)
                i++;
            else
                dir_list = dir_list->prev;
            break;
        case KEY_DOWN:
            i++;
            if (i >= str_cnt && page == max_pages)
                i--;
            else if (i >= y) {
                page++;
                print_new_page(sub, dir_list, &str_cnt, page, max_pages, a, b);
                i = 0;
            } else
                dir_list = dir_list->next;
        }
        mvwprintw_highlite(sub, i + b, 2, (char*)dir_list->data);
        wrefresh(sub);
    }
    return (char*)dir_list->data;
}

char* select_dir(WINDOW* menu, char* current_dir)
{
    int y, x;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    mvwprintw(sub, 1, 1, "Выберите каталог:");
    char* dir = NULL;

    GList* dir_list = get_files_or_dirs_list(".", DIRS);
    dir_list = g_list_sort(dir_list, my_comparator);
    size_t dir_len = g_list_length(dir_list);

    int dir_cnt = 0; // count of directories on one page
    print_items(sub, dir_list, &dir_cnt, 3, 2);
    mvwprintw_highlite(sub, 2, 2, (char*)dir_list->data);

    dir = get_item(sub, dir_list, y - 5, dir_len, dir_cnt, 3, 2);
    strcpy(current_dir, dir);

    remove_current_window(sub);
    g_list_free_full(dir_list, free);

    return current_dir;
}

GList* get_renamed_list(WINDOW* sub, GList* renamed_files)
{
    int x = getmaxx(sub);

    GList* str_list = NULL;
    for (GList* i = renamed_files; i != NULL; i = i->next) {
        char* old_name = ((RenamedFile*)i->data)->old_path;
        char* new_name = ((RenamedFile*)i->data)->new_path;
        char* str = write_correct_renamed_string(x, old_name, new_name);
        str_list = g_list_append(str_list, str);
    }

    return str_list;
}

void print_renamed_list(WINDOW* sub, GList* renamed_list, int x, int y)
{
    if (renamed_list == NULL) {
        wgetch(sub);
        return;
    }

    int str_cnt = 0;
    size_t size = g_list_length(renamed_list);

    print_items(sub, renamed_list, &str_cnt, 5, 4);
    mvwprintw_highlite(sub, 4, 2, (char*)renamed_list->data);

    get_item(sub, renamed_list, y - 5, size, str_cnt, 5, 4);
}

void free_renamed_list(GList* renamed_file_list, GList* list)
{
    g_list_free_full(list, free);
    g_list_free_full(renamed_file_list, free_renamed_files);
}

void get_and_print_renamed_list(
        WINDOW* sub, GList* renamed_file_list, int x, int y)
{
    GList* list = get_renamed_list(sub, renamed_file_list);
    list = g_list_sort(list, my_comparator);
    print_renamed_list(sub, list, x, y);
    free_renamed_list(renamed_file_list, list);
}

void print_succes_message(GList* renamed_file_list, WINDOW* sub)
{
    int x = getmaxx(sub);
    if (g_list_length(renamed_file_list) == 0) {
        char str[] = "По введённым шаблонам файлов не найдено";
        mvwprintw(sub, 3, (x - 39) / 2, "%s", str);
        return;
    }
    char str[MAX_LEN];
    sprintf(str,
            "Успешно! Файлов переименовано: %d!",
            g_list_length(renamed_file_list));
    mvwprintw(sub, 3, (x - 34) / 2, "%s", str);
}

void process(WINDOW* menu, GList* sample, char* dir_path, Option* opt)
{
    int y, x;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    getmaxyx(sub, y, x);
    GList* renamed_file_list = NULL;
    if (g_list_length(sample) == 0) {
        char stri[] = "Сперва введите шаблоны, после запускайте переименование";
        mvwprintw(sub, 1, (x - 55) / 2, "%s", stri);
        wrefresh(sub);
        return;
    }
    mvwprintw(sub, 1, (x - 30) / 2, "Идет процесс переименования...");
    wrefresh(sub);
    renamed_file_list = rename_and_get_renamed_list(sample, dir_path, opt);
    print_succes_message(renamed_file_list, sub);
    get_and_print_renamed_list(sub, renamed_file_list, x, y);
    remove_current_window(sub);
}

int print_input_strings(WINDOW* sub, GList* input_strings)
{
    int cnt = 0;
    for (GList* i = input_strings; i != NULL; i = i->next) {
        mvwprintw(sub, 2 + cnt, 2, "%s", (char*)i->data);
        cnt++;
    }

    return cnt;
}

WINDOW* open_pattern_window(WINDOW* menu, int* x, int* y)
{
    getmaxyx(menu, *y, *x);
    WINDOW* sub = init_sub_window(menu, *y, *x);
    mvwprintw(sub, 1, 1, "Введите шаблон:");
    getmaxyx(sub, *y, *x);
    curs_set(1);
    echo();
    return sub;
}

void close_pattern_window(WINDOW* sub)
{
    curs_set(0);
    noecho();
    remove_current_window(sub);
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

void print_counter(WINDOW* sub, int x, int cnt, int max_cnt)
{
    char counter[MAX_LEN] = {0};
    sprintf(counter, "%d/%d", cnt, max_cnt);
    mvwprintw(sub, 1, x - (strlen(counter) + 2), "%s", counter);
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

char* s_gets(char* dest, size_t n, FILE* stream)
{
    if (fgets(dest, n, stream) == NULL)
        return NULL;
    for (int i = 0; dest[i] != '\0'; i++) {
        if (dest[i] == '\n') {
            dest[i] = '\0';
            break;
        }
    }
    return dest;
}

void show_info(WINDOW* menu)
{
    int y, x;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    x = getmaxx(sub);
    mvwprintw(sub, 1, 1, "Информация:");
    FILE* file = fopen("./txt/instruction.txt", "r");

    GList* str_list = NULL;
    char* str = malloc(sizeof(char) * 2 * x);
    while (s_gets(str, x * 2, file) != NULL) {
        str_list = g_list_append(str_list, str);
        str = malloc(sizeof(char) * 2 * x);
    }

    size_t size = g_list_length(str_list);
    int cnt = 0;

    print_items(sub, str_list, &cnt, 3, 2);
    // mvwprintw_highlite(sub, 2, 2, (char*)str_list->data);
    get_item(sub, str_list, y - 5, size, cnt, 3, 2);

    remove_current_window(sub);
}

void start(WINDOW* menu)
{
    GList* input_strings = NULL;
    GList* patterns = NULL;
    Option option = {0};
    char current_dir[MAX_LEN] = ".";
    int y = getmaxy(menu);
    int i = INPUT_PATTERN;

    while ((i = select_items(menu, menu_items, i, 1, 5)) != EXIT
           && i != KEY_F(10)) {
        wrefresh(menu);
        switch (i) {
        case INPUT_PATTERN:
            patterns = pattern_input(menu, &input_strings, patterns);
            break;
        case SELECT_DIR:
            select_dir(menu, current_dir);
            mvwprintw(menu, y - 2, 2, "Выбранный каталог: %-30s", current_dir);
            break;
        case PROCESS:
            process(menu, patterns, current_dir, &option);
            clean_data(&option, &input_strings, &patterns);
            break;
        case SELECT_OPT:
            select_option(
                    menu, &option, &input_strings, &patterns, current_dir);
            break;
        case INFO:
            show_info(menu);
            break;
        }
        mvwprintw_highlite(menu, i + 1, 2, menu_items[i]);
    }

    clean_data(&option, &input_strings, &patterns);
}