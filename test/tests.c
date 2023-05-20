#include <ctest.h>
#include <glib.h>
#include <stdio.h>

#include <libfileproc/clean_memory.h>
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

CTEST(rename, make_str_smaller)
{
    char str[] = "docs/need_make_this_string_smaller.txt";
    char result[100];
    int needed_len = 20;
    char expect[] = "docs/need_...ler.txt";
    make_str_smaller(str, result, strlen(str), needed_len);

    ASSERT_STR(expect, result);
}

CTEST(rename, write_correct_renamed_string)
{
    char* old_name = "filename.txt";
    char* new_name = "nemname.txt";
    int string_len = 30;
    char* expect = "filename.txt   ->    nemname.txt";
    char* result = write_correct_renamed_string(string_len, old_name, new_name);
    ASSERT_STR(expect, result);
    free(result);

    old_name = "bigefllenamewowowowoowwow.txt";
    new_name = "bigefllenamewowowowoowwow.jpg";
    string_len = 50;
    expect = "bigefllename...owwow.txt -> bigefllename...owwow.jpg";
    result = write_correct_renamed_string(string_len, old_name, new_name);
    ASSERT_STR(expect, result);
    free(result);

    old_name = "docs/filenameeeeeee.txt";
    new_name = "docs/nemenwnewnameeeeee.txt";
    string_len = 40;
    expect = "docs/file...ee.txt  ->  docs/neme...ee.txt";
    result = write_correct_renamed_string(string_len, old_name, new_name);
    ASSERT_STR(expect, result);
    free(result);
}

CTEST(lexer, skip_space)
{
    char string_0[] = "    abc";
    char string_1[] = "abc";
    char* result_0 = skip_space(string_0);
    char* result_1 = skip_space(string_1);
    char expected[] = "abc";

    ASSERT_STR(expected, result_0);
    ASSERT_STR(expected, result_1);
}

CTEST(lexer, check_wrong_symbols)
{
    int result_0 = check_wrong_symbols("abc");
    int result_1 = check_wrong_symbols("ab/c");
    int result_2 = check_wrong_symbols("ab<c");
    int result_3 = check_wrong_symbols("ab>c");
    int result_4 = check_wrong_symbols("ab|c");
    int result_5 = check_wrong_symbols("abc«");

    int expected_0 = 0;
    int expected_1 = -1;

    ASSERT_EQUAL(expected_0, result_0);
    ASSERT_EQUAL(expected_1, result_1);
    ASSERT_EQUAL(expected_1, result_2);
    ASSERT_EQUAL(expected_1, result_3);
    ASSERT_EQUAL(expected_1, result_4);
    ASSERT_EQUAL(expected_1, result_5);
}

CTEST(lexer, check_colon)
{
    int result_0 = check_colon("*.txt : *.doc");
    int result_1 = check_colon(":*.txt");
    int result_2 = check_colon("test.txt   *.txt");
    int result_3 = check_colon("test.txt : *.txt : *");

    int expected_0 = 0;
    int expected_1 = -1;

    ASSERT_EQUAL(expected_0, result_0);
    ASSERT_EQUAL(expected_1, result_1);
    ASSERT_EQUAL(expected_1, result_2);
    ASSERT_EQUAL(expected_1, result_3);
}

CTEST(lexer, check_star)
{
    char string_0[] = "*.txt";
    char string_1[] = "*.*";
    char string_2[] = "txt.*";
    char string_3[] = "*abc?.txt";
    char string_4[] = "**.txt";
    char string_5[] = "*?.txt";

    char* test_string;

    test_string = string_0;
    int result_0 = check_star(&test_string);
    test_string = string_1;
    int result_1 = check_star(&test_string);
    test_string = string_2;
    int result_2 = check_star(&test_string);
    test_string = string_3;
    int result_3 = check_star(&test_string);
    test_string = string_4;
    int result_4 = check_star(&test_string);
    test_string = string_5;
    int result_5 = check_star(&test_string);

    int expected_0 = 0;
    int expected_1 = -1;

    ASSERT_EQUAL(expected_0, result_0);
    ASSERT_EQUAL(expected_0, result_1);
    ASSERT_EQUAL(expected_0, result_2);
    ASSERT_EQUAL(expected_0, result_3);
    ASSERT_EQUAL(expected_1, result_4);
    ASSERT_EQUAL(expected_1, result_5);
}

CTEST(lexer, check_quest)
{
    int result_0 = check_quest("?abc*.txt");
    int result_1 = check_quest("?*abc.txt");

    int expected_0 = 0;
    int expected_1 = -1;

    ASSERT_EQUAL(expected_0, result_0);
    ASSERT_EQUAL(expected_1, result_1);
}

CTEST(lexer, check_space)
{
    int result_0 = check_space("   ");
    int result_1 = check_space("   bcd");

    int expected_0 = 0;
    int expected_1 = -1;

    ASSERT_EQUAL(expected_0, result_0);
    ASSERT_EQUAL(expected_1, result_1);
}

CTEST(lexer, get_tokens)
{
    char input_string_0[] = "*.txt : abc.txt";
    char input_string_1[] = ": abc.txt";
    char input_string_2[] = "*.txt :";
    char input_string_3[] = "";

    char* tokens[2];

    int result_1 = get_tokens(tokens, input_string_1, ":");
    int result_2 = get_tokens(tokens, input_string_2, ":");
    int result_3 = get_tokens(tokens, input_string_3, ":");
    int result_0 = get_tokens(tokens, input_string_0, ":");

    int expected_0 = 0;
    int expected_1 = -1;

    ASSERT_EQUAL(expected_0, result_0);
    ASSERT_EQUAL(expected_1, result_1);
    ASSERT_EQUAL(expected_1, result_2);
    ASSERT_EQUAL(expected_1, result_3);

    char expected_token_0[] = "*.txt ";
    char expected_token_1[] = " abc.txt";

    ASSERT_STR(expected_token_0, tokens[0]);
    ASSERT_STR(expected_token_1, tokens[1]);
}

CTEST(lexer, check_pattern)
{
    int result_0 = check_pattern("*.txt");
    int result_1 = check_pattern("**.txt");
    int result_2 = check_pattern("?*test.txt");
    int result_3 = check_pattern("test.txt   *.txt");
    int result_4 = check_pattern("*/.txt:  *");

    int expected_0 = 0;
    int expected_1 = -1;

    ASSERT_EQUAL(expected_0, result_0);
    ASSERT_EQUAL(expected_1, result_1);
    ASSERT_EQUAL(expected_1, result_2);
    ASSERT_EQUAL(expected_1, result_3);
    ASSERT_EQUAL(expected_1, result_4);
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

CTEST(lexer, get_pattern)
{
    char inp_str_0[] = "*.txt : a.txt";
    char inp_str_1[] = "*.txt:a.txt";
    char inp_str_2[] = "*.txt   ";

    char* pattern_0 = malloc(sizeof(char) * 10);
    char* pattern_1 = malloc(sizeof(char) * 10);
    char* pattern_2 = malloc(sizeof(char) * 10);

    char* test_inp_str_0 = get_pattern(inp_str_0, pattern_0);
    char* test_inp_str_1 = get_pattern(inp_str_1, pattern_1);
    char* test_inp_str_2 = get_pattern(inp_str_2, pattern_2);

    char expected_pattern[] = "*.txt";

    char expected_inp_str_0[] = " : a.txt";
    char expected_inp_str_1[] = ":a.txt";
    char expected_inp_str_2[] = "   ";

    ASSERT_STR(expected_inp_str_0, test_inp_str_0);
    ASSERT_STR(expected_inp_str_1, test_inp_str_1);
    ASSERT_STR(expected_inp_str_2, test_inp_str_2);

    ASSERT_STR(expected_pattern, pattern_0);
    ASSERT_STR(expected_pattern, pattern_1);
    ASSERT_STR(expected_pattern, pattern_2);

    free(pattern_0);
    free(pattern_1);
    free(pattern_2);
}

CTEST(lexer, to_rename_pattern)
{
    char inp_str_0[] = " : a.txt";
    char inp_str_1[] = ":a.txt";

    char* test_inp_str_0 = to_rename_pattern(inp_str_0);
    char* test_inp_str_1 = to_rename_pattern(inp_str_1);

    char expected_inp_str[] = "a.txt";

    ASSERT_STR(expected_inp_str, test_inp_str_0);
    ASSERT_STR(expected_inp_str, test_inp_str_1);
}

CTEST(lexer, split_input_string)
{
    char input_string_0[] = "*.txt : a.txt";
    char input_string_1[] = "*.txt:a.txt";

    SplittedPattern* patterns_0 = malloc(sizeof(SplittedPattern));
    SplittedPattern* patterns_1 = malloc(sizeof(SplittedPattern));

    patterns_0->search_pattern = malloc(sizeof(char) * MAX_LEN);
    patterns_0->rename_pattern = malloc(sizeof(char) * MAX_LEN);
    patterns_1->search_pattern = malloc(sizeof(char) * MAX_LEN);
    patterns_1->rename_pattern = malloc(sizeof(char) * MAX_LEN);

    split_input_string(input_string_0, patterns_0);
    split_input_string(input_string_1, patterns_1);

    char expected_search_pattern[] = "*.txt";
    char expected_rename_pattern[] = "a.txt";

    ASSERT_STR(expected_search_pattern, patterns_0->search_pattern);
    ASSERT_STR(expected_rename_pattern, patterns_0->rename_pattern);
    ASSERT_STR(expected_search_pattern, patterns_1->search_pattern);
    ASSERT_STR(expected_rename_pattern, patterns_1->rename_pattern);

    free_SplittedPattern(patterns_0);
    free_SplittedPattern(patterns_1);
}

CTEST(lexer, get_patterns)
{
    char input_string_0[] = "*.txt : a.txt";
    char input_string_1[] = "*.txt  a.txt";

    SplittedPattern* patterns = malloc(sizeof(SplittedPattern));

    int result_0 = get_patterns(input_string_0, patterns);
    int result_1 = get_patterns(input_string_1, patterns);

    char expected_search_pattern[] = "*.txt";
    char expected_rename_pattern[] = "a.txt";

    int expected_0 = 0;
    int expected_1 = -1;

    ASSERT_EQUAL(expected_0, result_0);
    ASSERT_EQUAL(expected_1, result_1);

    ASSERT_STR(expected_search_pattern, patterns->search_pattern);
    ASSERT_STR(expected_rename_pattern, patterns->rename_pattern);

    free_SplittedPattern(patterns);
}

CTEST(lexer, add_patterns)
{
    char input_string[] = "*.txt:a.txt";
    int exit_code;
    GList* patterns = NULL;
    patterns = add_patterns(patterns, input_string, &exit_code);

    char expected_search_pattern[] = "*.txt";
    char expected_rename_pattern[] = "a.txt";

    ASSERT_STR(
            expected_search_pattern,
            ((SplittedPattern*)patterns->data)->search_pattern);
    ASSERT_STR(
            expected_rename_pattern,
            ((SplittedPattern*)patterns->data)->rename_pattern);

    g_list_free_full(patterns, free_SplittedPattern);
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