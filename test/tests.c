#include <ctest.h>
#include <glib.h>
#include <stdio.h>

#include <libfileproc/directory.h>
#include <libfileproc/lexer.h>
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

CTEST(rename, get_name)
{
    char file_path[MAX_LEN] = "/usr/somerdirs/file";
    char* result = get_name(file_path);
    char* expect = "/file";
    ASSERT_STR(expect, result);

    char file_p[MAX_LEN] = "file";
    char* result1 = get_name(file_p);
    char* expect2 = "file";
    ASSERT_STR(expect2, result1);
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

    name = "text_file.jpg";
    pattern = "????.*";
    char dest7[MAX_LEN] = {0};
    expect = "text.jpg";
    get_new_name(name, pattern, dest7);
    ASSERT_STR(expect, dest7);

    name = "filename";
    pattern = "filename.old";
    char dest8[MAX_LEN] = {0};
    expect = "filename.old";
    get_new_name(name, pattern, dest8);
    ASSERT_STR(expect, dest8);

    name = "filename";
    pattern = "filename2";
    char dest9[MAX_LEN] = {0};
    expect = "filename2";
    get_new_name(name, pattern, dest9);
    ASSERT_STR(expect, dest9);
}

CTEST(rename, get_correct_name_default)
{
    FILE* f = fopen("rename_testing_file.txt", "w");
    fclose(f);
    f = fopen("rename_new_name.txt", "w");
    fclose(f);

    Option opt = {.name_register = R_DEFAULT};

    char* old_name = "rename_testing_file.txt";
    char* new_name = "rename_new_name.txt";
    char dest[MAX_LEN];
    char* expect = "rename_new_name (1).txt";
    get_correct_name(old_name, new_name, dest, &opt);
    remove("rename_testing_file.txt");
    remove("rename_new_name.txt");
    ASSERT_STR(expect, dest);
}

CTEST(rename, get_correct_name_upper)
{
    FILE* f = fopen("rename_testing_file.txt", "w");
    fclose(f);
    f = fopen("rename_new_name.txt", "w");
    fclose(f);

    Option opt = {.name_register = R_HIGH};

    char* old_name = "rename_testing_file.txt";
    char* new_name = "rename_new_name.txt";
    char dest[MAX_LEN];
    char* expect = "RENAME_NEW_NAME.txt";
    get_correct_name(old_name, new_name, dest, &opt);
    remove("rename_testing_file.txt");
    remove("rename_new_name.txt");
    ASSERT_STR(expect, dest);
}

CTEST(rename, get_correct_name_lower)
{
    FILE* f = fopen("rename_testing_file.txt", "w");
    fclose(f);
    f = fopen("Rename_New_Name.txt", "w");
    fclose(f);

    Option opt = {.name_register = R_LOW};

    char* old_name = "rename_testing_file.txt";
    char* new_name = "rename_new_name.txt";
    char dest[MAX_LEN];
    char* expect = "rename_new_name.txt";
    get_correct_name(old_name, new_name, dest, &opt);
    remove("rename_testing_file.txt");
    remove("Rename_New_Name.txt");
    ASSERT_STR(expect, dest);
}

CTEST(lexer, check_input_string)
{
    int result_0 = check_input_string("*.txt:  *");
    int result_1 = check_input_string("**.txt:*");
    int result_2 = check_input_string("?*test.txt:*");
    int result_3 = check_input_string("       :*.txt");
    int result_4 = check_input_string("test.txt:    ");
    int result_5 = check_input_string("test.txt   *.txt");
    int result_6 = check_input_string("test.txt : *.txt : *");
    int result_7 = check_input_string("test.txt 1.bin : one_piece.*");
    int result_8 = check_input_string("test.txt : *.txt  0");
    int result_9 = check_input_string("*/.txt:  *");

    int expected_0 = 0;
    int expected_1 = -1;

    ASSERT_EQUAL(expected_0, result_0);
    ASSERT_EQUAL(expected_1, result_1);
    ASSERT_EQUAL(expected_1, result_2);
    ASSERT_EQUAL(expected_1, result_3);
    ASSERT_EQUAL(expected_1, result_4);
    ASSERT_EQUAL(expected_1, result_5);
    ASSERT_EQUAL(expected_1, result_6);
    ASSERT_EQUAL(expected_1, result_7);
    ASSERT_EQUAL(expected_1, result_8);
    ASSERT_EQUAL(expected_1, result_9);
}

CTEST(lexer, get_patterns)
{
    char sample[] = "*.txt:  *";
    char search_pattern[] = "*.txt";
    char rename_pattern[] = "*";
    SplittedPattern patterns[10];
    int returned = get_patterns(sample, &patterns[3]);
    int search_pattern_result
            = strcmp(search_pattern, patterns[3].search_pattern);
    int rename_pattern_result
            = strcmp(rename_pattern, patterns[3].rename_pattern);
    int expected = 0;
    ASSERT_EQUAL(expected, returned);
    ASSERT_EQUAL(expected, search_pattern_result);
    ASSERT_EQUAL(expected, rename_pattern_result);
}

CTEST(directory, get_files_patterns_list)
{
    GList* result_list = NULL;
    GList* filesname = NULL;
    GList* sample = NULL;
    int expect = 0;

    filesname = g_list_append(filesname, "test.txt");
    filesname = g_list_append(filesname, "test.h");
    filesname = g_list_append(filesname, "tost.txt");

    SplittedPattern* part1 = malloc(sizeof(SplittedPattern));
    SplittedPattern* part2 = malloc(sizeof(SplittedPattern));
    SplittedPattern* part3 = malloc(sizeof(SplittedPattern));
    part1->rename_pattern = "*.jpg";
    part1->search_pattern = "*.txt";
    part2->rename_pattern = "*.pdf";
    part2->search_pattern = "*.*";
    part3->rename_pattern = "op.txt";
    part3->search_pattern = "tost.*";

    sample = g_list_append(sample, part1);
    sample = g_list_append(sample, part2);
    sample = g_list_append(sample, part3);

    result_list = get_files_patterns_list(filesname, sample);
    File_to_rename* file = (File_to_rename*)result_list->data;

    int result_file_name1 = strcmp(file->filename, "test.txt");
    int result_pattern1 = strcmp(file->pattern, "*.jpg");
    int result1 = (result_file_name1 == 0 && result_pattern1 == 0) ? 0 : 1;

    result_list = result_list->next;
    file = (File_to_rename*)result_list->data;

    int result_file_name2 = strcmp(file->filename, "test.h");
    int result_pattern2 = strcmp(file->pattern, "*.pdf");
    int result2 = (result_file_name2 == 0 && result_pattern2 == 0) ? 0 : 1;

    result_list = result_list->next;
    file = (File_to_rename*)result_list->data;

    int result_file_name3 = strcmp(file->filename, "tost.txt");
    int result_pattern31 = strcmp(file->pattern, "*.jpg");
    int result31 = (result_file_name3 == 0 && result_pattern31 == 0) ? 0 : 1;

    ASSERT_EQUAL(expect, result1);
    ASSERT_EQUAL(expect, result2);
    ASSERT_EQUAL(expect, result31);
}