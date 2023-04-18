#include <glib.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>

#include <fileproc/interface.h>
#include <libfileproc/directory.h>
#include <libfileproc/lexer.h>
#include <libfileproc/rename.h>

void clear_list_data(GSList* list)
{
    for (GSList* i = list; i != NULL; i = i->next) {
        free(i->data);
    }
}

// driver
int main()
{
    setlocale(LC_ALL, "");
    WINDOW* menu = init_menu();
    start(menu);

    delwin(menu);
    endwin();


    return 0;
}
