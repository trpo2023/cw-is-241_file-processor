#include <interface/init.h>
#include <interface/print.h>
#include <interface/start.h>

WINDOW* init_menu()
{
    const char* menu_items[] = {
            "1) Ввести шаблоны",
            "2) Выбрать каталог",
            "3) Запустить массовое переименование",
            "4) Опции",
            "5) Информация",
            "6) Выход из приложения (F10)",
    };
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

WINDOW* open_info_window(WINDOW* menu, int* y, int* x)
{
    getmaxyx(menu, *y, *x);
    WINDOW* sub = init_sub_window(menu, *y, *x);
    *x = getmaxx(sub);
    mvwprintw(sub, 1, 1, "Информация:");
    return sub;
}

void close_pattern_window(WINDOW* sub)
{
    curs_set(0);
    noecho();
    remove_current_window(sub);
}