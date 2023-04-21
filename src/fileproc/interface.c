#include <glib.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"
#include <libfileproc/directory.h>
#include <libfileproc/rename.h>

const char menu_items[5][71] = {
        "1) Ввести шаблоны                      ",
        "2) Выбрать каталог                     ",
        "3) Запустить массовое переименование   ",
        "4) Опции                               ",
        "5) Выход из приложения (F10)           ",
};

void pattern_input(WINDOW* menu);

WINDOW* init_menu()
{
    initscr();
    int y, x;
    getmaxyx(stdscr, y, x);
    if (x < 120 || y < 30) {
        endwin();
        printf("\e[1;31mОшибка\e[0m: размер терминала меньше 120x30\n");
        printf("Текущий размер терминала: %dx%d (x, y)\n", x, y);
        exit(EXIT_FAILURE);
    }

    char* top = "File processor";
    mvwprintw(stdscr, 0, (x - strlen(top)) / 2, "%s", top);
    refresh();

    WINDOW* win = newwin(y - 1, x, 1, 0);
    box(win, 0, 0);
    getmaxyx(win, y, x);

    wattron(win, A_STANDOUT);
    mvwprintw(win, 1, 2, "%s", menu_items[0]);
    wattroff(win, A_STANDOUT);

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

int item_select(WINDOW* menu, int i)
{
    int y = getmaxy(menu);
    int ch;
    while ((ch = wgetch(menu)) != KEY_F(10)) {
        mvwprintw(menu, i + 1, 2, "%s", menu_items[i]);
        switch (ch) {
        case KEY_UP:
            i--;
            i = (i < 0) ? 4 : i;
            break;
        case KEY_DOWN:
            i++;
            i = (i > 4) ? 0 : i;
            break;
        case 10: // KEY_ENTER
            return i;
        }
        wattron(menu, A_STANDOUT);
        mvwprintw(menu, i + 1, 2, "%s", menu_items[i]);
        wattroff(menu, A_STANDOUT);

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
    case 0:
        switch (opt->name_register) {
        case R_DEFAULT:
            mvwprintw(sub, i + 3, 15, "%-20s", "Default");
            break;
        case R_LOW:
            mvwprintw(sub, i + 3, 15, "%-20s", "Low");
            break;
        case R_HIGH:
            mvwprintw(sub, i + 3, 15, "%-20s", "High");
            break;
        }
    }
}

void select_option(WINDOW* menu, Option* opt)
{
    const char* options[] = {"Регистр", "<- вернуться обратно в меню"};
    int y, x;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    mvwprintw(sub, 1, 1, "Выберите опции:");
    int options_cnt = 1; // 0 = 1 :)

    wattron(sub, A_STANDOUT);
    mvwprintw(sub, 3, 2, "%s", options[0]);
    wattroff(sub, A_STANDOUT);
    print_opt(sub, opt, 0);

    for (int i = 1; i <= options_cnt; i++) {
        mvwprintw(sub, i + 3, 2, "%s", options[i]);
    }

    wrefresh(sub);
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
            switch (i) {
            case 0:
                opt->name_register++;
                if (opt->name_register == 3)
                    opt->name_register = R_DEFAULT;
                print_opt(sub, opt, i);
                wrefresh(sub);
                break;
            case 1:
                wclear(sub);
                wrefresh(sub);
                return;
            }
        }
        wattron(sub, A_STANDOUT);
        mvwprintw(sub, i + 3, 2, "%s", options[i]);
        wattroff(sub, A_STANDOUT);
    }
}

void start(WINDOW* menu)
{
    WINDOW* sub;
    int row, col;
    getmaxyx(menu, row, col);
    int result = 0;
    char* current_dir = ".";
    Option option = {0};
    while ((result = item_select(menu, result)) != 4 && result != KEY_F(10)) {
        wrefresh(menu);
        switch (result) {
        case 0:
            pattern_input(menu);
            wattron(menu, A_STANDOUT);
            mvwprintw(menu, result + 1, 2, "%s", menu_items[result]);
            wattroff(menu, A_STANDOUT);
            break;
        case 1:
            current_dir = select_dir(menu);
            mvwprintw(
                    menu, row - 2, 2, "Выбранный каталог: %-30s", current_dir);
            wattron(menu, A_STANDOUT);
            mvwprintw(menu, result + 1, 2, "%s", menu_items[result]);
            wattroff(menu, A_STANDOUT);
            break;
        case 2:
            sub = init_sub_window(menu, row, col);
            mvwprintw(sub, 1, 1, "ЗАПУСК");
            wrefresh(sub);
            delwin(sub);
            break;
        case 3:
            select_option(menu, &option);
            wattron(menu, A_STANDOUT);
            mvwprintw(menu, result + 1, 2, "%s", menu_items[result]);
            wattroff(menu, A_STANDOUT);
            break;
        }
    }

    return;
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

        wattron(sub, A_STANDOUT);
        mvwprintw(sub, i + 2, 2, "%s", (char*)dir_list->data);
        wattroff(sub, A_STANDOUT);
        wrefresh(sub);
    }

    return (char*)dir_list->data;
}

char* select_dir(WINDOW* menu)
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
    wattron(sub, A_STANDOUT);
    mvwprintw(sub, 2, 2, "%s", (char*)dir_list->data);
    wattroff(sub, A_STANDOUT);

    dir = get_item(sub, dir_list, y - 5, dir_len, dir_cnt);

    wclear(sub);
    wrefresh(sub);
    return dir;
}

void pattern_input(WINDOW* menu)
{
    int y, x;
    getmaxyx(menu, y, x);
    WINDOW* sub = init_sub_window(menu, y, x);
    mvwprintw(sub, 1, 1, "Введите шаблон:");
    wrefresh(sub);

    getmaxyx(sub, y, x);

    int max_npatterns = y - 3;
    curs_set(1);
    echo();

    int cnt = 0;
    mvwprintw(sub, 1, x - 10, "%d/%d", cnt, max_npatterns);
    char str[MAX_LEN] = {0};
    mvwgetnstr(sub, 2, 2, str, x - 3);
    mvwprintw(sub, 1, x - 10, "%d/%d", ++cnt, max_npatterns);

    for (int i = 1; i < max_npatterns && str[0] != '\0'; i++) {
        mvwgetnstr(sub, 2 + i, 2, str, x - 3);
        mvwprintw(sub, 1, x - 10, "%d/%d", ++cnt, max_npatterns);
        wrefresh(sub);
    }

    curs_set(0);
    noecho();

    wclear(sub);
    wrefresh(sub);
    delwin(sub);
}
