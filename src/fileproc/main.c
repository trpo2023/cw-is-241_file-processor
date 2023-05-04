#include <locale.h>

#include <fileproc/interface.h>

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
