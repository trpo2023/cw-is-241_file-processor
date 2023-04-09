#include <stdlib.h>
#include <string.h>

#include "interface.h"

const char menu_items[5][71] = {
        "1) Ввести шаблоны                      ",
        "2) Выбрать каталог                     ",
        "3) Запустить массовое переименование   ",
        "4) Опции                               ",
        "5) Выход из приложения (F10)           ",
};

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

    return KEY_F(1);
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

void start(WINDOW* menu)
{
    WINDOW* sub;
    int row, col;
    getmaxyx(menu, row, col);
    int result = 0;
    while ((result = item_select(menu, result)) != 4 && result != KEY_F(1)) {
        wrefresh(menu);
        switch (result) {
        case 0:
            sub = init_sub_window(menu, row, col);
            mvwprintw(sub, 1, 1, "ВВЕСТИ ШАБЛОНЫ");
            wrefresh(sub);
            delwin(sub);
            break;
        case 1:
            sub = init_sub_window(menu, row, col);
            mvwprintw(sub, 1, 1, "ВЫБРАТЬ КАТАЛОГ");
            wrefresh(sub);
            delwin(sub);
            break;
        case 2:
            sub = init_sub_window(menu, row, col);
            mvwprintw(sub, 1, 1, "ЗАПУСК");
            wrefresh(sub);
            delwin(sub);
            break;
        case 3:
            sub = init_sub_window(menu, row, col);
            mvwprintw(sub, 1, 1, "ВЫБОР ОПЦИЙ");
            wrefresh(sub);
            delwin(sub);
            break;
        }
    }

    return;
}