#include <glib.h>

#include <interface/file_read.h>
#include <interface/info.h>
#include <interface/init.h>
#include <interface/print.h>

int show_info(WINDOW* menu)
{
    int y, x;
    WINDOW* sub = open_info_window(menu, &y, &x);

    GList* str_list = read_file(sub, x, "instruction.txt");
    if (str_list == NULL)
        return -1;

    int stat = print_info(sub, str_list, y);

    g_list_free_full(str_list, free);
    remove_current_window(sub);
    if (!stat)
        return 0;
    return 1;
}