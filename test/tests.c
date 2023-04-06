#include <ctest.h>

#include <libfileproc/rename.h>

CTEST(rename, get_suffix)
{
    char* string = "text.txt";
    char* result;
    result = get_suffix(string);
    ASSERT_STR(".txt", result);
}