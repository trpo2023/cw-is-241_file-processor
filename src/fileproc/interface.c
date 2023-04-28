#include <glib.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"
#include <libfileproc/directory.h>
#include <libfileproc/lexer.h>
#include <libfileproc/rename.h>
#include <libfileproc/running.h>

const char menu_items[5][71] = {
        "1) Ввести шаблоны                      ",
        "2) Выбрать каталог                     ",
        "3) Запустить массовое переименование   ",
        "4) Опции                               ",
        "5) Выход из приложения (F10)           ",
};

const char* options[] = {
        "Регистр", "Восстановить значения по умолчанию", "<-Вернуться в меню"};

void clean_data(Option* opt, GList** input_strings, GList** samples);

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

    for (int i = 1; i < 5; i++) {
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

int select_menu_items(WINDOW* menu, int i)
{
    int y = getmaxy(menu);
    int ch;
    int max_items = 4;
    while ((ch = wgetch(menu)) != KEY_F(10)) {
        mvwprintw(menu, i + 1, 2, "%s", menu_items[i]);
        switch (ch) {
        case KEY_UP:
            i--;
            i = (i < 0) ? max_items : i;
            break;
        case KEY_DOWN:
            i++;
            i = (i > max_items) ? 0 : i;
            break;
        case 10: // KEY_ENTER
            return i;
        }

        mvwprintw_highlite(menu, i + 1, 2, menu_items[i]);
        mvwprintw(menu, y - 3, 2, "%s", menu_items[i]);
    }

    return KEY_F(10);
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

int select_option_items(WINDOW* sub, const char* options[])
{
    int options_cnt = 2;
    int ch;
    int i = 0;

    while ((ch = wgetch(sub)) != KEY_F(10)) {
        mvwprintw(sub, i + 3, 2, "%s", options[i]);
        switch (ch) {
        case KEY_UP:
            i--;
            i = (i < 0) ? options_cnt : i;
            break;
        case KEY_DOWN:
            i++;
            i = (i > options_cnt) ? 0 : i;
            break;
        case 10: // KEY_ENTER
            return i;
        }
        mvwprintw_highlite(sub, i + 3, 2, options[i]);
    }

    return KEY_F(10);
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

    while ((i = select_option_items(sub, options)) != BACK && i != KEY_F(10)) {
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

    wclear(sub);
    wrefresh(sub);
    delwin(sub);
}

void free_renamed_files(void* data)
{
    free(((RenamedFile*)data)->new_path);
    free(data);
}

void process(WINDOW* menu, GList* sample, char* dir_path, Option* opt)
{
    int y, x;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    GList* renamed_file_list = NULL;
    if (g_list_length(sample) == 0) {
        mvwprintw(
                sub,
                1,
                1,
                "Сперва введите шаблоны, а потом запускайте "
                "переименование");
        wrefresh(sub);
        return;
    }
    mvwprintw(sub, 1, 15, "Идет процесс переименования...");
    renamed_file_list = rename_and_get_renamed_list(sample, dir_path, opt);
    char str[MAX_LEN];
    sprintf(str,
            "Успешно! Файлов переименовано: %d!",
            g_list_length(renamed_file_list));
    g_list_free_full(renamed_file_list, free_renamed_files);
    mvwprintw(sub, 3, 13, "%s", str);
    wrefresh(sub);
    delwin(sub);
}

void start(WINDOW* menu)
{
    GList* input_strings = NULL;
    GList* samples = NULL;
    Option option = {0};
    char current_dir[MAX_LEN] = ".";
    int y = getmaxy(menu);
    int i = INPUT_PATTERN;

    while ((i = select_menu_items(menu, i)) != EXIT && i != KEY_F(10)) {
        wrefresh(menu);
        switch (i) {
        case INPUT_PATTERN:
            samples = pattern_input(menu, &input_strings, samples);
            break;
        case SELECT_DIR:
            select_dir(menu, current_dir);
            mvwprintw(menu, y - 2, 2, "Выбранный каталог: %-30s", current_dir);
            break;
        case PROCESS:
            process(menu, samples, current_dir, &option);
            clean_data(&option, &input_strings, &samples);
            break;
        case SELECT_OPT:
            select_option(menu, &option, &input_strings, &samples, current_dir);
            break;
        }
        mvwprintw_highlite(menu, i + 1, 2, menu_items[i]);
    }

    clean_data(&option, &input_strings, &samples);
}

void print_dir(WINDOW* sub, GList* dir_list, int* dir_cnt)
{
    int y = getmaxy(sub);
    int cnt = 0;
    for (GList* i = dir_list; i != NULL && cnt < y - 3; i = i->next) {
        mvwprintw(sub, cnt + 2, 2, "%-55s", (char*)i->data);
        cnt++;
    }

    *dir_cnt = cnt;

    while (cnt < y - 3) {
        mvwprintw(sub, cnt + 2, 2, "%-55s", " ");
        cnt++;
    }

    wrefresh(sub);
}

// p - pages
// max_p - max_page
// dir - list of directories
void print_new_page(WINDOW* sub, GList* dir, int* dir_cnt, int p, int max_p)
{
    int x = getmaxx(sub);
    print_dir(sub, dir, dir_cnt);
    mvwprintw(sub, 1, x - 5, "%d/%d", p + 1, max_p + 1);
}

char* get_item(WINDOW* sub, GList* dir_list, int y, size_t len, int dir_cnt)
{
    int ch, i = 0, page = 0, x = getmaxx(sub);
    size_t max_pages = len / y;
    GList* pages[max_pages];

    // page counter
    mvwprintw(sub, 1, x - 5, "%d/%ld", page + 1, max_pages + 1);

    while ((ch = wgetch(sub)) != 10) { // 10 - KEY_ENTER
        mvwprintw(sub, i + 2, 2, "%s", (char*)dir_list->data);

        if (i == 0)
            pages[page] = dir_list;

        switch (ch) {
        case KEY_UP:
            i--;
            if (i < 0 && page == 1) {
                page--;
                print_new_page(sub, pages[page], &dir_cnt, page, max_pages);
                i = y - 1;
            } else if (i < 0 && page > 1) {
                page--;
                print_new_page(sub, pages[page], &dir_cnt, page, max_pages);
                i = y - 1;
            } else if (i < 0)
                i++;
            else
                dir_list = dir_list->prev;
            break;

        case KEY_DOWN:
            i++;
            if (i >= dir_cnt && page == max_pages)
                i--;
            else if (i >= y) {
                page++;
                print_new_page(sub, dir_list, &dir_cnt, page, max_pages);
                i = 0;
            } else
                dir_list = dir_list->next;

            break;
        }

        mvwprintw_highlite(sub, i + 2, 2, (char*)dir_list->data);
        wrefresh(sub);
    }

    return (char*)dir_list->data;
}

void free_input_string(void* str)
{
    free(str);
}

char* select_dir(WINDOW* menu, char* current_string)
{
    int y, x;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    mvwprintw(sub, 1, 1, "Выберите каталог:");
    char* dir = ".";

    GList* dir_list = get_files_or_dirs_list(".", dirs);
    dir_list = g_list_sort(dir_list, my_comparator);
    size_t dir_len = g_list_length(dir_list);

    int dir_cnt = 0; // count of directories on one page

    print_dir(sub, dir_list, &dir_cnt);

    // highliht first dir
    mvwprintw_highlite(sub, 2, 2, (char*)dir_list->data);

    dir = get_item(sub, dir_list, y - 5, dir_len, dir_cnt);

    strcpy(current_string, dir);

    wclear(sub);
    wrefresh(sub);
    delwin(sub);
    g_list_free_full(dir_list, free_input_string);

    return current_string;
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

GList* pattern_input(WINDOW* menu, GList** input_strings, GList* samples)
{
    int y, x;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    mvwprintw(sub, 1, 1, "Введите шаблон:");

    getmaxyx(sub, y, x);

    int max_ninstr = y - 3;
    curs_set(1);
    echo();
    int cnt = print_input_strings(sub, *input_strings);

    mvwprintw(sub, 1, x - 10, "%d/%d", cnt, max_ninstr);
    if (cnt == max_ninstr) {
        mvwprintw(sub, 1, 1, "Введено максимальное количество шаблонов!");
        curs_set(0);
        noecho();
        wgetch(sub);

        wclear(sub);
        delwin(sub);
        return samples;
    }

    char* str = malloc(x - 2);
    mvwgetnstr(sub, cnt + 2, 2, str, x - 3);

    int exit_code;
    if (str[0] != '\0') {
        samples = add_sample(samples, str, &exit_code);
        if (exit_code == 0) {
            *input_strings = g_list_append(*input_strings, str);
            mvwprintw(sub, 1, x - 10, "%d/%d", ++cnt, max_ninstr);
        } else {
            free(str);
            mvwprintw(sub, cnt + 2, 1, "╳");
            mvwprintw(sub, 1, x - 10, "%d/%d", ++cnt, max_ninstr);
        }
    } else {
        free(str);
        curs_set(0);
        noecho();

        wclear(sub);
        delwin(sub);

        return samples;
    }

    for (int i = cnt; i < max_ninstr && str[0] != '\0'; i++) {
        str = malloc(x - 3);
        mvwgetnstr(sub, 2 + i, 2, str, x - 3);
        if (str[0] == '\0') {
            free(str);
            break;
        }
        samples = add_sample(samples, str, &exit_code);
        if (exit_code == 0) {
            *input_strings = g_list_append(*input_strings, str);
        } else {
            mvwprintw(sub, cnt + 2, 1, "╳");
            free(str);
        }
        mvwprintw(sub, 1, x - 10, "%d/%d", ++cnt, max_ninstr);
    }

    curs_set(0);
    noecho();

    wclear(sub);
    delwin(sub);

    return samples;
}

void clean_data(Option* opt, GList** input_strings, GList** samples)
{
    opt->name_register = R_DEFAULT;
    if (*input_strings != NULL) {
        g_list_free_full(*input_strings, free_input_string);
        *input_strings = NULL;
    }
    if (*samples != NULL) {
        g_list_free_full(*samples, free_sample_parts);
        *samples = NULL;
    }
}