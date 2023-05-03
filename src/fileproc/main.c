#include <locale.h>

#include <fileproc/interface.h>
#include <libfileproc/directory.h>
#include <libfileproc/lexer.h>
#include <libfileproc/rename.h>

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
