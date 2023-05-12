#include <locale.h>

#include <interface/init.h>
#include <interface/start.h>

int main()
{
    setlocale(LC_ALL, "");
    WINDOW* menu = init_menu();
    start(menu);

    delwin(menu);
    delwin(stdscr);
    endwin();

    return 0;
}
