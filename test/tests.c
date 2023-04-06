#include <ctest.h>
#include <stdio.h>

#include <libfileproc/rename.h>

CTEST(rename, get_suffix)
{
    char* string = "image.jpg";
    char* result;
    result = get_suffix(string);
    char* expect = ".jpg";
    ASSERT_STR(expect, result);

    string = "notsuffix";
    result = get_suffix(string);
    expect = NULL;
    ASSERT_STR(expect, result);
}

CTEST(rename, get_new_name)
{
    char* name = "one_piece.txt";
    char* pattern = "file-{*}.*.old";
    char dest[MAX_LEN] = {0};
    char* expect = "file-{one_piece}.txt.old";
    get_new_name(name, pattern, dest);
    ASSERT_STR(expect, dest);

    name = "image.jpg";
    pattern = "*.jpeg";
    char dest1[MAX_LEN] = {0};
    expect = "image.jpeg";
    get_new_name(name, pattern, dest1);
    ASSERT_STR(expect, dest1);

    name = "hellabc.txt";
    pattern = "hello*.txt";
    char dest2[MAX_LEN] = {0};
    expect = "helloabc.txt";
    get_new_name(name, pattern, dest2);
    ASSERT_STR(expect, dest2);

    name = "hellabc.txt.txt";
    pattern = "hell*o.txt";
    char dest3[MAX_LEN] = {0};
    expect = "hellabco.txt";
    get_new_name(name, pattern, dest3);
    ASSERT_STR(expect, dest3);

    name = "cool.txt";
    pattern = "coo?o.txt";
    char dest4[MAX_LEN] = {0};
    expect = "coolo.txt";
    get_new_name(name, pattern, dest4);
    ASSERT_STR(expect, dest4);

    name = "geometry.txt";
    pattern = "????.txt";
    char dest5[MAX_LEN] = {0};
    expect = "geom.txt";
    get_new_name(name, pattern, dest5);
    ASSERT_STR(expect, dest5);

    name = "hellooooo.txt";
    pattern = "hell?.txt";
    char dest6[MAX_LEN] = {0};
    expect = "hello.txt";
    get_new_name(name, pattern, dest6);
    ASSERT_STR(expect, dest6);
}

CTEST(rename, get_correct_name)
{
    FILE* f = fopen("rename_testing_file.txt", "w");
    fclose(f);
    f = fopen("rename_new_name.txt", "w");
    fclose(f);

    char* old_name = "rename_testing_file.txt";
    char* new_name = "rename_new_name.txt";
    char dest[MAX_LEN];
    char* expect = "rename_new_name (1).txt";
    get_correct_name(old_name, new_name, dest);
    remove("rename_testing_file.txt");
    remove("rename_new_name.txt");
    ASSERT_STR(expect, dest);
}