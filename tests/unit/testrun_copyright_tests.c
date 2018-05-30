/***
        ------------------------------------------------------------------------

        Copyright 2017 Markus Toepfer

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.

        This file is part of the testrun project. http://testrun.info

        ------------------------------------------------------------------------
*//**

        @file           testrun_copyright_tests.c
        @author         Markus Toepfer
        @date           2017-11-25

        @ingroup        testrun_lib

        @brief          Unit testing for testrun_copyright.


        ------------------------------------------------------------------------
*/


/*
 *      ------------------------------------------------------------------------
 *
 *      TEST SETUP                                                      #SETUP
 *
 *      ------------------------------------------------------------------------
 */

#include "../../include/testrun.h"
#include "../../src/testrun_copyright.c"

/*
 *      ------------------------------------------------------------------------
 *
 *      STRUCTURE RELATED FUNCTIONS                                     #SRF
 *
 *      Functions which manipulate or support the structure itself.
 *
 *      ------------------------------------------------------------------------
 */

int test_testrun_copyright_structure_is_valid(){

        testrun_copyright statement;

        bzero(&statement, sizeof(testrun_copyright));
        testrun(!testrun_copyright_structure_is_valid(&statement),
                "bzero initialized");

        statement = (testrun_copyright) {
                .intro          = NULL,
                .year           = NULL,
                .owner          = NULL,
                .note           = NULL,
                .text           = NULL,
                .to_string      = NULL
        };
        testrun(!testrun_copyright_structure_is_valid(&statement),
                "initialized to NULL");


        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "text",
                .owner          = "text",
                .note           = "text",
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        testrun(testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = NULL,
                .year           = "text",
                .owner          = "text",
                .note           = "text",
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        testrun(!testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = NULL,
                .owner          = "text",
                .note           = "text",
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        testrun(!testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "text",
                .owner          = NULL,
                .note           = "text",
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        testrun(!testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "text",
                .owner          = "text",
                .note           = "text",
                .text           = NULL,
                .to_string      = testrun_copyright_to_string
        };
        testrun(!testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "text",
                .owner          = "text",
                .note           = "text",
                .text           = "text",
                .to_string      = NULL
        };
        testrun(!testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "text",
                .owner          = "text",
                .note           = NULL,
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        testrun(testrun_copyright_structure_is_valid(&statement),
                "note is optional");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_calculate_length(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t expect  = 0;
        size_t result  = 0;
        size_t lineend = strlen(TESTRUN_LINEEND);

        testrun(0 == testrun_copyright_calculate_length(NULL));
        testrun(0 == testrun_copyright_calculate_length(&statement));

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "123",
                .to_string      = testrun_copyright_to_string
        };
        expect = 5 + lineend + 3 + lineend;
        result = testrun_copyright_calculate_length(&statement);
        testrun(expect == result);

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "2017",
                .owner          = "owner",
                .note           = NULL,
                .text           = "123",
                .to_string      = testrun_copyright_to_string
        };
        expect = 4 + 1 + 4 + 1 + 5 + lineend + 3 + lineend;
        result = testrun_copyright_calculate_length(&statement);
        testrun(expect == result);

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "2017",
                .owner          = "owner",
                .note           = "note",
                .text           = "123",
                .to_string      = testrun_copyright_to_string
        };
        expect = 4 + 1 + 4 + 1 + 5 + lineend + 3 + lineend + 4 + (2 * lineend);
        result = testrun_copyright_calculate_length(&statement);
        testrun(expect == result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_write_to(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 100;
        size_t open = size;
        size_t min  = 0;

        char expect[size];
        char buf[size];
        char *result = NULL;

        statement = (testrun_copyright) {
                .intro          = "i",
                .year           = "y",
                .owner          = "o",
                .note           = NULL,
                .text           = "t",
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_write_to(NULL, NULL, NULL));
        testrun(NULL == testrun_copyright_write_to(
                buf, &open, NULL));
        testrun(NULL == testrun_copyright_write_to(
                buf, NULL,  &statement));
        testrun(NULL == testrun_copyright_write_to(
                NULL,&open, &statement));

        snprintf(expect, size,  "i y o%s"
                                "t%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_write_to(buf, &open, &statement);
        testrun(result);
        testrun(result[0]  == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == size - strlen(expect));


        // reset
        open = size;
        bzero(buf, size);
        statement = (testrun_copyright) {
                .intro          = "intro",
                .year           = "year",
                .owner          = "owner",
                .note           = "note",
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "intro year owner%s"
                                "text%s"
                                "%snote%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_write_to(buf, &open, &statement);
        testrun(result);
        testrun(result[0]  == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == size - strlen(expect));


        // reset
        open = size;
        bzero(buf, size);
        statement = (testrun_copyright) {
                .intro          = "intro",
                .year           = "year",
                .owner          = "owner",
                .note           = "note",
                .text           = "line1\nline2\nline3\n",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "intro year owner%s"
                                "line1\nline2\nline3\n%s"
                                "%snote%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_write_to(buf, &open, &statement);
        testrun(result);
        testrun(result[0]  == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == size - strlen(expect));

        // write behind (write to previous result)
        statement = (testrun_copyright) {
                .intro          = "2",
                .year           = "2",
                .owner          = "2",
                .note           = "2",
                .text           = "line1\nline2\nline3\n",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "intro year owner%s"
                                "line1\nline2\nline3\n%s"
                                "%snote%s"
                                "2 2 2%s"
                                "line1\nline2\nline3\n%s"
                                "%s2%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_write_to(result,&open, &statement);
        testrun(result);
        testrun(result[0]  == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == size - strlen(expect));

        // write behind (write to previous result)
        statement = (testrun_copyright) {
                .intro          = "3",
                .year           = "3",
                .owner          = "3",
                .note           = NULL,
                .text           = "3",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "intro year owner%s"
                                "line1\nline2\nline3\n%s"
                                "%snote%s"
                                "2 2 2%s"
                                "line1\nline2\nline3\n%s"
                                "%s2%s"
                                "3 3 3%s"
                                "3%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_write_to(result,&open, &statement);
        testrun(result);
        testrun(result[0]  == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == size - strlen(expect));

        // reset
        open = size;
        bzero(buf, size);
        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "1 2 3%s"
                                "4%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);

        // check write < min
        min    = 5 + strlen(TESTRUN_LINEEND) + 1 + strlen(TESTRUN_LINEEND) + 1;
        open   = min - 1;
        result = testrun_copyright_write_to(buf, &open, &statement);
        testrun(!result, "buffer to small, nothing written.");
        for (size_t i = 0; i < size; i++){
                testrun(buf[i] == 0, "expect nothinǵ written.");
        }
        testrun(open == min - 1, "open unchanged");

        // check write to min
        open   = min;
        result = testrun_copyright_write_to(buf, &open, &statement);
        testrun(result);
        testrun(result[0] == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == 1);

        // check try to write behind, but buffer is to small
        open   = min - 1;
        result = testrun_copyright_write_to(result,&open, &statement);

        testrun(!result, "buffer to small, nothing written.");
        for (size_t i = 0; i < size; i++){
                if (i < (min - 1)){
                        testrun(buf[i] != 0, "written in run 1");
                } else {
                       testrun(buf[i] == 0, "expect nothinǵ written in run 2");
                }
        }
        testrun(open == min - 1, "open unchanged");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_to_string(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 200;
        char expect[size];
        char *result = NULL;

        statement = (testrun_copyright) {
                .intro          = "i",
                .year           = "y",
                .owner          = NULL,
                .note           = NULL,
                .text           = "t",
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_to_string(NULL));
        testrun(NULL == testrun_copyright_to_string(&statement),
                "statement invalid");

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "1 2 3" TESTRUN_LINEEND
                                "4"     TESTRUN_LINEEND);
        result = testrun_copyright_to_string(&statement);
        testrun(result);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);


        statement = (testrun_copyright) {
                .intro          = "intro",
                .year           = "year",
                .owner          = "owner",
                .note           = "note",
                .text           = "line1\nline2\nline3\n",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "intro year owner"      TESTRUN_LINEEND
                                "line1\nline2\nline3\n" TESTRUN_LINEEND
                                TESTRUN_LINEEND
                                "note" TESTRUN_LINEEND);
        result = testrun_copyright_to_string(&statement);
        testrun(result);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_gpl_statement_to_string(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 200;
        char expect[size];
        char *result = NULL;

        statement = (testrun_copyright) {
                .intro          = "i",
                .year           = "y",
                .owner          = NULL,
                .note           = NULL,
                .text           = "t",
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_gpl_statement_to_string(NULL));
        testrun(NULL == testrun_copyright_gpl_statement_to_string(&statement),
                "statement invalid");

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_gpl_statement_to_string(&statement),
                "statement gpl invalid");

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .program        = "5",
                .variant        = NULL,
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_gpl_statement_to_string(&statement),
                "statement gpl invalid");

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .program        = NULL,
                .variant        = "6",
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_gpl_statement_to_string(&statement),
                "statement gpl invalid");

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .program        = "5",
                .variant        = "6",
                .to_string      = testrun_copyright_to_string
        };

        snprintf(expect, size,  "1 2 3%s"
                                "4%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_gpl_statement_to_string(&statement);
        testrun(result, "no TAG to replace contained.");
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);

        char *text = "4"
                TESTRUN_COPYRIGHT_TAG_PROGRAM
                TESTRUN_COPYRIGHT_TAG_PROGRAM
                TESTRUN_COPYRIGHT_TAG_PROGRAM
                TESTRUN_COPYRIGHT_TAG_PROGRAM;

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = text,
                .program        = "5",
                .variant        = "6",
                .to_string      = testrun_copyright_to_string
        };

        snprintf(expect, size,  "1 2 3%s"
                                "45555%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_gpl_statement_to_string(&statement);
        testrun(result, "replaced all tags program");
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);

        text = "4"
                TESTRUN_COPYRIGHT_TAG_VARIANT
                TESTRUN_COPYRIGHT_TAG_VARIANT
                TESTRUN_COPYRIGHT_TAG_VARIANT;

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = text,
                .program        = "5",
                .variant        = "6",
                .to_string      = testrun_copyright_to_string
        };

        snprintf(expect, size,  "1 2 3%s"
                                "4666%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_gpl_statement_to_string(&statement);
        testrun(result, "replaced all tags variant");
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);

        text = "4"
                TESTRUN_COPYRIGHT_TAG_PROGRAM
                TESTRUN_COPYRIGHT_TAG_VARIANT
                TESTRUN_COPYRIGHT_TAG_PROGRAM
                TESTRUN_COPYRIGHT_TAG_VARIANT;

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = text,
                .program        = "5",
                .variant        = "6",
                .to_string      = testrun_copyright_to_string
        };

        snprintf(expect, size,  "1 2 3%s"
                                "45656%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_gpl_statement_to_string(&statement);
        testrun(result, "replaced all tags variant and program");
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      OUTPUT FORMAT RELATED FUNCTIONS                                 #OFRF
 *
 *      Special output formats, which allow preconfigured and standardized
 *      output of the copyright statements, as well as custom output defintion.
 *
 *      ------------------------------------------------------------------------
 */

int test_testrun_copyright_to_header(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 1500;
        char expect[size];
        char text[size];
        char *result = NULL;
        char *prefix = NULL;
        char *suffix = NULL;
        char *line_prefix = NULL;
        char *line_suffix = NULL;

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .to_string      = testrun_copyright_to_string
        };

        testrun(!testrun_copyright_to_header(   NULL,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0));

        // -------------------------------------------------------------
        // Positive test - unframed
        // -------------------------------------------------------------

        snprintf(expect, size,  "1 2 3%s"
                                "4%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);

        result = testrun_copyright_to_header(   &statement,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0);
        testrun(strncmp(result, expect, strlen(expect)) == 0, "unframed");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - header prefixed
        // -------------------------------------------------------------

        prefix      = "abc" TESTRUN_LINEEND;
        suffix      = NULL;
        line_prefix = NULL;
        line_suffix = NULL;

        snprintf(expect, size,  "%s1 2 3" TESTRUN_LINEEND
                                "4"       TESTRUN_LINEEND,
                                prefix);

        result = testrun_copyright_to_header(   &statement,
                                                prefix, strlen(prefix),
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0);

        // log("|%s|\n", result);

        testrun(strncmp(result, expect, strlen(expect)) == 0, "prefixed");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - header suffixed
        // -------------------------------------------------------------

        prefix      = NULL;
        suffix      = "abc" TESTRUN_LINEEND;
        line_prefix = NULL;
        line_suffix = NULL;

        snprintf(expect, size,  "1 2 3" TESTRUN_LINEEND
                                "4" TESTRUN_LINEEND
                                "%s",
                                suffix);

        result = testrun_copyright_to_header(   &statement,
                                                NULL, 0,
                                                suffix, strlen(suffix),
                                                NULL, 0,
                                                NULL, 0);

        //log("|%s|\n", result);

        testrun(strncmp(result, expect, strlen(expect)) == 0, "suffixed");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - line prefixed
        // -------------------------------------------------------------

        prefix      = NULL;
        suffix      = NULL;
        line_prefix = "#";
        line_suffix = NULL;

        snprintf(expect, size,  "%s""1 2 3" TESTRUN_LINEEND
                                "%s""4"     TESTRUN_LINEEND,
                                line_prefix, line_prefix);

        result = testrun_copyright_to_header(   &statement,
                                                NULL, 0,
                                                NULL, 0,
                                                line_prefix, strlen(line_prefix),
                                                NULL, 0);

        //log("|%s|\n", result);

        testrun(strncmp(result, expect, strlen(expect)) == 0, "line prefixed");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - line suffixed
        // -------------------------------------------------------------


        prefix      = NULL;
        suffix      = NULL;
        line_prefix = NULL;
        line_suffix = "#";

        snprintf(expect, size,  "1 2 3""%s" TESTRUN_LINEEND
                                "4""%s" TESTRUN_LINEEND,
                                line_suffix, line_suffix);

        result = testrun_copyright_to_header(   &statement,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                line_suffix, strlen(line_suffix));

        //log("|%s|\n", result);

        testrun(strncmp(result, expect, strlen(expect)) == 0, "line suffixed");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - real world
        // -------------------------------------------------------------

        snprintf(text, size,    "line1" TESTRUN_LINEEND
                                "line2" TESTRUN_LINEEND
                                "line3" TESTRUN_LINEEND
                                TESTRUN_LINEEND
                                "line4");

        statement.text = text;

        prefix      = "/**" TESTRUN_LINEEND;
        suffix      = " */" TESTRUN_LINEEND;
        line_prefix = " *  ";
        line_suffix = NULL;

        snprintf(expect, size,  "/**" TESTRUN_LINEEND
                                " *  1 2 3" TESTRUN_LINEEND
                                " *  line1" TESTRUN_LINEEND
                                " *  line2" TESTRUN_LINEEND
                                " *  line3" TESTRUN_LINEEND
                                " *"TESTRUN_LINEEND
                                " *  line4" TESTRUN_LINEEND
                                " */"TESTRUN_LINEEND);

        result = testrun_copyright_to_header(   &statement,
                                                prefix, strlen(prefix),
                                                suffix, strlen(suffix),
                                                line_prefix, strlen(line_prefix),
                                                NULL, 0);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));

        testrun(strncmp(result, expect, strlen(expect)) == 0, "reallife");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - real world 2
        // -------------------------------------------------------------

        statement = testrun_copyright_apache_version_2(
                "2017", "MT" , "some additional note");

        prefix      = "/**" TESTRUN_LINEEND;
        suffix      = " */" TESTRUN_LINEEND;
        line_prefix = " *  ";
        line_suffix = NULL;

        snprintf(expect, size,
        "/**"
        TESTRUN_LINEEND
        " *  Copyright 2017 MT"
        TESTRUN_LINEEND
        " *"
        TESTRUN_LINEEND
        " *  Licensed under the Apache License, Version 2.0 (the \"License\");"
        TESTRUN_LINEEND
        " *  you may not use this file except in compliance with the License."
        TESTRUN_LINEEND
        " *  You may obtain a copy of the License at"
        TESTRUN_LINEEND
        " *"
        TESTRUN_LINEEND
        " *          http://www.apache.org/licenses/LICENSE-2.0"
        TESTRUN_LINEEND
        " *"
        TESTRUN_LINEEND
        " *  Unless required by applicable law or agreed to in writing, software"
        TESTRUN_LINEEND
        " *  distributed under the License is distributed on an \"AS IS\" BASIS,"
        TESTRUN_LINEEND
        " *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."
        TESTRUN_LINEEND
        " *  See the License for the specific language governing permissions and"
        TESTRUN_LINEEND
        " *  limitations under the License."
        TESTRUN_LINEEND
        " *"
        TESTRUN_LINEEND
        " *  some additional note"
        TESTRUN_LINEEND
        " */"
        TESTRUN_LINEEND);

        result = testrun_copyright_to_header(   &statement,
                                                prefix, strlen(prefix),
                                                suffix, strlen(suffix),
                                                line_prefix, strlen(line_prefix),
                                                NULL, 0);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));

        testrun(strncmp(result, expect, strlen(expect)) == 0, "reallife");
        result = testrun_string_free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_default_c_header(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 1500;
        char expect[size];
        char *result = NULL;

        statement = testrun_copyright_default("2017", "NAME", "NOTE");

        snprintf(expect, size,
        "/***"  TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) 2017 NAME"       TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        NOTE" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        );

        testrun(!testrun_copyright_default_c_header(NULL, false));
        testrun(!testrun_copyright_default_c_header(NULL, true));

        result = testrun_copyright_default_c_header(&statement, false);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);


        statement = testrun_copyright_apache_version_2("YEAR", "NAME", NULL);

        snprintf(expect, size,
        "/***"  TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright YEAR NAME" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Licensed under the Apache License, Version 2.0 (the \"License\");" TESTRUN_LINEEND
        "        you may not use this file except in compliance with the License." TESTRUN_LINEEND
        "        You may obtain a copy of the License at" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "                http://www.apache.org/licenses/LICENSE-2.0" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Unless required by applicable law or agreed to in writing, software" TESTRUN_LINEEND
        "        distributed under the License is distributed on an \"AS IS\" BASIS," TESTRUN_LINEEND
        "        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied." TESTRUN_LINEEND
        "        See the License for the specific language governing permissions and" TESTRUN_LINEEND
        "        limitations under the License." TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        );

        testrun(!testrun_copyright_default_c_header(NULL, false));
        testrun(!testrun_copyright_default_c_header(NULL, true));

        result = testrun_copyright_default_c_header(&statement, false);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);


        statement = testrun_copyright_default("2017", "NAME", "NOTE");

        snprintf(expect, size,
        "/***"  TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) 2017 NAME"       TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        NOTE" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        );

        result = testrun_copyright_default_c_header(&statement, true);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_default_shell_header(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 1500;
        char expect[size];
        char *result = NULL;

        statement = testrun_copyright_default("2017", "NAME", "NOTE");

        snprintf(expect, size,
        "#!/usr/bin/env bash"  TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       Copyright (c) 2017 NAME"       TESTRUN_LINEEND
        "#       All rights reserved."          TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       NOTE" TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        );


        testrun(!testrun_copyright_default_shell_header(NULL));

        result = testrun_copyright_default_shell_header(&statement);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);


        statement = testrun_copyright_apache_version_2("YEAR", "NAME", NULL);

        snprintf(expect, size,
        "#!/usr/bin/env bash"  TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       Copyright YEAR NAME" TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       Licensed under the Apache License, Version 2.0 (the \"License\");" TESTRUN_LINEEND
        "#       you may not use this file except in compliance with the License." TESTRUN_LINEEND
        "#       You may obtain a copy of the License at" TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#               http://www.apache.org/licenses/LICENSE-2.0" TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       Unless required by applicable law or agreed to in writing, software" TESTRUN_LINEEND
        "#       distributed under the License is distributed on an \"AS IS\" BASIS," TESTRUN_LINEEND
        "#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied." TESTRUN_LINEEND
        "#       See the License for the specific language governing permissions and" TESTRUN_LINEEND
        "#       limitations under the License." TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        );

        result = testrun_copyright_default_shell_header(&statement);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      CONTENT RELATED FUNCTIONS                                       #CRF
 *
 *      Functions which create content copyright statements.
 *      NOTE: These statements are hardcoded with intention.
 *
 *      ------------------------------------------------------------------------
 */

int test_testrun_copyright_default(){

        testrun_copyright statement;

        char *intro = "Copyright (c)";
        char *text  = "All rights reserved.";

        statement = testrun_copyright_default(NULL, NULL, NULL);
        testrun(statement.intro != NULL, "check structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_default("2017", NULL, NULL);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_default("2017", "name", NULL);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);


        statement = testrun_copyright_default("2017", "name", "note");
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_default(NULL, "name", NULL);
        testrun(statement.intro != NULL, "check name only structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_default(NULL, NULL, "note");
        testrun(statement.intro != NULL, "check note only structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(strncmp(statement.note, "note",  strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_apache_version_2(){

        testrun_copyright statement;

        char *intro = "Copyright";
        char *text  =
        "\n"
        "Licensed under the Apache License, Version 2.0 (the \"License\");\n"
        "you may not use this file except in compliance with the License.\n"
        "You may obtain a copy of the License at\n"
        "\n"
        "        http://www.apache.org/licenses/LICENSE-2.0\n"
        "\n"
        "Unless required by applicable law or agreed to in writing, software\n"
        "distributed under the License is distributed on an \"AS IS\" BASIS,\n"
        "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
        "See the License for the specific language governing permissions and\n"
        "limitations under the License.";

        statement = testrun_copyright_apache_version_2(
                NULL, NULL, NULL);
        testrun(statement.intro != NULL, "check structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);
        testrun(statement.full_text == testrun_copyright_apache_v2_licence);
        /*
        testrun(strncmp(statement.filename,
                TESTRUN_COPYRIGHT_FILE_APACHE,
                strlen(TESTRUN_COPYRIGHT_FILE_APACHE) == 0));
*/
        statement = testrun_copyright_apache_version_2(
                "2017", NULL, NULL);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_apache_version_2(
                "2017", "name", NULL);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);


        statement = testrun_copyright_apache_version_2(
                "2017", "name", "note");
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_GPL_v3(){

        testrun_copyright statement;

        char *intro   = "Copyright ";
        char *program = TESTRUN_COPYRIGHT_TAG_PROGRAM;
        char *variant = "General";
        char *string  = NULL;

        char text[1500];
        char text2[1500];
        bzero(text, 1500);
        snprintf(text, 1500,
        "\n"
        "This file is part of " TESTRUN_COPYRIGHT_TAG_PROGRAM "\n"
        "\n"
        TESTRUN_COPYRIGHT_TAG_PROGRAM
        " is free software: you can redistribute it and/or modify\n"
        "it under the terms of the GNU "
        TESTRUN_COPYRIGHT_TAG_VARIANT
        " Public License as published by\n"
        "the Free Software Foundation, either version 3 of the License, or\n"
        "(at your option) any later version.\n"
        "\n"
        TESTRUN_COPYRIGHT_TAG_PROGRAM
        " is distributed in the hope that it will be useful,\n"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
        "GNU "TESTRUN_COPYRIGHT_TAG_VARIANT" Public License for more details.\n"
        "\n"
        "You should have received a copy of the GNU "
        TESTRUN_COPYRIGHT_TAG_VARIANT" Public License\n"
        "along with " TESTRUN_COPYRIGHT_TAG_PROGRAM
        ". If not, see <http://www.gnu.org/licenses/>.");

        statement = testrun_copyright_GPL_v3(
                NULL, NULL, NULL, NULL, 0);
        testrun(statement.intro != NULL, "check structure created.");


        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);
        testrun(statement.full_text == testrun_copyright_gpl_v3_general_licence);
        /*
        testrun(strncmp(statement.filename,
                TESTRUN_COPYRIGHT_FILE_GPL,
                strlen(TESTRUN_COPYRIGHT_FILE_GPL) == 0));
        */
        statement = testrun_copyright_GPL_v3(
                "2017", NULL, NULL, NULL, 0);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);
        testrun(statement.full_text == testrun_copyright_gpl_v3_general_licence);
        /*
        testrun(strncmp(statement.filename,
                TESTRUN_COPYRIGHT_FILE_GPL,
                strlen(TESTRUN_COPYRIGHT_FILE_GPL) == 0));
        */
        statement = testrun_copyright_GPL_v3(
                "2017", "name", NULL, NULL, 0);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);
        testrun(statement.full_text == testrun_copyright_gpl_v3_general_licence);

        statement = testrun_copyright_GPL_v3(
                "2017", "name", "note", NULL, 0);
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);
        testrun(statement.full_text == testrun_copyright_gpl_v3_general_licence);

        statement = testrun_copyright_GPL_v3(
                "2017", "name", "note", NULL, 0);
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);

        program = "TEST";
        variant = "General";
        bzero(text2, 1500);
        snprintf(text2, 1500,
        "%s %s %s\n"
        "\n"
        "This file is part of %s\n"
        "\n"
        "%s is free software: you can redistribute it and/or modify\n"
        "it under the terms of the GNU %s"
        " Public License as published by\n"
        "the Free Software Foundation, either version 3 of the License, or\n"
        "(at your option) any later version.\n"
        "\n"
        "%s is distributed in the hope that it will be useful,\n"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
        "GNU %s Public License for more details.\n"
        "\n"
        "You should have received a copy of the GNU %s Public License\n"
        "along with %s. If not, see <http://www.gnu.org/licenses/>.\n\n"
        "note\n",
        statement.intro, statement.year, statement.owner,
        program, program, variant, program, variant, variant, program);

        statement = testrun_copyright_GPL_v3(
                "2017", "name", "note", program, 0);
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);
        testrun(statement.full_text == testrun_copyright_gpl_v3_general_licence);

        string = statement.to_string(&statement);
        testrun(strncmp(string,  text2,   strlen(text2))   == 0);
        free(string);

        program = "PROGRAM";
        variant = "Lesser General";
        bzero(text2, 1500);
        snprintf(text2, 1500,
        "%s %s %s\n"
        "\n"
        "This file is part of %s\n"
        "\n"
        "%s is free software: you can redistribute it and/or modify\n"
        "it under the terms of the GNU %s"
        " Public License as published by\n"
        "the Free Software Foundation, either version 3 of the License, or\n"
        "(at your option) any later version.\n"
        "\n"
        "%s is distributed in the hope that it will be useful,\n"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
        "GNU %s Public License for more details.\n"
        "\n"
        "You should have received a copy of the GNU %s Public License\n"
        "along with %s. If not, see <http://www.gnu.org/licenses/>.\n\n"
        "note\n",
        statement.intro, statement.year, statement.owner,
        program, program, variant, program, variant, variant, program);

        statement = testrun_copyright_GPL_v3(
                "2017", "name", "note", program, LESSER);
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);
        testrun(statement.full_text == testrun_copyright_gpl_v3_lesser_licence);

        string = statement.to_string(&statement);
        testrun(strncmp(string,  text2,   strlen(text2))   == 0);
        free(string);

        program = "PROGRAM";
        variant = "Affero General";
        bzero(text2, 1500);
        snprintf(text2, 1500,
        "%s %s %s\n"
        "\n"
        "This file is part of %s\n"
        "\n"
        "%s is free software: you can redistribute it and/or modify\n"
        "it under the terms of the GNU %s"
        " Public License as published by\n"
        "the Free Software Foundation, either version 3 of the License, or\n"
        "(at your option) any later version.\n"
        "\n"
        "%s is distributed in the hope that it will be useful,\n"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
        "GNU %s Public License for more details.\n"
        "\n"
        "You should have received a copy of the GNU %s Public License\n"
        "along with %s. If not, see <http://www.gnu.org/licenses/>.\n\n"
        "note\n",
        statement.intro, statement.year, statement.owner,
        program, program, variant, program, variant, variant, program);

        statement = testrun_copyright_GPL_v3(
                "2017", "name", "note", program, AFFERO);
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);
        testrun(statement.full_text == testrun_copyright_gpl_v3_affero_licence);

        string = statement.to_string(&statement);
        testrun(strncmp(string,  text2,   strlen(text2))   == 0);
        free(string);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_BSD_3Clause(){

        testrun_copyright statement;

        char *intro = "Copyright ";
        char *text =
        "\n"
        "Redistribution and use in source and binary forms, with or without\n"
        "modification, are permitted provided that the following conditions are met:\n"
        "    * Redistributions of source code must retain the above copyright\n"
        "      notice, this list of conditions and the following disclaimer.\n"
        "    * Redistributions in binary form must reproduce the above copyright\n"
        "      notice, this list of conditions and the following disclaimer in the\n"
        "      documentation and/or other materials provided with the distribution.\n"
        "    * Neither the name of the copyright holder nor the\n"
        "      names of its contributors may be used to endorse or promote products\n"
        "      derived from this software without specific prior written permission.\n"
        "\n"
        "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND\n"
        "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n"
        "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n"
        "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY\n"
        "DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n"
        "(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n"
        "LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND\n"
        "ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n"
        "(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n"
        "SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.";

        statement = testrun_copyright_BSD_3Clause(
                NULL, NULL, NULL);
        testrun(statement.intro != NULL, "check structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);

        statement = testrun_copyright_BSD_3Clause(
                "2017", NULL, NULL);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_BSD_3Clause(
                "2017", "name", NULL);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);


        statement = testrun_copyright_BSD_3Clause(
                "2017", "name", "note");
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);

        // log("%s", statement.to_string(&statement));

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_BSD_2Clause(){

        testrun_copyright statement;

        char *intro = "Copyright";
        char *text =
        "\n"
        "Redistribution and use in source and binary forms, with or without\n"
        "modification, are permitted provided that the following conditions are met:\n"
        "1. Redistributions of source code must retain the above copyright notice, this\n"
        "   list of conditions and the following disclaimer.\n"
        "2. Redistributions in binary form must reproduce the above copyright notice,\n"
        "   this list of conditions and the following disclaimer in the documentation\n"
        "   and/or other materials provided with the distribution.\n"
        "\n"
        "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND\n"
        "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n"
        "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n"
        "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR\n"
        "ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n"
        "(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n"
        "LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND\n"
        "ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n"
        "(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n"
        "SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.";

        statement = testrun_copyright_BSD_2Clause(
                NULL, NULL, NULL);
        testrun(statement.intro != NULL, "check structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);

        statement = testrun_copyright_BSD_2Clause(
                "2017", NULL, NULL);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_BSD_2Clause(
                "2017", "name", NULL);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);


        statement = testrun_copyright_BSD_2Clause(
                "2017", "name", "note");
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);

        //log("%s", statement.to_string(&statement));

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_MIT(){

        testrun_copyright statement;

        char *intro = "Copyright";
        char *text =
        "\n"
        "Permission is hereby granted, free of charge, to any person obtaining a copy\n"
        "of this software and associated documentation files (the \"Software\"), to deal\n"
        "in the Software without restriction, including without limitation the rights\n"
        "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
        "copies of the Software, and to permit persons to whom the Software is\n"
        "furnished to do so, subject to the following conditions:\n"
        "\n"
        "The above copyright notice and this permission notice shall be included in all\n"
        "copies or substantial portions of the Software.\n"
        "\n"
        "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
        "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
        "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
        "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
        "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
        "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
        "SOFTWARE.";

        statement = testrun_copyright_MIT(
                NULL, NULL, NULL);
        testrun(statement.intro != NULL, "check structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);

        statement = testrun_copyright_MIT(
                "2017", NULL, NULL);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_MIT(
                "2017", "name", NULL);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);


        statement = testrun_copyright_MIT(
                "2017", "name", "note");
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);

        //log("%s", statement.to_string(&statement));

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_apache_v2_licence(){

        size_t size = 15000;
        char expect[size];

        char *result = testrun_copyright_apache_v2_licence();

        snprintf(expect, size,
        "                              Apache License\n"
        "                        Version 2.0, January 2004\n"
        "                     http://www.apache.org/licenses/\n"

        "TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION\n"

        "1. Definitions.\n"

        "   \"License\" shall mean the terms and conditions for use, reproduction,\n"
        "   and distribution as defined by Sections 1 through 9 of this document.\n"

        "   \"Licensor\" shall mean the copyright owner or entity authorized by\n"
        "   the copyright owner that is granting the License.\n"

        "   \"Legal Entity\" shall mean the union of the acting entity and all\n"
        "   other entities that control, are controlled by, or are under common\n"
        "   control with that entity. For the purposes of this definition,\n"
        "   \"control\" means (i) the power, direct or indirect, to cause the\n"
        "   direction or management of such entity, whether by contract or\n"
        "   otherwise, or (ii) ownership of fifty percent (50%%) or more of the\n"
        "   outstanding shares, or (iii) beneficial ownership of such entity.\n"

        "   \"You\" (or \"Your\") shall mean an individual or Legal Entity\n"
        "   exercising permissions granted by this License.\n"

        "   \"Source\" form shall mean the preferred form for making modifications,\n"
        "   including but not limited to software source code, documentation\n"
        "   source, and configuration files.\n"

        "   \"Object\" form shall mean any form resulting from mechanical\n"
        "   transformation or translation of a Source form, including but\n"
        "   not limited to compiled object code, generated documentation,\n"
        "   and conversions to other media types.\n"

        "   \"Work\" shall mean the work of authorship, whether in Source or\n"
        "   Object form, made available under the License, as indicated by a\n"
        "   copyright notice that is included in or attached to the work\n"
        "   (an example is provided in the Appendix below).\n"

        "   \"Derivative Works\" shall mean any work, whether in Source or Object\n"
        "   form, that is based on (or derived from) the Work and for which the\n"
        "   editorial revisions, annotations, elaborations, or other modifications\n"
        "   represent, as a whole, an original work of authorship. For the purposes\n"
        "   of this License, Derivative Works shall not include works that remain\n"
        "   separable from, or merely link (or bind by name) to the interfaces of,\n"
        "   the Work and Derivative Works thereof.\n"

        "   \"Contribution\" shall mean any work of authorship, including\n"
        "   the original version of the Work and any modifications or additions\n"
        "   to that Work or Derivative Works thereof, that is intentionally\n"
        "   submitted to Licensor for inclusion in the Work by the copyright owner\n"
        "   or by an individual or Legal Entity authorized to submit on behalf of\n"
        "   the copyright owner. For the purposes of this definition, \"submitted\"\n"
        "   means any form of electronic, verbal, or written communication sent\n"
        "   to the Licensor or its representatives, including but not limited to\n"
        "   communication on electronic mailing lists, source code control systems,\n"
        "   and issue tracking systems that are managed by, or on behalf of, the\n"
        "   Licensor for the purpose of discussing and improving the Work, but\n"
        "   excluding communication that is conspicuously marked or otherwise\n"
        "   designated in writing by the copyright owner as \"Not a Contribution.\"\n"

        "   \"Contributor\" shall mean Licensor and any individual or Legal Entity\n"
        "   on behalf of whom a Contribution has been received by Licensor and\n"
        "   subsequently incorporated within the Work.\n"

        "2. Grant of Copyright License. Subject to the terms and conditions of\n"
        "   this License, each Contributor hereby grants to You a perpetual,\n"
        "   worldwide, non-exclusive, no-charge, royalty-free, irrevocable\n"
        "   copyright license to reproduce, prepare Derivative Works of,\n"
        "   publicly display, publicly perform, sublicense, and distribute the\n"
        "   Work and such Derivative Works in Source or Object form.\n"

        "3. Grant of Patent License. Subject to the terms and conditions of\n"
        "   this License, each Contributor hereby grants to You a perpetual,\n"
        "   worldwide, non-exclusive, no-charge, royalty-free, irrevocable\n"
        "   (except as stated in this section) patent license to make, have made,\n"
        "   use, offer to sell, sell, import, and otherwise transfer the Work,\n"
        "   where such license applies only to those patent claims licensable\n"
        "   by such Contributor that are necessarily infringed by their\n"
        "   Contribution(s) alone or by combination of their Contribution(s)\n"
        "   with the Work to which such Contribution(s) was submitted. If You\n"
        "   institute patent litigation against any entity (including a\n"
        "   cross-claim or counterclaim in a lawsuit) alleging that the Work\n"
        "   or a Contribution incorporated within the Work constitutes direct\n"
        "   or contributory patent infringement, then any patent licenses\n"
        "   granted to You under this License for that Work shall terminate\n"
        "   as of the date such litigation is filed.\n"

        "4. Redistribution. You may reproduce and distribute copies of the\n"
        "   Work or Derivative Works thereof in any medium, with or without\n"
        "   modifications, and in Source or Object form, provided that You\n"
        "   meet the following conditions:\n"

        "   (a) You must give any other recipients of the Work or\n"
        "       Derivative Works a copy of this License; and\n"

        "   (b) You must cause any modified files to carry prominent notices\n"
        "       stating that You changed the files; and\n"

        "   (c) You must retain, in the Source form of any Derivative Works\n"
        "       that You distribute, all copyright, patent, trademark, and\n"
        "       attribution notices from the Source form of the Work,\n"
        "       excluding those notices that do not pertain to any part of\n"
        "       the Derivative Works; and\n"

        "   (d) If the Work includes a \"NOTICE\" text file as part of its\n"
        "       distribution, then any Derivative Works that You distribute must\n"
        "       include a readable copy of the attribution notices contained\n"
        "       within such NOTICE file, excluding those notices that do not\n"
        "       pertain to any part of the Derivative Works, in at least one\n"
        "       of the following places: within a NOTICE text file distributed\n"
        "       as part of the Derivative Works; within the Source form or\n"
        "       documentation, if provided along with the Derivative Works; or,\n"
        "       within a display generated by the Derivative Works, if and\n"
        "       wherever such third-party notices normally appear. The contents\n"
        "       of the NOTICE file are for informational purposes only and\n"
        "       do not modify the License. You may add Your own attribution\n"
        "       notices within Derivative Works that You distribute, alongside\n"
        "       or as an addendum to the NOTICE text from the Work, provided\n"
        "       that such additional attribution notices cannot be construed\n"
        "       as modifying the License.\n"

        "   You may add Your own copyright statement to Your modifications and\n"
        "   may provide additional or different license terms and conditions\n"
        "   for use, reproduction, or distribution of Your modifications, or\n"
        "   for any such Derivative Works as a whole, provided Your use,\n"
        "   reproduction, and distribution of the Work otherwise complies with\n"
        "   the conditions stated in this License.\n"

        "5. Submission of Contributions. Unless You explicitly state otherwise,\n"
        "   any Contribution intentionally submitted for inclusion in the Work\n"
        "   by You to the Licensor shall be under the terms and conditions of\n"
        "   this License, without any additional terms or conditions.\n"
        "   Notwithstanding the above, nothing herein shall supersede or modify\n"
        "   the terms of any separate license agreement you may have executed\n"
        "   with Licensor regarding such Contributions.\n"

        "6. Trademarks. This License does not grant permission to use the trade\n"
        "   names, trademarks, service marks, or product names of the Licensor,\n"
        "   except as required for reasonable and customary use in describing the\n"
        "   origin of the Work and reproducing the content of the NOTICE file.\n"

        "7. Disclaimer of Warranty. Unless required by applicable law or\n"
        "   agreed to in writing, Licensor provides the Work (and each\n"
        "   Contributor provides its Contributions) on an \"AS IS\" BASIS,\n"
        "   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or\n"
        "   implied, including, without limitation, any warranties or conditions\n"
        "   of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A\n"
        "   PARTICULAR PURPOSE. You are solely responsible for determining the\n"
        "   appropriateness of using or redistributing the Work and assume any\n"
        "   risks associated with Your exercise of permissions under this License.\n"

        "8. Limitation of Liability. In no event and under no legal theory,\n"
        "   whether in tort (including negligence), contract, or otherwise,\n"
        "   unless required by applicable law (such as deliberate and grossly\n"
        "   negligent acts) or agreed to in writing, shall any Contributor be\n"
        "   liable to You for damages, including any direct, indirect, special,\n"
        "   incidental, or consequential damages of any character arising as a\n"
        "   result of this License or out of the use or inability to use the\n"
        "   Work (including but not limited to damages for loss of goodwill,\n"
        "   work stoppage, computer failure or malfunction, or any and all\n"
        "   other commercial damages or losses), even if such Contributor\n"
        "   has been advised of the possibility of such damages.\n"

        "9. Accepting Warranty or Additional Liability. While redistributing\n"
        "   the Work or Derivative Works thereof, You may choose to offer,\n"
        "   and charge a fee for, acceptance of support, warranty, indemnity,\n"
        "   or other liability obligations and/or rights consistent with this\n"
        "   License. However, in accepting such obligations, You may act only\n"
        "   on Your own behalf and on Your sole responsibility, not on behalf\n"
        "   of any other Contributor, and only if You agree to indemnify,\n"
        "   defend, and hold each Contributor harmless for any liability\n"
        "   incurred by, or claims asserted against, such Contributor by reason\n"
        "   of your accepting any such warranty or additional liability.\n"

        "END OF TERMS AND CONDITIONS\n"

        "APPENDIX: How to apply the Apache License to your work.\n"

        "   To apply the Apache License to your work, attach the following\n"
        "   boilerplate notice, with the fields enclosed by brackets \"[]\"\n"
        "   replaced with your own identifying information. (Don't include\n"
        "   the brackets!)  The text should be enclosed in the appropriate\n"
        "   comment syntax for the file format. We also recommend that a\n"
        "   file or class name and description of purpose be included on the\n"
        "   same \"printed page\" as the copyright notice for easier\n"
        "   identification within third-party archives.\n"

        "Copyright [yyyy] [name of copyright owner]\n"

        "Licensed under the Apache License, Version 2.0 (the \"License\");\n"
        "you may not use this file except in compliance with the License.\n"
        "You may obtain a copy of the License at\n"

        "    http://www.apache.org/licenses/LICENSE-2.0\n"

        "Unless required by applicable law or agreed to in writing, software\n"
        "distributed under the License is distributed on an \"AS IS\" BASIS,\n"
        "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
        "See the License for the specific language governing permissions and\n"
        "limitations under the License.\n"
        );

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(expect, result, strlen(expect)) == 0);

        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_gpl_v3_general_licence(){

        size_t size = 40000;
        char expect[size];

        char *result = testrun_copyright_gpl_v3_general_licence();

        snprintf(expect, size,
        "            GNU GENERAL PUBLIC LICENSE\n"
        "               Version 3, 29 June 2007\n"
        "\n"
        " Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>\n"
        " Everyone is permitted to copy and distribute verbatim copies\n"
        " of this license document, but changing it is not allowed.\n"
        "\n"
        "                            Preamble\n"
        "\n"
        "  The GNU General Public License is a free, copyleft license for\n"
        "software and other kinds of works.\n"
        "\n"
        "  The licenses for most software and other practical works are designed\n"
        "to take away your freedom to share and change the works.  By contrast,\n"
        "the GNU General Public License is intended to guarantee your freedom to\n"
        "share and change all versions of a program--to make sure it remains free\n"
        "software for all its users.  We, the Free Software Foundation, use the\n"
        "GNU General Public License for most of our software; it applies also to\n"
        "any other work released this way by its authors.  You can apply it to\n"
        "your programs, too.\n"
        "\n"
        "  When we speak of free software, we are referring to freedom, not\n"
        "price.  Our General Public Licenses are designed to make sure that you\n"
        "have the freedom to distribute copies of free software (and charge for\n"
        "them if you wish), that you receive source code or can get it if you\n"
        "want it, that you can change the software or use pieces of it in new\n"
        "free programs, and that you know you can do these things.\n"
        "\n"
        "  To protect your rights, we need to prevent others from denying you\n"
        "these rights or asking you to surrender the rights.  Therefore, you have\n"
        "certain responsibilities if you distribute copies of the software, or if\n"
        "you modify it: responsibilities to respect the freedom of others.\n"
        "\n"
        "  For example, if you distribute copies of such a program, whether\n"
        "gratis or for a fee, you must pass on to the recipients the same\n"
        "freedoms that you received.  You must make sure that they, too, receive\n"
        "or can get the source code.  And you must show them these terms so they\n"
        "know their rights.\n"
        "\n"
        "  Developers that use the GNU GPL protect your rights with two steps:\n"
        "(1) assert copyright on the software, and (2) offer you this License\n"
        "giving you legal permission to copy, distribute and/or modify it.\n"
        "\n"
        "  For the developers' and authors' protection, the GPL clearly explains\n"
        "that there is no warranty for this free software.  For both users' and\n"
        "authors' sake, the GPL requires that modified versions be marked as\n"
        "changed, so that their problems will not be attributed erroneously to\n"
        "authors of previous versions.\n"
        "\n"
        "  Some devices are designed to deny users access to install or run\n"
        "modified versions of the software inside them, although the manufacturer\n"
        "can do so.  This is fundamentally incompatible with the aim of\n"
        "protecting users' freedom to change the software.  The systematic\n"
        "pattern of such abuse occurs in the area of products for individuals to\n"
        "use, which is precisely where it is most unacceptable.  Therefore, we\n"
        "have designed this version of the GPL to prohibit the practice for those\n"
        "products.  If such problems arise substantially in other domains, we\n"
        "stand ready to extend this provision to those domains in future versions\n"
        "of the GPL, as needed to protect the freedom of users.\n"
        "\n"
        "  Finally, every program is threatened constantly by software patents.\n"
        "States should not allow patents to restrict development and use of\n"
        "software on general-purpose computers, but in those that do, we wish to\n"
        "avoid the special danger that patents applied to a free program could\n"
        "make it effectively proprietary.  To prevent this, the GPL assures that\n"
        "patents cannot be used to render the program non-free.\n"
        "\n"
        "  The precise terms and conditions for copying, distribution and\n"
        "modification follow.\n"
        "\n"
        "                       TERMS AND CONDITIONS\n"
        "\n"
        "  0. Definitions.\n"
        "\n"
        "  \"This License\" refers to version 3 of the GNU General Public License.\n"
        "\n"
        "  \"Copyright\" also means copyright-like laws that apply to other kinds of\n"
        "works, such as semiconductor masks.\n"
        "\n"
        "  \"The Program\" refers to any copyrightable work licensed under this\n"
        "License.  Each licensee is addressed as \"you\".  \"Licensees\" and\n"
        "\"recipients\" may be individuals or organizations.\n"
        "\n"
        "  To \"modify\" a work means to copy from or adapt all or part of the work\n"
        "in a fashion requiring copyright permission, other than the making of an\n"
        "exact copy.  The resulting work is called a \"modified version\" of the\n"
        "earlier work or a work \"based on\" the earlier work.\n"
        "\n"
        "  A \"covered work\" means either the unmodified Program or a work based\n"
        "on the Program.\n"
        "\n"
        "  To \"propagate\" a work means to do anything with it that, without\n"
        "permission, would make you directly or secondarily liable for\n"
        "infringement under applicable copyright law, except executing it on a\n"
        "computer or modifying a private copy.  Propagation includes copying,\n"
        "distribution (with or without modification), making available to the\n"
        "public, and in some countries other activities as well.\n"
        "\n"
        "  To \"convey\" a work means any kind of propagation that enables other\n"
        "parties to make or receive copies.  Mere interaction with a user through\n"
        "a computer network, with no transfer of a copy, is not conveying.\n"
        "\n"
        "  An interactive user interface displays \"Appropriate Legal Notices\"\n"
        "to the extent that it includes a convenient and prominently visible\n"
        "feature that (1) displays an appropriate copyright notice, and (2)\n"
        "tells the user that there is no warranty for the work (except to the\n"
        "extent that warranties are provided), that licensees may convey the\n"
        "work under this License, and how to view a copy of this License.  If\n"
        "the interface presents a list of user commands or options, such as a\n"
        "menu, a prominent item in the list meets this criterion.\n"
        "\n"
        "  1. Source Code.\n"
        "\n"
        "  The \"source code\" for a work means the preferred form of the work\n"
        "for making modifications to it.  \"Object code\" means any non-source\n"
        "form of a work.\n"
        "\n"
        "  A \"Standard Interface\" means an interface that either is an official\n"
        "standard defined by a recognized standards body, or, in the case of\n"
        "interfaces specified for a particular programming language, one that\n"
        "is widely used among developers working in that language.\n"
        "\n"
        "  The \"System Libraries\" of an executable work include anything, other\n"
        "than the work as a whole, that (a) is included in the normal form of\n"
        "packaging a Major Component, but which is not part of that Major\n"
        "Component, and (b) serves only to enable use of the work with that\n"
        "Major Component, or to implement a Standard Interface for which an\n"
        "implementation is available to the public in source code form.  A\n"
        "\"Major Component\", in this context, means a major essential component\n"
        "(kernel, window system, and so on) of the specific operating system\n"
        "(if any) on which the executable work runs, or a compiler used to\n"
        "produce the work, or an object code interpreter used to run it.\n"
        "\n"
        "  The \"Corresponding Source\" for a work in object code form means all\n"
        "the source code needed to generate, install, and (for an executable\n"
        "work) run the object code and to modify the work, including scripts to\n"
        "control those activities.  However, it does not include the work's\n"
        "System Libraries, or general-purpose tools or generally available free\n"
        "programs which are used unmodified in performing those activities but\n"
        "which are not part of the work.  For example, Corresponding Source\n"
        "includes interface definition files associated with source files for\n"
        "the work, and the source code for shared libraries and dynamically\n"
        "linked subprograms that the work is specifically designed to require,\n"
        "such as by intimate data communication or control flow between those\n"
        "subprograms and other parts of the work.\n"
        "\n"
        "  The Corresponding Source need not include anything that users\n"
        "can regenerate automatically from other parts of the Corresponding\n"
        "Source.\n"
        "\n"
        "  The Corresponding Source for a work in source code form is that\n"
        "same work.\n"
        "\n"
        "  2. Basic Permissions.\n"
        "\n"
        "  All rights granted under this License are granted for the term of\n"
        "copyright on the Program, and are irrevocable provided the stated\n"
        "conditions are met.  This License explicitly affirms your unlimited\n"
        "permission to run the unmodified Program.  The output from running a\n"
        "covered work is covered by this License only if the output, given its\n"
        "content, constitutes a covered work.  This License acknowledges your\n"
        "rights of fair use or other equivalent, as provided by copyright law.\n"
        "\n"
        "  You may make, run and propagate covered works that you do not\n"
        "convey, without conditions so long as your license otherwise remains\n"
        "in force.  You may convey covered works to others for the sole purpose\n"
        "of having them make modifications exclusively for you, or provide you\n"
        "with facilities for running those works, provided that you comply with\n"
        "the terms of this License in conveying all material for which you do\n"
        "not control copyright.  Those thus making or running the covered works\n"
        "for you must do so exclusively on your behalf, under your direction\n"
        "and control, on terms that prohibit them from making any copies of\n"
        "your copyrighted material outside their relationship with you.\n"
        "\n"
        "  Conveying under any other circumstances is permitted solely under\n"
        "the conditions stated below.  Sublicensing is not allowed; section 10\n"
        "makes it unnecessary.\n"
        "\n"
        "  3. Protecting Users' Legal Rights From Anti-Circumvention Law.\n"
        "\n"
        "  No covered work shall be deemed part of an effective technological\n"
        "measure under any applicable law fulfilling obligations under article\n"
        "11 of the WIPO copyright treaty adopted on 20 December 1996, or\n"
        "similar laws prohibiting or restricting circumvention of such\n"
        "measures.\n"
        "\n"
        "  When you convey a covered work, you waive any legal power to forbid\n"
        "circumvention of technological measures to the extent such circumvention\n"
        "is effected by exercising rights under this License with respect to\n"
        "the covered work, and you disclaim any intention to limit operation or\n"
        "modification of the work as a means of enforcing, against the work's\n"
        "users, your or third parties' legal rights to forbid circumvention of\n"
        "technological measures.\n"
        "\n"
        "  4. Conveying Verbatim Copies.\n"
        "\n"
        "  You may convey verbatim copies of the Program's source code as you\n"
        "receive it, in any medium, provided that you conspicuously and\n"
        "appropriately publish on each copy an appropriate copyright notice;\n"
        "keep intact all notices stating that this License and any\n"
        "non-permissive terms added in accord with section 7 apply to the code;\n"
        "keep intact all notices of the absence of any warranty; and give all\n"
        "recipients a copy of this License along with the Program.\n"
        "\n"
        "  You may charge any price or no price for each copy that you convey,\n"
        "and you may offer support or warranty protection for a fee.\n"
        "\n"
        "  5. Conveying Modified Source Versions.\n"
        "\n"
        "  You may convey a work based on the Program, or the modifications to\n"
        "produce it from the Program, in the form of source code under the\n"
        "terms of section 4, provided that you also meet all of these conditions:\n"
        "\n"
        "    a) The work must carry prominent notices stating that you modified\n"
        "    it, and giving a relevant date.\n"
        "\n"
        "    b) The work must carry prominent notices stating that it is\n"
        "    released under this License and any conditions added under section\n"
        "    7.  This requirement modifies the requirement in section 4 to\n"
        "    \"keep intact all notices\".\n"
        "\n"
        "    c) You must license the entire work, as a whole, under this\n"
        "    License to anyone who comes into possession of a copy.  This\n"
        "    License will therefore apply, along with any applicable section 7\n"
        "    additional terms, to the whole of the work, and all its parts,\n"
        "    regardless of how they are packaged.  This License gives no\n"
        "    permission to license the work in any other way, but it does not\n"
        "    invalidate such permission if you have separately received it.\n"
        "\n"
        "    d) If the work has interactive user interfaces, each must display\n"
        "    Appropriate Legal Notices; however, if the Program has interactive\n"
        "    interfaces that do not display Appropriate Legal Notices, your\n"
        "    work need not make them do so.\n"
        "\n"
        "  A compilation of a covered work with other separate and independent\n"
        "works, which are not by their nature extensions of the covered work,\n"
        "and which are not combined with it such as to form a larger program,\n"
        "in or on a volume of a storage or distribution medium, is called an\n"
        "\"aggregate\" if the compilation and its resulting copyright are not\n"
        "used to limit the access or legal rights of the compilation's users\n"
        "beyond what the individual works permit.  Inclusion of a covered work\n"
        "in an aggregate does not cause this License to apply to the other\n"
        "parts of the aggregate.\n"
        "\n"
        "  6. Conveying Non-Source Forms.\n"
        "\n"
        "  You may convey a covered work in object code form under the terms\n"
        "of sections 4 and 5, provided that you also convey the\n"
        "machine-readable Corresponding Source under the terms of this License,\n"
        "in one of these ways:\n"
        "\n"
        "    a) Convey the object code in, or embodied in, a physical product\n"
        "    (including a physical distribution medium), accompanied by the\n"
        "    Corresponding Source fixed on a durable physical medium\n"
        "    customarily used for software interchange.\n"
        "\n"
        "    b) Convey the object code in, or embodied in, a physical product\n"
        "    (including a physical distribution medium), accompanied by a\n"
        "    written offer, valid for at least three years and valid for as\n"
        "    long as you offer spare parts or customer support for that product\n"
        "    model, to give anyone who possesses the object code either (1) a\n"
        "    copy of the Corresponding Source for all the software in the\n"
        "    product that is covered by this License, on a durable physical\n"
        "    medium customarily used for software interchange, for a price no\n"
        "    more than your reasonable cost of physically performing this\n"
        "    conveying of source, or (2) access to copy the\n"
        "    Corresponding Source from a network server at no charge.\n"
        "\n"
        "    c) Convey individual copies of the object code with a copy of the\n"
        "    written offer to provide the Corresponding Source.  This\n"
        "    alternative is allowed only occasionally and noncommercially, and\n"
        "    only if you received the object code with such an offer, in accord\n"
        "    with subsection 6b.\n"
        "\n"
        "    d) Convey the object code by offering access from a designated\n"
        "    place (gratis or for a charge), and offer equivalent access to the\n"
        "    Corresponding Source in the same way through the same place at no\n"
        "    further charge.  You need not require recipients to copy the\n"
        "    Corresponding Source along with the object code.  If the place to\n"
        "    copy the object code is a network server, the Corresponding Source\n"
        "    may be on a different server (operated by you or a third party)\n"
        "    that supports equivalent copying facilities, provided you maintain\n"
        "    clear directions next to the object code saying where to find the\n"
        "    Corresponding Source.  Regardless of what server hosts the\n"
        "    Corresponding Source, you remain obligated to ensure that it is\n"
        "    available for as long as needed to satisfy these requirements.\n"
        "\n"
        "    e) Convey the object code using peer-to-peer transmission, provided\n"
        "    you inform other peers where the object code and Corresponding\n"
        "    Source of the work are being offered to the general public at no\n"
        "    charge under subsection 6d.\n"
        "\n"
        "  A separable portion of the object code, whose source code is excluded\n"
        "from the Corresponding Source as a System Library, need not be\n"
        "included in conveying the object code work.\n"
        "\n"
        "  A \"User Product\" is either (1) a \"consumer product\", which means any\n"
        "tangible personal property which is normally used for personal, family,\n"
        "or household purposes, or (2) anything designed or sold for incorporation\n"
        "into a dwelling.  In determining whether a product is a consumer product,\n"
        "doubtful cases shall be resolved in favor of coverage.  For a particular\n"
        "product received by a particular user, \"normally used\" refers to a\n"
        "typical or common use of that class of product, regardless of the status\n"
        "of the particular user or of the way in which the particular user\n"
        "actually uses, or expects or is expected to use, the product.  A product\n"
        "is a consumer product regardless of whether the product has substantial\n"
        "commercial, industrial or non-consumer uses, unless such uses represent\n"
        "the only significant mode of use of the product.\n"
        "\n"
        "  \"Installation Information\" for a User Product means any methods,\n"
        "procedures, authorization keys, or other information required to install\n"
        "and execute modified versions of a covered work in that User Product from\n"
        "a modified version of its Corresponding Source.  The information must\n"
        "suffice to ensure that the continued functioning of the modified object\n"
        "code is in no case prevented or interfered with solely because\n"
        "modification has been made.\n"
        "\n"
        "  If you convey an object code work under this section in, or with, or\n"
        "specifically for use in, a User Product, and the conveying occurs as\n"
        "part of a transaction in which the right of possession and use of the\n"
        "User Product is transferred to the recipient in perpetuity or for a\n"
        "fixed term (regardless of how the transaction is characterized), the\n"
        "Corresponding Source conveyed under this section must be accompanied\n"
        "by the Installation Information.  But this requirement does not apply\n"
        "if neither you nor any third party retains the ability to install\n"
        "modified object code on the User Product (for example, the work has\n"
        "been installed in ROM).\n"
        "\n"
        "  The requirement to provide Installation Information does not include a\n"
        "requirement to continue to provide support service, warranty, or updates\n"
        "for a work that has been modified or installed by the recipient, or for\n"
        "the User Product in which it has been modified or installed.  Access to a\n"
        "network may be denied when the modification itself materially and\n"
        "adversely affects the operation of the network or violates the rules and\n"
        "protocols for communication across the network.\n"
        "\n"
        "  Corresponding Source conveyed, and Installation Information provided,\n"
        "in accord with this section must be in a format that is publicly\n"
        "documented (and with an implementation available to the public in\n"
        "source code form), and must require no special password or key for\n"
        "unpacking, reading or copying.\n"
        "\n"
        "  7. Additional Terms.\n"
        "\n"
        "  \"Additional permissions\" are terms that supplement the terms of this\n"
        "License by making exceptions from one or more of its conditions.\n"
        "Additional permissions that are applicable to the entire Program shall\n"
        "be treated as though they were included in this License, to the extent\n"
        "that they are valid under applicable law.  If additional permissions\n"
        "apply only to part of the Program, that part may be used separately\n"
        "under those permissions, but the entire Program remains governed by\n"
        "this License without regard to the additional permissions.\n"
        "\n"
        "  When you convey a copy of a covered work, you may at your option\n"
        "remove any additional permissions from that copy, or from any part of\n"
        "it.  (Additional permissions may be written to require their own\n"
        "removal in certain cases when you modify the work.)  You may place\n"
        "additional permissions on material, added by you to a covered work,\n"
        "for which you have or can give appropriate copyright permission.\n"
        "\n"
        "  Notwithstanding any other provision of this License, for material you\n"
        "add to a covered work, you may (if authorized by the copyright holders of\n"
        "that material) supplement the terms of this License with terms:\n"
        "\n"
        "    a) Disclaiming warranty or limiting liability differently from the\n"
        "    terms of sections 15 and 16 of this License; or\n"
        "\n"
        "    b) Requiring preservation of specified reasonable legal notices or\n"
        "    author attributions in that material or in the Appropriate Legal\n"
        "    Notices displayed by works containing it; or\n"
        "\n"
        "    c) Prohibiting misrepresentation of the origin of that material, or\n"
        "    requiring that modified versions of such material be marked in\n"
        "    reasonable ways as different from the original version; or\n"
        "\n"
        "    d) Limiting the use for publicity purposes of names of licensors or\n"
        "    authors of the material; or\n"
        "\n"
        "    e) Declining to grant rights under trademark law for use of some\n"
        "    trade names, trademarks, or service marks; or\n"
        "\n"
        "    f) Requiring indemnification of licensors and authors of that\n"
        "    material by anyone who conveys the material (or modified versions of\n"
        "    it) with contractual assumptions of liability to the recipient, for\n"
        "    any liability that these contractual assumptions directly impose on\n"
        "    those licensors and authors.\n"
        "\n"
        "  All other non-permissive additional terms are considered \"further\n"
        "restrictions\" within the meaning of section 10.  If the Program as you\n"
        "received it, or any part of it, contains a notice stating that it is\n"
        "governed by this License along with a term that is a further\n"
        "restriction, you may remove that term.  If a license document contains\n"
        "a further restriction but permits relicensing or conveying under this\n"
        "License, you may add to a covered work material governed by the terms\n"
        "of that license document, provided that the further restriction does\n"
        "not survive such relicensing or conveying.\n"
        "\n"
        "  If you add terms to a covered work in accord with this section, you\n"
        "must place, in the relevant source files, a statement of the\n"
        "additional terms that apply to those files, or a notice indicating\n"
        "where to find the applicable terms.\n"
        "\n"
        "  Additional terms, permissive or non-permissive, may be stated in the\n"
        "form of a separately written license, or stated as exceptions;\n"
        "the above requirements apply either way.\n"
        "\n"
        "  8. Termination.\n"
        "\n"
        "  You may not propagate or modify a covered work except as expressly\n"
        "provided under this License.  Any attempt otherwise to propagate or\n"
        "modify it is void, and will automatically terminate your rights under\n"
        "this License (including any patent licenses granted under the third\n"
        "paragraph of section 11).\n"
        "\n"
        "  However, if you cease all violation of this License, then your\n"
        "license from a particular copyright holder is reinstated (a)\n"
        "provisionally, unless and until the copyright holder explicitly and\n"
        "finally terminates your license, and (b) permanently, if the copyright\n"
        "holder fails to notify you of the violation by some reasonable means\n"
        "prior to 60 days after the cessation.\n"
        "\n"
        "  Moreover, your license from a particular copyright holder is\n"
        "reinstated permanently if the copyright holder notifies you of the\n"
        "violation by some reasonable means, this is the first time you have\n"
        "received notice of violation of this License (for any work) from that\n"
        "copyright holder, and you cure the violation prior to 30 days after\n"
        "your receipt of the notice.\n"
        "\n"
        "  Termination of your rights under this section does not terminate the\n"
        "licenses of parties who have received copies or rights from you under\n"
        "this License.  If your rights have been terminated and not permanently\n"
        "reinstated, you do not qualify to receive new licenses for the same\n"
        "material under section 10.\n"
        "\n"
        "  9. Acceptance Not Required for Having Copies.\n"
        "\n"
        "  You are not required to accept this License in order to receive or\n"
        "run a copy of the Program.  Ancillary propagation of a covered work\n"
        "occurring solely as a consequence of using peer-to-peer transmission\n"
        "to receive a copy likewise does not require acceptance.  However,\n"
        "nothing other than this License grants you permission to propagate or\n"
        "modify any covered work.  These actions infringe copyright if you do\n"
        "not accept this License.  Therefore, by modifying or propagating a\n"
        "covered work, you indicate your acceptance of this License to do so.\n"
        "\n"
        "  10. Automatic Licensing of Downstream Recipients.\n"
        "\n"
        "  Each time you convey a covered work, the recipient automatically\n"
        "receives a license from the original licensors, to run, modify and\n"
        "propagate that work, subject to this License.  You are not responsible\n"
        "for enforcing compliance by third parties with this License.\n"
        "\n"
        "  An \"entity transaction\" is a transaction transferring control of an\n"
        "organization, or substantially all assets of one, or subdividing an\n"
        "organization, or merging organizations.  If propagation of a covered\n"
        "work results from an entity transaction, each party to that\n"
        "transaction who receives a copy of the work also receives whatever\n"
        "licenses to the work the party's predecessor in interest had or could\n"
        "give under the previous paragraph, plus a right to possession of the\n"
        "Corresponding Source of the work from the predecessor in interest, if\n"
        "the predecessor has it or can get it with reasonable efforts.\n"
        "\n"
        "  You may not impose any further restrictions on the exercise of the\n"
        "rights granted or affirmed under this License.  For example, you may\n"
        "not impose a license fee, royalty, or other charge for exercise of\n"
        "rights granted under this License, and you may not initiate litigation\n"
        "(including a cross-claim or counterclaim in a lawsuit) alleging that\n"
        "any patent claim is infringed by making, using, selling, offering for\n"
        "sale, or importing the Program or any portion of it.\n"
        "\n"
        "  11. Patents.\n"
        "\n"
        "  A \"contributor\" is a copyright holder who authorizes use under this\n"
        "License of the Program or a work on which the Program is based.  The\n"
        "work thus licensed is called the contributor's \"contributor version\".\n"
        "\n"
        "  A contributor's \"essential patent claims\" are all patent claims\n"
        "owned or controlled by the contributor, whether already acquired or\n"
        "hereafter acquired, that would be infringed by some manner, permitted\n"
        "by this License, of making, using, or selling its contributor version,\n"
        "but do not include claims that would be infringed only as a\n"
        "consequence of further modification of the contributor version.  For\n"
        "purposes of this definition, \"control\" includes the right to grant\n"
        "patent sublicenses in a manner consistent with the requirements of\n"
        "this License.\n"
        "\n"
        "  Each contributor grants you a non-exclusive, worldwide, royalty-free\n"
        "patent license under the contributor's essential patent claims, to\n"
        "make, use, sell, offer for sale, import and otherwise run, modify and\n"
        "propagate the contents of its contributor version.\n"
        "\n"
        "  In the following three paragraphs, a \"patent license\" is any express\n"
        "agreement or commitment, however denominated, not to enforce a patent\n"
        "(such as an express permission to practice a patent or covenant not to\n"
        "sue for patent infringement).  To \"grant\" such a patent license to a\n"
        "party means to make such an agreement or commitment not to enforce a\n"
        "patent against the party.\n"
        "\n"
        "  If you convey a covered work, knowingly relying on a patent license,\n"
        "and the Corresponding Source of the work is not available for anyone\n"
        "to copy, free of charge and under the terms of this License, through a\n"
        "publicly available network server or other readily accessible means,\n"
        "then you must either (1) cause the Corresponding Source to be so\n"
        "available, or (2) arrange to deprive yourself of the benefit of the\n"
        "patent license for this particular work, or (3) arrange, in a manner\n"
        "consistent with the requirements of this License, to extend the patent\n"
        "license to downstream recipients.  \"Knowingly relying\" means you have\n"
        "actual knowledge that, but for the patent license, your conveying the\n"
        "covered work in a country, or your recipient's use of the covered work\n"
        "in a country, would infringe one or more identifiable patents in that\n"
        "country that you have reason to believe are valid.\n"
        "\n"
        "  If, pursuant to or in connection with a single transaction or\n"
        "arrangement, you convey, or propagate by procuring conveyance of, a\n"
        "covered work, and grant a patent license to some of the parties\n"
        "receiving the covered work authorizing them to use, propagate, modify\n"
        "or convey a specific copy of the covered work, then the patent license\n"
        "you grant is automatically extended to all recipients of the covered\n"
        "work and works based on it.\n"
        "\n"
        "  A patent license is \"discriminatory\" if it does not include within\n"
        "the scope of its coverage, prohibits the exercise of, or is\n"
        "conditioned on the non-exercise of one or more of the rights that are\n"
        "specifically granted under this License.  You may not convey a covered\n"
        "work if you are a party to an arrangement with a third party that is\n"
        "in the business of distributing software, under which you make payment\n"
        "to the third party based on the extent of your activity of conveying\n"
        "the work, and under which the third party grants, to any of the\n"
        "parties who would receive the covered work from you, a discriminatory\n"
        "patent license (a) in connection with copies of the covered work\n"
        "conveyed by you (or copies made from those copies), or (b) primarily\n"
        "for and in connection with specific products or compilations that\n"
        "contain the covered work, unless you entered into that arrangement,\n"
        "or that patent license was granted, prior to 28 March 2007.\n"
        "\n"
        "  Nothing in this License shall be construed as excluding or limiting\n"
        "any implied license or other defenses to infringement that may\n"
        "otherwise be available to you under applicable patent law.\n"
        "\n"
        "  12. No Surrender of Others' Freedom.\n"
        "\n"
        "  If conditions are imposed on you (whether by court order, agreement or\n"
        "otherwise) that contradict the conditions of this License, they do not\n"
        "excuse you from the conditions of this License.  If you cannot convey a\n"
        "covered work so as to satisfy simultaneously your obligations under this\n"
        "License and any other pertinent obligations, then as a consequence you may\n"
        "not convey it at all.  For example, if you agree to terms that obligate you\n"
        "to collect a royalty for further conveying from those to whom you convey\n"
        "the Program, the only way you could satisfy both those terms and this\n"
        "License would be to refrain entirely from conveying the Program.\n"
        "\n"
        "  13. Use with the GNU Affero General Public License.\n"
        "\n"
        "  Notwithstanding any other provision of this License, you have\n"
        "permission to link or combine any covered work with a work licensed\n"
        "under version 3 of the GNU Affero General Public License into a single\n"
        "combined work, and to convey the resulting work.  The terms of this\n"
        "License will continue to apply to the part which is the covered work,\n"
        "but the special requirements of the GNU Affero General Public License,\n"
        "section 13, concerning interaction through a network will apply to the\n"
        "combination as such.\n"
        "\n"
        "  14. Revised Versions of this License.\n"
        "\n"
        "  The Free Software Foundation may publish revised and/or new versions of\n"
        "the GNU General Public License from time to time.  Such new versions will\n"
        "be similar in spirit to the present version, but may differ in detail to\n"
        "address new problems or concerns.\n"
        "\n"
        "  Each version is given a distinguishing version number.  If the\n"
        "Program specifies that a certain numbered version of the GNU General\n"
        "Public License \"or any later version\" applies to it, you have the\n"
        "option of following the terms and conditions either of that numbered\n"
        "version or of any later version published by the Free Software\n"
        "Foundation.  If the Program does not specify a version number of the\n"
        "GNU General Public License, you may choose any version ever published\n"
        "by the Free Software Foundation.\n"
        "\n"
        "  If the Program specifies that a proxy can decide which future\n"
        "versions of the GNU General Public License can be used, that proxy's\n"
        "public statement of acceptance of a version permanently authorizes you\n"
        "to choose that version for the Program.\n"
        "\n"
        "  Later license versions may give you additional or different\n"
        "permissions.  However, no additional obligations are imposed on any\n"
        "author or copyright holder as a result of your choosing to follow a\n"
        "later version.\n"
        "\n"
        "  15. Disclaimer of Warranty.\n"
        "\n"
        "  THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\n"
        "APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT\n"
        "HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY\n"
        "OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,\n"
        "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n"
        "PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM\n"
        "IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF\n"
        "ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\n"
        "\n"
        "  16. Limitation of Liability.\n"
        "\n"
        "  IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING\n"
        "WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS\n"
        "THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY\n"
        "GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE\n"
        "USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF\n"
        "DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD\n"
        "PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),\n"
        "EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF\n"
        "SUCH DAMAGES.\n"
        "\n"
        "  17. Interpretation of Sections 15 and 16.\n"
        "\n"
        "  If the disclaimer of warranty and limitation of liability provided\n"
        "above cannot be given local legal effect according to their terms,\n"
        "reviewing courts shall apply local law that most closely approximates\n"
        "an absolute waiver of all civil liability in connection with the\n"
        "Program, unless a warranty or assumption of liability accompanies a\n"
        "copy of the Program in return for a fee.\n"
        "\n"
        "                     END OF TERMS AND CONDITIONS\n"
        "\n"
        "            How to Apply These Terms to Your New Programs\n"
        "\n"
        "  If you develop a new program, and you want it to be of the greatest\n"
        "possible use to the public, the best way to achieve this is to make it\n"
        "free software which everyone can redistribute and change under these terms.\n"
        "\n"
        "  To do so, attach the following notices to the program.  It is safest\n"
        "to attach them to the start of each source file to most effectively\n"
        "state the exclusion of warranty; and each file should have at least\n"
        "the \"copyright\" line and a pointer to where the full notice is found.\n"
        "\n"
        "    <one line to give the program's name and a brief idea of what it does.>\n"
        "    Copyright (C) <year>  <name of author>\n"
        "\n"
        "    This program is free software: you can redistribute it and/or modify\n"
        "    it under the terms of the GNU General Public License as published by\n"
        "    the Free Software Foundation, either version 3 of the License, or\n"
        "    (at your option) any later version.\n"
        "\n"
        "    This program is distributed in the hope that it will be useful,\n"
        "    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
        "    GNU General Public License for more details.\n"
        "\n"
        "    You should have received a copy of the GNU General Public License\n"
        "    along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"
        "\n"
        "Also add information on how to contact you by electronic and paper mail.\n"
        "\n"
        "  If the program does terminal interaction, make it output a short\n"
        "notice like this when it starts in an interactive mode:\n"
        "\n"
        "    <program>  Copyright (C) <year>  <name of author>\n"
        "    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n"
        "    This is free software, and you are welcome to redistribute it\n"
        "    under certain conditions; type `show c' for details.\n"
        "\n"
        "The hypothetical commands `show w' and `show c' should show the appropriate\n"
        "parts of the General Public License.  Of course, your program's commands\n"
        "might be different; for a GUI interface, you would use an \"about box\".\n"
        "\n"
        "  You should also get your employer (if you work as a programmer) or school,\n"
        "if any, to sign a \"copyright disclaimer\" for the program, if necessary.\n"
        "For more information on this, and how to apply and follow the GNU GPL, see\n"
        "<https://www.gnu.org/licenses/>.\n"
        "\n"
        "  The GNU General Public License does not permit incorporating your program\n"
        "into proprietary programs.  If your program is a subroutine library, you\n"
        "may consider it more useful to permit linking proprietary applications with\n"
        "the library.  If this is what you want to do, use the GNU Lesser General\n"
        "Public License instead of this License.  But first, please read\n"
        "<https://www.gnu.org/licenses/why-not-lgpl.html>.\n"
        "\n");

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(expect, result, strlen(expect)) == 0);

        result = testrun_string_free(result);
        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int test_testrun_copyright_gpl_v3_lesser_licence(){

        size_t size = 10000;
        char expect[size];

        char *result = testrun_copyright_gpl_v3_lesser_licence();

        snprintf(expect, size,
        "                   GNU LESSER GENERAL PUBLIC LICENSE\n"
        "                       Version 3, 29 June 2007\n"
        "\n"
        " Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>\n"
        " Everyone is permitted to copy and distribute verbatim copies\n"
        " of this license document, but changing it is not allowed.\n"
        "\n"
        "\n"
        "  This version of the GNU Lesser General Public License incorporates\n"
        "the terms and conditions of version 3 of the GNU General Public\n"
        "License, supplemented by the additional permissions listed below.\n"
        "\n"
        "  0. Additional Definitions.\n"
        "\n"
        "  As used herein, \"this License\" refers to version 3 of the GNU Lesser\n"
        "General Public License, and the \"GNU GPL\" refers to version 3 of the GNU\n"
        "General Public License.\n"
        "\n"
        "  \"The Library\" refers to a covered work governed by this License,\n"
        "other than an Application or a Combined Work as defined below.\n"
        "\n"
        "  An \"Application\" is any work that makes use of an interface provided\n"
        "by the Library, but which is not otherwise based on the Library.\n"
        "Defining a subclass of a class defined by the Library is deemed a mode\n"
        "of using an interface provided by the Library.\n"
        "\n"
        "  A \"Combined Work\" is a work produced by combining or linking an\n"
        "Application with the Library.  The particular version of the Library\n"
        "with which the Combined Work was made is also called the \"Linked\n"
        "Version\".\n"
        "\n"
        "  The \"Minimal Corresponding Source\" for a Combined Work means the\n"
        "Corresponding Source for the Combined Work, excluding any source code\n"
        "for portions of the Combined Work that, considered in isolation, are\n"
        "based on the Application, and not on the Linked Version.\n"
        "\n"
        "  The \"Corresponding Application Code\" for a Combined Work means the\n"
        "object code and/or source code for the Application, including any data\n"
        "and utility programs needed for reproducing the Combined Work from the\n"
        "Application, but excluding the System Libraries of the Combined Work.\n"
        "\n"
        "  1. Exception to Section 3 of the GNU GPL.\n"
        "\n"
        "  You may convey a covered work under sections 3 and 4 of this License\n"
        "without being bound by section 3 of the GNU GPL.\n"
        "\n"
        "  2. Conveying Modified Versions.\n"
        "\n"
        "  If you modify a copy of the Library, and, in your modifications, a\n"
        "facility refers to a function or data to be supplied by an Application\n"
        "that uses the facility (other than as an argument passed when the\n"
        "facility is invoked), then you may convey a copy of the modified\n"
        "version:\n"
        "\n"
        "   a) under this License, provided that you make a good faith effort to\n"
        "   ensure that, in the event an Application does not supply the\n"
        "   function or data, the facility still operates, and performs\n"
        "   whatever part of its purpose remains meaningful, or\n"
        "\n"
        "   b) under the GNU GPL, with none of the additional permissions of\n"
        "   this License applicable to that copy.\n"
        "\n"
        "  3. Object Code Incorporating Material from Library Header Files.\n"
        "\n"
        "  The object code form of an Application may incorporate material from\n"
        "a header file that is part of the Library.  You may convey such object\n"
        "code under terms of your choice, provided that, if the incorporated\n"
        "material is not limited to numerical parameters, data structure\n"
        "layouts and accessors, or small macros, inline functions and templates\n"
        "(ten or fewer lines in length), you do both of the following:\n"
        "\n"
        "   a) Give prominent notice with each copy of the object code that the\n"
        "   Library is used in it and that the Library and its use are\n"
        "   covered by this License.\n"
        "\n"
        "   b) Accompany the object code with a copy of the GNU GPL and this license\n"
        "   document.\n"
        "\n"
        "  4. Combined Works.\n"
        "\n"
        "  You may convey a Combined Work under terms of your choice that,\n"
        "taken together, effectively do not restrict modification of the\n"
        "portions of the Library contained in the Combined Work and reverse\n"
        "engineering for debugging such modifications, if you also do each of\n"
        "the following:\n"
        "\n"
        "   a) Give prominent notice with each copy of the Combined Work that\n"
        "   the Library is used in it and that the Library and its use are\n"
        "   covered by this License.\n"
        "\n"
        "   b) Accompany the Combined Work with a copy of the GNU GPL and this license\n"
        "   document.\n"
        "\n"
        "   c) For a Combined Work that displays copyright notices during\n"
        "   execution, include the copyright notice for the Library among\n"
        "   these notices, as well as a reference directing the user to the\n"
        "   copies of the GNU GPL and this license document.\n"
        "\n"
        "   d) Do one of the following:\n"
        "\n"
        "       0) Convey the Minimal Corresponding Source under the terms of this\n"
        "       License, and the Corresponding Application Code in a form\n"
        "       suitable for, and under terms that permit, the user to\n"
        "       recombine or relink the Application with a modified version of\n"
        "       the Linked Version to produce a modified Combined Work, in the\n"
        "       manner specified by section 6 of the GNU GPL for conveying\n"
        "       Corresponding Source.\n"
        "\n"
        "       1) Use a suitable shared library mechanism for linking with the\n"
        "       Library.  A suitable mechanism is one that (a) uses at run time\n"
        "       a copy of the Library already present on the user's computer\n"
        "       system, and (b) will operate properly with a modified version\n"
        "       of the Library that is interface-compatible with the Linked\n"
        "       Version.\n"
        "\n"
        "   e) Provide Installation Information, but only if you would otherwise\n"
        "   be required to provide such information under section 6 of the\n"
        "   GNU GPL, and only to the extent that such information is\n"
        "   necessary to install and execute a modified version of the\n"
        "   Combined Work produced by recombining or relinking the\n"
        "   Application with a modified version of the Linked Version. (If\n"
        "   you use option 4d0, the Installation Information must accompany\n"
        "   the Minimal Corresponding Source and Corresponding Application\n"
        "   Code. If you use option 4d1, you must provide the Installation\n"
        "   Information in the manner specified by section 6 of the GNU GPL\n"
        "   for conveying Corresponding Source.)\n"
        "\n"
        "  5. Combined Libraries.\n"
        "\n"
        "  You may place library facilities that are a work based on the\n"
        "Library side by side in a single library together with other library\n"
        "facilities that are not Applications and are not covered by this\n"
        "License, and convey such a combined library under terms of your\n"
        "choice, if you do both of the following:\n"
        "\n"
        "   a) Accompany the combined library with a copy of the same work based\n"
        "   on the Library, uncombined with any other library facilities,\n"
        "   conveyed under the terms of this License.\n"
        "\n"
        "   b) Give prominent notice with the combined library that part of it\n"
        "   is a work based on the Library, and explaining where to find the\n"
        "   accompanying uncombined form of the same work.\n"
        "\n"
        "  6. Revised Versions of the GNU Lesser General Public License.\n"
        "\n"
        "  The Free Software Foundation may publish revised and/or new versions\n"
        "of the GNU Lesser General Public License from time to time. Such new\n"
        "versions will be similar in spirit to the present version, but may\n"
        "differ in detail to address new problems or concerns.\n"
        "\n"
        "  Each version is given a distinguishing version number. If the\n"
        "Library as you received it specifies that a certain numbered version\n"
        "of the GNU Lesser General Public License \"or any later version\"\n"
        "applies to it, you have the option of following the terms and\n"
        "conditions either of that published version or of any later version\n"
        "published by the Free Software Foundation. If the Library as you\n"
        "received it does not specify a version number of the GNU Lesser\n"
        "General Public License, you may choose any version of the GNU Lesser\n"
        "General Public License ever published by the Free Software Foundation.\n"
        "\n"
        "  If the Library as you received it specifies that a proxy can decide\n"
        "whether future versions of the GNU Lesser General Public License shall\n"
        "apply, that proxy's public statement of acceptance of any version is\n"
        "permanent authorization for you to choose that version for the\n"
        "Library.\n"
        );

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(expect, result, strlen(expect)) == 0);

        result = testrun_string_free(result);
        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int test_testrun_copyright_gpl_v3_affero_licence(){

        size_t size = 40000;
        char expect[size];

        char *result = testrun_copyright_gpl_v3_affero_licence();

        snprintf(expect, size,
        "                    GNU AFFERO GENERAL PUBLIC LICENSE\n"
        "                       Version 3, 19 November 2007\n"
        "\n"
        " Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>\n"
        " Everyone is permitted to copy and distribute verbatim copies\n"
        " of this license document, but changing it is not allowed.\n"
        "\n"
        "                            Preamble\n"
        "\n"
        "  The GNU Affero General Public License is a free, copyleft license for\n"
        "software and other kinds of works, specifically designed to ensure\n"
        "cooperation with the community in the case of network server software.\n"
        "\n"
        "  The licenses for most software and other practical works are designed\n"
        "to take away your freedom to share and change the works.  By contrast,\n"
        "our General Public Licenses are intended to guarantee your freedom to\n"
        "share and change all versions of a program--to make sure it remains free\n"
        "software for all its users.\n"
        "\n"
        "  When we speak of free software, we are referring to freedom, not\n"
        "price.  Our General Public Licenses are designed to make sure that you\n"
        "have the freedom to distribute copies of free software (and charge for\n"
        "them if you wish), that you receive source code or can get it if you\n"
        "want it, that you can change the software or use pieces of it in new\n"
        "free programs, and that you know you can do these things.\n"
        "\n"
        "  Developers that use our General Public Licenses protect your rights\n"
        "with two steps: (1) assert copyright on the software, and (2) offer\n"
        "you this License which gives you legal permission to copy, distribute\n"
        "and/or modify the software.\n"
        "\n"
        "  A secondary benefit of defending all users' freedom is that\n"
        "improvements made in alternate versions of the program, if they\n"
        "receive widespread use, become available for other developers to\n"
        "incorporate.  Many developers of free software are heartened and\n"
        "encouraged by the resulting cooperation.  However, in the case of\n"
        "software used on network servers, this result may fail to come about.\n"
        "The GNU General Public License permits making a modified version and\n"
        "letting the public access it on a server without ever releasing its\n"
        "source code to the public.\n"
        "\n"
        "  The GNU Affero General Public License is designed specifically to\n"
        "ensure that, in such cases, the modified source code becomes available\n"
        "to the community.  It requires the operator of a network server to\n"
        "provide the source code of the modified version running there to the\n"
        "users of that server.  Therefore, public use of a modified version, on\n"
        "a publicly accessible server, gives the public access to the source\n"
        "code of the modified version.\n"
        "\n"
        "  An older license, called the Affero General Public License and\n"
        "published by Affero, was designed to accomplish similar goals.  This is\n"
        "a different license, not a version of the Affero GPL, but Affero has\n"
        "released a new version of the Affero GPL which permits relicensing under\n"
        "this license.\n"
        "\n"
        "  The precise terms and conditions for copying, distribution and\n"
        "modification follow.\n"
        "\n"
        "                       TERMS AND CONDITIONS\n"
        "\n"
        "  0. Definitions.\n"
        "\n"
        "  \"This License\" refers to version 3 of the GNU Affero General Public License.\n"
        "\n"
        "  \"Copyright\" also means copyright-like laws that apply to other kinds of\n"
        "works, such as semiconductor masks.\n"
        "\n"
        "  \"The Program\" refers to any copyrightable work licensed under this\n"
        "License.  Each licensee is addressed as \"you\".  \"Licensees\" and\n"
        "\"recipients\" may be individuals or organizations.\n"
        "\n"
        "  To \"modify\" a work means to copy from or adapt all or part of the work\n"
        "in a fashion requiring copyright permission, other than the making of an\n"
        "exact copy.  The resulting work is called a \"modified version\" of the\n"
        "earlier work or a work \"based on\" the earlier work.\n"
        "\n"
        "  A \"covered work\" means either the unmodified Program or a work based\n"
        "on the Program.\n"
        "\n"
        "  To \"propagate\" a work means to do anything with it that, without\n"
        "permission, would make you directly or secondarily liable for\n"
        "infringement under applicable copyright law, except executing it on a\n"
        "computer or modifying a private copy.  Propagation includes copying,\n"
        "distribution (with or without modification), making available to the\n"
        "public, and in some countries other activities as well.\n"
        "\n"
        "  To \"convey\" a work means any kind of propagation that enables other\n"
        "parties to make or receive copies.  Mere interaction with a user through\n"
        "a computer network, with no transfer of a copy, is not conveying.\n"
        "\n"
        "  An interactive user interface displays \"Appropriate Legal Notices\"\n"
        "to the extent that it includes a convenient and prominently visible\n"
        "feature that (1) displays an appropriate copyright notice, and (2)\n"
        "tells the user that there is no warranty for the work (except to the\n"
        "extent that warranties are provided), that licensees may convey the\n"
        "work under this License, and how to view a copy of this License.  If\n"
        "the interface presents a list of user commands or options, such as a\n"
        "menu, a prominent item in the list meets this criterion.\n"
        "\n"
        "  1. Source Code.\n"
        "\n"
        "  The \"source code\" for a work means the preferred form of the work\n"
        "for making modifications to it.  \"Object code\" means any non-source\n"
        "form of a work.\n"
        "\n"
        "  A \"Standard Interface\" means an interface that either is an official\n"
        "standard defined by a recognized standards body, or, in the case of\n"
        "interfaces specified for a particular programming language, one that\n"
        "is widely used among developers working in that language.\n"
        "\n"
        "  The \"System Libraries\" of an executable work include anything, other\n"
        "than the work as a whole, that (a) is included in the normal form of\n"
        "packaging a Major Component, but which is not part of that Major\n"
        "Component, and (b) serves only to enable use of the work with that\n"
        "Major Component, or to implement a Standard Interface for which an\n"
        "implementation is available to the public in source code form.  A\n"
        "\"Major Component\", in this context, means a major essential component\n"
        "(kernel, window system, and so on) of the specific operating system\n"
        "(if any) on which the executable work runs, or a compiler used to\n"
        "produce the work, or an object code interpreter used to run it.\n"
        "\n"
        "  The \"Corresponding Source\" for a work in object code form means all\n"
        "the source code needed to generate, install, and (for an executable\n"
        "work) run the object code and to modify the work, including scripts to\n"
        "control those activities.  However, it does not include the work's\n"
        "System Libraries, or general-purpose tools or generally available free\n"
        "programs which are used unmodified in performing those activities but\n"
        "which are not part of the work.  For example, Corresponding Source\n"
        "includes interface definition files associated with source files for\n"
        "the work, and the source code for shared libraries and dynamically\n"
        "linked subprograms that the work is specifically designed to require,\n"
        "such as by intimate data communication or control flow between those\n"
        "subprograms and other parts of the work.\n"
        "\n"
        "  The Corresponding Source need not include anything that users\n"
        "can regenerate automatically from other parts of the Corresponding\n"
        "Source.\n"
        "\n"
        "  The Corresponding Source for a work in source code form is that\n"
        "same work.\n"
        "\n"
        "  2. Basic Permissions.\n"
        "\n"
        "  All rights granted under this License are granted for the term of\n"
        "copyright on the Program, and are irrevocable provided the stated\n"
        "conditions are met.  This License explicitly affirms your unlimited\n"
        "permission to run the unmodified Program.  The output from running a\n"
        "covered work is covered by this License only if the output, given its\n"
        "content, constitutes a covered work.  This License acknowledges your\n"
        "rights of fair use or other equivalent, as provided by copyright law.\n"
        "\n"
        "  You may make, run and propagate covered works that you do not\n"
        "convey, without conditions so long as your license otherwise remains\n"
        "in force.  You may convey covered works to others for the sole purpose\n"
        "of having them make modifications exclusively for you, or provide you\n"
        "with facilities for running those works, provided that you comply with\n"
        "the terms of this License in conveying all material for which you do\n"
        "not control copyright.  Those thus making or running the covered works\n"
        "for you must do so exclusively on your behalf, under your direction\n"
        "and control, on terms that prohibit them from making any copies of\n"
        "your copyrighted material outside their relationship with you.\n"
        "\n"
        "  Conveying under any other circumstances is permitted solely under\n"
        "the conditions stated below.  Sublicensing is not allowed; section 10\n"
        "makes it unnecessary.\n"
        "\n"
        "  3. Protecting Users' Legal Rights From Anti-Circumvention Law.\n"
        "\n"
        "  No covered work shall be deemed part of an effective technological\n"
        "measure under any applicable law fulfilling obligations under article\n"
        "11 of the WIPO copyright treaty adopted on 20 December 1996, or\n"
        "similar laws prohibiting or restricting circumvention of such\n"
        "measures.\n"
        "\n"
        "  When you convey a covered work, you waive any legal power to forbid\n"
        "circumvention of technological measures to the extent such circumvention\n"
        "is effected by exercising rights under this License with respect to\n"
        "the covered work, and you disclaim any intention to limit operation or\n"
        "modification of the work as a means of enforcing, against the work's\n"
        "users, your or third parties' legal rights to forbid circumvention of\n"
        "technological measures.\n"
        "\n"
        "  4. Conveying Verbatim Copies.\n"
        "\n"
        "  You may convey verbatim copies of the Program's source code as you\n"
        "receive it, in any medium, provided that you conspicuously and\n"
        "appropriately publish on each copy an appropriate copyright notice;\n"
        "keep intact all notices stating that this License and any\n"
        "non-permissive terms added in accord with section 7 apply to the code;\n"
        "keep intact all notices of the absence of any warranty; and give all\n"
        "recipients a copy of this License along with the Program.\n"
        "\n"
        "  You may charge any price or no price for each copy that you convey,\n"
        "and you may offer support or warranty protection for a fee.\n"
        "\n"
        "  5. Conveying Modified Source Versions.\n"
        "\n"
        "  You may convey a work based on the Program, or the modifications to\n"
        "produce it from the Program, in the form of source code under the\n"
        "terms of section 4, provided that you also meet all of these conditions:\n"
        "\n"
        "    a) The work must carry prominent notices stating that you modified\n"
        "    it, and giving a relevant date.\n"
        "\n"
        "    b) The work must carry prominent notices stating that it is\n"
        "    released under this License and any conditions added under section\n"
        "    7.  This requirement modifies the requirement in section 4 to\n"
        "    \"keep intact all notices\".\n"
        "\n"
        "    c) You must license the entire work, as a whole, under this\n"
        "    License to anyone who comes into possession of a copy.  This\n"
        "    License will therefore apply, along with any applicable section 7\n"
        "    additional terms, to the whole of the work, and all its parts,\n"
        "    regardless of how they are packaged.  This License gives no\n"
        "    permission to license the work in any other way, but it does not\n"
        "    invalidate such permission if you have separately received it.\n"
        "\n"
        "    d) If the work has interactive user interfaces, each must display\n"
        "    Appropriate Legal Notices; however, if the Program has interactive\n"
        "    interfaces that do not display Appropriate Legal Notices, your\n"
        "    work need not make them do so.\n"
        "\n"
        "  A compilation of a covered work with other separate and independent\n"
        "works, which are not by their nature extensions of the covered work,\n"
        "and which are not combined with it such as to form a larger program,\n"
        "in or on a volume of a storage or distribution medium, is called an\n"
        "\"aggregate\" if the compilation and its resulting copyright are not\n"
        "used to limit the access or legal rights of the compilation's users\n"
        "beyond what the individual works permit.  Inclusion of a covered work\n"
        "in an aggregate does not cause this License to apply to the other\n"
        "parts of the aggregate.\n"
        "\n"
        "  6. Conveying Non-Source Forms.\n"
        "\n"
        "  You may convey a covered work in object code form under the terms\n"
        "of sections 4 and 5, provided that you also convey the\n"
        "machine-readable Corresponding Source under the terms of this License,\n"
        "in one of these ways:\n"
        "\n"
        "    a) Convey the object code in, or embodied in, a physical product\n"
        "    (including a physical distribution medium), accompanied by the\n"
        "    Corresponding Source fixed on a durable physical medium\n"
        "    customarily used for software interchange.\n"
        "\n"
        "    b) Convey the object code in, or embodied in, a physical product\n"
        "    (including a physical distribution medium), accompanied by a\n"
        "    written offer, valid for at least three years and valid for as\n"
        "    long as you offer spare parts or customer support for that product\n"
        "    model, to give anyone who possesses the object code either (1) a\n"
        "    copy of the Corresponding Source for all the software in the\n"
        "    product that is covered by this License, on a durable physical\n"
        "    medium customarily used for software interchange, for a price no\n"
        "    more than your reasonable cost of physically performing this\n"
        "    conveying of source, or (2) access to copy the\n"
        "    Corresponding Source from a network server at no charge.\n"
        "\n"
        "    c) Convey individual copies of the object code with a copy of the\n"
        "    written offer to provide the Corresponding Source.  This\n"
        "    alternative is allowed only occasionally and noncommercially, and\n"
        "    only if you received the object code with such an offer, in accord\n"
        "    with subsection 6b.\n"
        "\n"
        "    d) Convey the object code by offering access from a designated\n"
        "    place (gratis or for a charge), and offer equivalent access to the\n"
        "    Corresponding Source in the same way through the same place at no\n"
        "    further charge.  You need not require recipients to copy the\n"
        "    Corresponding Source along with the object code.  If the place to\n"
        "    copy the object code is a network server, the Corresponding Source\n"
        "    may be on a different server (operated by you or a third party)\n"
        "    that supports equivalent copying facilities, provided you maintain\n"
        "    clear directions next to the object code saying where to find the\n"
        "    Corresponding Source.  Regardless of what server hosts the\n"
        "    Corresponding Source, you remain obligated to ensure that it is\n"
        "    available for as long as needed to satisfy these requirements.\n"
        "\n"
        "    e) Convey the object code using peer-to-peer transmission, provided\n"
        "    you inform other peers where the object code and Corresponding\n"
        "    Source of the work are being offered to the general public at no\n"
        "    charge under subsection 6d.\n"
        "\n"
        "  A separable portion of the object code, whose source code is excluded\n"
        "from the Corresponding Source as a System Library, need not be\n"
        "included in conveying the object code work.\n"
        "\n"
        "  A \"User Product\" is either (1) a \"consumer product\", which means any\n"
        "tangible personal property which is normally used for personal, family,\n"
        "or household purposes, or (2) anything designed or sold for incorporation\n"
        "into a dwelling.  In determining whether a product is a consumer product,\n"
        "doubtful cases shall be resolved in favor of coverage.  For a particular\n"
        "product received by a particular user, \"normally used\" refers to a\n"
        "typical or common use of that class of product, regardless of the status\n"
        "of the particular user or of the way in which the particular user\n"
        "actually uses, or expects or is expected to use, the product.  A product\n"
        "is a consumer product regardless of whether the product has substantial\n"
        "commercial, industrial or non-consumer uses, unless such uses represent\n"
        "the only significant mode of use of the product.\n"
        "\n"
        "  \"Installation Information\" for a User Product means any methods,\n"
        "procedures, authorization keys, or other information required to install\n"
        "and execute modified versions of a covered work in that User Product from\n"
        "a modified version of its Corresponding Source.  The information must\n"
        "suffice to ensure that the continued functioning of the modified object\n"
        "code is in no case prevented or interfered with solely because\n"
        "modification has been made.\n"
        "\n"
        "  If you convey an object code work under this section in, or with, or\n"
        "specifically for use in, a User Product, and the conveying occurs as\n"
        "part of a transaction in which the right of possession and use of the\n"
        "User Product is transferred to the recipient in perpetuity or for a\n"
        "fixed term (regardless of how the transaction is characterized), the\n"
        "Corresponding Source conveyed under this section must be accompanied\n"
        "by the Installation Information.  But this requirement does not apply\n"
        "if neither you nor any third party retains the ability to install\n"
        "modified object code on the User Product (for example, the work has\n"
        "been installed in ROM).\n"
        "\n"
        "  The requirement to provide Installation Information does not include a\n"
        "requirement to continue to provide support service, warranty, or updates\n"
        "for a work that has been modified or installed by the recipient, or for\n"
        "the User Product in which it has been modified or installed.  Access to a\n"
        "network may be denied when the modification itself materially and\n"
        "adversely affects the operation of the network or violates the rules and\n"
        "protocols for communication across the network.\n"
        "\n"
        "  Corresponding Source conveyed, and Installation Information provided,\n"
        "in accord with this section must be in a format that is publicly\n"
        "documented (and with an implementation available to the public in\n"
        "source code form), and must require no special password or key for\n"
        "unpacking, reading or copying.\n"
        "\n"
        "  7. Additional Terms.\n"
        "\n"
        "  \"Additional permissions\" are terms that supplement the terms of this\n"
        "License by making exceptions from one or more of its conditions.\n"
        "Additional permissions that are applicable to the entire Program shall\n"
        "be treated as though they were included in this License, to the extent\n"
        "that they are valid under applicable law.  If additional permissions\n"
        "apply only to part of the Program, that part may be used separately\n"
        "under those permissions, but the entire Program remains governed by\n"
        "this License without regard to the additional permissions.\n"
        "\n"
        "  When you convey a copy of a covered work, you may at your option\n"
        "remove any additional permissions from that copy, or from any part of\n"
        "it.  (Additional permissions may be written to require their own\n"
        "removal in certain cases when you modify the work.)  You may place\n"
        "additional permissions on material, added by you to a covered work,\n"
        "for which you have or can give appropriate copyright permission.\n"
        "\n"
        "  Notwithstanding any other provision of this License, for material you\n"
        "add to a covered work, you may (if authorized by the copyright holders of\n"
        "that material) supplement the terms of this License with terms:\n"
        "\n"
        "    a) Disclaiming warranty or limiting liability differently from the\n"
        "    terms of sections 15 and 16 of this License; or\n"
        "\n"
        "    b) Requiring preservation of specified reasonable legal notices or\n"
        "    author attributions in that material or in the Appropriate Legal\n"
        "    Notices displayed by works containing it; or\n"
        "\n"
        "    c) Prohibiting misrepresentation of the origin of that material, or\n"
        "    requiring that modified versions of such material be marked in\n"
        "    reasonable ways as different from the original version; or\n"
        "\n"
        "    d) Limiting the use for publicity purposes of names of licensors or\n"
        "    authors of the material; or\n"
        "\n"
        "    e) Declining to grant rights under trademark law for use of some\n"
        "    trade names, trademarks, or service marks; or\n"
        "\n"
        "    f) Requiring indemnification of licensors and authors of that\n"
        "    material by anyone who conveys the material (or modified versions of\n"
        "    it) with contractual assumptions of liability to the recipient, for\n"
        "    any liability that these contractual assumptions directly impose on\n"
        "    those licensors and authors.\n"
        "\n"
        "  All other non-permissive additional terms are considered \"further\n"
        "restrictions\" within the meaning of section 10.  If the Program as you\n"
        "received it, or any part of it, contains a notice stating that it is\n"
        "governed by this License along with a term that is a further\n"
        "restriction, you may remove that term.  If a license document contains\n"
        "a further restriction but permits relicensing or conveying under this\n"
        "License, you may add to a covered work material governed by the terms\n"
        "of that license document, provided that the further restriction does\n"
        "not survive such relicensing or conveying.\n"
        "\n"
        "  If you add terms to a covered work in accord with this section, you\n"
        "must place, in the relevant source files, a statement of the\n"
        "additional terms that apply to those files, or a notice indicating\n"
        "where to find the applicable terms.\n"
        "\n"
        "  Additional terms, permissive or non-permissive, may be stated in the\n"
        "form of a separately written license, or stated as exceptions;\n"
        "the above requirements apply either way.\n"
        "\n"
        "  8. Termination.\n"
        "\n"
        "  You may not propagate or modify a covered work except as expressly\n"
        "provided under this License.  Any attempt otherwise to propagate or\n"
        "modify it is void, and will automatically terminate your rights under\n"
        "this License (including any patent licenses granted under the third\n"
        "paragraph of section 11).\n"
        "\n"
        "  However, if you cease all violation of this License, then your\n"
        "license from a particular copyright holder is reinstated (a)\n"
        "provisionally, unless and until the copyright holder explicitly and\n"
        "finally terminates your license, and (b) permanently, if the copyright\n"
        "holder fails to notify you of the violation by some reasonable means\n"
        "prior to 60 days after the cessation.\n"
        "\n"
        "  Moreover, your license from a particular copyright holder is\n"
        "reinstated permanently if the copyright holder notifies you of the\n"
        "violation by some reasonable means, this is the first time you have\n"
        "received notice of violation of this License (for any work) from that\n"
        "copyright holder, and you cure the violation prior to 30 days after\n"
        "your receipt of the notice.\n"
        "\n"
        "  Termination of your rights under this section does not terminate the\n"
        "licenses of parties who have received copies or rights from you under\n"
        "this License.  If your rights have been terminated and not permanently\n"
        "reinstated, you do not qualify to receive new licenses for the same\n"
        "material under section 10.\n"
        "\n"
        "  9. Acceptance Not Required for Having Copies.\n"
        "\n"
        "  You are not required to accept this License in order to receive or\n"
        "run a copy of the Program.  Ancillary propagation of a covered work\n"
        "occurring solely as a consequence of using peer-to-peer transmission\n"
        "to receive a copy likewise does not require acceptance.  However,\n"
        "nothing other than this License grants you permission to propagate or\n"
        "modify any covered work.  These actions infringe copyright if you do\n"
        "not accept this License.  Therefore, by modifying or propagating a\n"
        "covered work, you indicate your acceptance of this License to do so.\n"
        "\n"
        "  10. Automatic Licensing of Downstream Recipients.\n"
        "\n"
        "  Each time you convey a covered work, the recipient automatically\n"
        "receives a license from the original licensors, to run, modify and\n"
        "propagate that work, subject to this License.  You are not responsible\n"
        "for enforcing compliance by third parties with this License.\n"
        "\n"
        "  An \"entity transaction\" is a transaction transferring control of an\n"
        "organization, or substantially all assets of one, or subdividing an\n"
        "organization, or merging organizations.  If propagation of a covered\n"
        "work results from an entity transaction, each party to that\n"
        "transaction who receives a copy of the work also receives whatever\n"
        "licenses to the work the party's predecessor in interest had or could\n"
        "give under the previous paragraph, plus a right to possession of the\n"
        "Corresponding Source of the work from the predecessor in interest, if\n"
        "the predecessor has it or can get it with reasonable efforts.\n"
        "\n"
        "  You may not impose any further restrictions on the exercise of the\n"
        "rights granted or affirmed under this License.  For example, you may\n"
        "not impose a license fee, royalty, or other charge for exercise of\n"
        "rights granted under this License, and you may not initiate litigation\n"
        "(including a cross-claim or counterclaim in a lawsuit) alleging that\n"
        "any patent claim is infringed by making, using, selling, offering for\n"
        "sale, or importing the Program or any portion of it.\n"
        "\n"
        "  11. Patents.\n"
        "\n"
        "  A \"contributor\" is a copyright holder who authorizes use under this\n"
        "License of the Program or a work on which the Program is based.  The\n"
        "work thus licensed is called the contributor's \"contributor version\".\n"
        "\n"
        "  A contributor's \"essential patent claims\" are all patent claims\n"
        "owned or controlled by the contributor, whether already acquired or\n"
        "hereafter acquired, that would be infringed by some manner, permitted\n"
        "by this License, of making, using, or selling its contributor version,\n"
        "but do not include claims that would be infringed only as a\n"
        "consequence of further modification of the contributor version.  For\n"
        "purposes of this definition, \"control\" includes the right to grant\n"
        "patent sublicenses in a manner consistent with the requirements of\n"
        "this License.\n"
        "\n"
        "  Each contributor grants you a non-exclusive, worldwide, royalty-free\n"
        "patent license under the contributor's essential patent claims, to\n"
        "make, use, sell, offer for sale, import and otherwise run, modify and\n"
        "propagate the contents of its contributor version.\n"
        "\n"
        "  In the following three paragraphs, a \"patent license\" is any express\n"
        "agreement or commitment, however denominated, not to enforce a patent\n"
        "(such as an express permission to practice a patent or covenant not to\n"
        "sue for patent infringement).  To \"grant\" such a patent license to a\n"
        "party means to make such an agreement or commitment not to enforce a\n"
        "patent against the party.\n"
        "\n"
        "  If you convey a covered work, knowingly relying on a patent license,\n"
        "and the Corresponding Source of the work is not available for anyone\n"
        "to copy, free of charge and under the terms of this License, through a\n"
        "publicly available network server or other readily accessible means,\n"
        "then you must either (1) cause the Corresponding Source to be so\n"
        "available, or (2) arrange to deprive yourself of the benefit of the\n"
        "patent license for this particular work, or (3) arrange, in a manner\n"
        "consistent with the requirements of this License, to extend the patent\n"
        "license to downstream recipients.  \"Knowingly relying\" means you have\n"
        "actual knowledge that, but for the patent license, your conveying the\n"
        "covered work in a country, or your recipient's use of the covered work\n"
        "in a country, would infringe one or more identifiable patents in that\n"
        "country that you have reason to believe are valid.\n"
        "\n"
        "  If, pursuant to or in connection with a single transaction or\n"
        "arrangement, you convey, or propagate by procuring conveyance of, a\n"
        "covered work, and grant a patent license to some of the parties\n"
        "receiving the covered work authorizing them to use, propagate, modify\n"
        "or convey a specific copy of the covered work, then the patent license\n"
        "you grant is automatically extended to all recipients of the covered\n"
        "work and works based on it.\n"
        "\n"
        "  A patent license is \"discriminatory\" if it does not include within\n"
        "the scope of its coverage, prohibits the exercise of, or is\n"
        "conditioned on the non-exercise of one or more of the rights that are\n"
        "specifically granted under this License.  You may not convey a covered\n"
        "work if you are a party to an arrangement with a third party that is\n"
        "in the business of distributing software, under which you make payment\n"
        "to the third party based on the extent of your activity of conveying\n"
        "the work, and under which the third party grants, to any of the\n"
        "parties who would receive the covered work from you, a discriminatory\n"
        "patent license (a) in connection with copies of the covered work\n"
        "conveyed by you (or copies made from those copies), or (b) primarily\n"
        "for and in connection with specific products or compilations that\n"
        "contain the covered work, unless you entered into that arrangement,\n"
        "or that patent license was granted, prior to 28 March 2007.\n"
        "\n"
        "  Nothing in this License shall be construed as excluding or limiting\n"
        "any implied license or other defenses to infringement that may\n"
        "otherwise be available to you under applicable patent law.\n"
        "\n"
        "  12. No Surrender of Others' Freedom.\n"
        "\n"
        "  If conditions are imposed on you (whether by court order, agreement or\n"
        "otherwise) that contradict the conditions of this License, they do not\n"
        "excuse you from the conditions of this License.  If you cannot convey a\n"
        "covered work so as to satisfy simultaneously your obligations under this\n"
        "License and any other pertinent obligations, then as a consequence you may\n"
        "not convey it at all.  For example, if you agree to terms that obligate you\n"
        "to collect a royalty for further conveying from those to whom you convey\n"
        "the Program, the only way you could satisfy both those terms and this\n"
        "License would be to refrain entirely from conveying the Program.\n"
        "\n"
        "  13. Remote Network Interaction; Use with the GNU General Public License.\n"
        "\n"
        "  Notwithstanding any other provision of this License, if you modify the\n"
        "Program, your modified version must prominently offer all users\n"
        "interacting with it remotely through a computer network (if your version\n"
        "supports such interaction) an opportunity to receive the Corresponding\n"
        "Source of your version by providing access to the Corresponding Source\n"
        "from a network server at no charge, through some standard or customary\n"
        "means of facilitating copying of software.  This Corresponding Source\n"
        "shall include the Corresponding Source for any work covered by version 3\n"
        "of the GNU General Public License that is incorporated pursuant to the\n"
        "following paragraph.\n"
        "\n"
        "  Notwithstanding any other provision of this License, you have\n"
        "permission to link or combine any covered work with a work licensed\n"
        "under version 3 of the GNU General Public License into a single\n"
        "combined work, and to convey the resulting work.  The terms of this\n"
        "License will continue to apply to the part which is the covered work,\n"
        "but the work with which it is combined will remain governed by version\n"
        "3 of the GNU General Public License.\n"
        "\n"
        "  14. Revised Versions of this License.\n"
        "\n"
        "  The Free Software Foundation may publish revised and/or new versions of\n"
        "the GNU Affero General Public License from time to time.  Such new versions\n"
        "will be similar in spirit to the present version, but may differ in detail to\n"
        "address new problems or concerns.\n"
        "\n"
        "  Each version is given a distinguishing version number.  If the\n"
        "Program specifies that a certain numbered version of the GNU Affero General\n"
        "Public License \"or any later version\" applies to it, you have the\n"
        "option of following the terms and conditions either of that numbered\n"
        "version or of any later version published by the Free Software\n"
        "Foundation.  If the Program does not specify a version number of the\n"
        "GNU Affero General Public License, you may choose any version ever published\n"
        "by the Free Software Foundation.\n"
        "\n"
        "  If the Program specifies that a proxy can decide which future\n"
        "versions of the GNU Affero General Public License can be used, that proxy's\n"
        "public statement of acceptance of a version permanently authorizes you\n"
        "to choose that version for the Program.\n"
        "\n"
        "  Later license versions may give you additional or different\n"
        "permissions.  However, no additional obligations are imposed on any\n"
        "author or copyright holder as a result of your choosing to follow a\n"
        "later version.\n"
        "\n"
        "  15. Disclaimer of Warranty.\n"
        "\n"
        "  THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\n"
        "APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT\n"
        "HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY\n"
        "OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,\n"
        "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n"
        "PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM\n"
        "IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF\n"
        "ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\n"
        "\n"
        "  16. Limitation of Liability.\n"
        "\n"
        "  IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING\n"
        "WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS\n"
        "THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY\n"
        "GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE\n"
        "USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF\n"
        "DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD\n"
        "PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),\n"
        "EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF\n"
        "SUCH DAMAGES.\n"
        "\n"
        "  17. Interpretation of Sections 15 and 16.\n"
        "\n"
        "  If the disclaimer of warranty and limitation of liability provided\n"
        "above cannot be given local legal effect according to their terms,\n"
        "reviewing courts shall apply local law that most closely approximates\n"
        "an absolute waiver of all civil liability in connection with the\n"
        "Program, unless a warranty or assumption of liability accompanies a\n"
        "copy of the Program in return for a fee.\n"
        "\n"
        "                     END OF TERMS AND CONDITIONS\n"
        "\n"
        "            How to Apply These Terms to Your New Programs\n"
        "\n"
        "  If you develop a new program, and you want it to be of the greatest\n"
        "possible use to the public, the best way to achieve this is to make it\n"
        "free software which everyone can redistribute and change under these terms.\n"
        "\n"
        "  To do so, attach the following notices to the program.  It is safest\n"
        "to attach them to the start of each source file to most effectively\n"
        "state the exclusion of warranty; and each file should have at least\n"
        "the \"copyright\" line and a pointer to where the full notice is found.\n"
        "\n"
        "    <one line to give the program's name and a brief idea of what it does.>\n"
        "    Copyright (C) <year>  <name of author>\n"
        "\n"
        "    This program is free software: you can redistribute it and/or modify\n"
        "    it under the terms of the GNU Affero General Public License as published by\n"
        "    the Free Software Foundation, either version 3 of the License, or\n"
        "    (at your option) any later version.\n"
        "\n"
        "    This program is distributed in the hope that it will be useful,\n"
        "    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
        "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
        "    GNU Affero General Public License for more details.\n"
        "\n"
        "    You should have received a copy of the GNU Affero General Public License\n"
        "    along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"
        "\n"
        "Also add information on how to contact you by electronic and paper mail.\n"
        "\n"
        "  If your software can interact with users remotely through a computer\n"
        "network, you should also make sure that it provides a way for users to\n"
        "get its source.  For example, if your program is a web application, its\n"
        "interface could display a \"Source\" link that leads users to an archive\n"
        "of the code.  There are many ways you could offer source, and different\n"
        "solutions will be better for different programs; see section 13 for the\n"
        "specific requirements.\n"
        "\n"
        "  You should also get your employer (if you work as a programmer) or school,\n"
        "if any, to sign a \"copyright disclaimer\" for the program, if necessary.\n"
        "For more information on this, and how to apply and follow the GNU AGPL, see\n"
        "<https://www.gnu.org/licenses/>.\n"
        );

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(expect, result, strlen(expect)) == 0);

        result = testrun_string_free(result);
        return testrun_log_success();
}



/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                   #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int testcases_structure() {

        testrun_init();
        testrun_test(test_testrun_copyright_structure_is_valid);
        testrun_test(test_testrun_copyright_calculate_length);
        testrun_test(test_testrun_copyright_write_to);
        testrun_test(test_testrun_copyright_to_string);
        testrun_test(test_testrun_copyright_gpl_statement_to_string);

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int testcases_output_formating() {

        testrun_init();
        testrun_test(test_testrun_copyright_to_header);
        testrun_test(test_testrun_copyright_default_c_header);
        testrun_test(test_testrun_copyright_default_shell_header);

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int testcases_content() {

        testrun_init();
        testrun_test(test_testrun_copyright_default);
        testrun_test(test_testrun_copyright_apache_version_2);
        testrun_test(test_testrun_copyright_GPL_v3);
        testrun_test(test_testrun_copyright_BSD_3Clause);
        testrun_test(test_testrun_copyright_BSD_2Clause);
        testrun_test(test_testrun_copyright_MIT);

        testrun_test(test_testrun_copyright_apache_v2_licence);
        testrun_test(test_testrun_copyright_gpl_v3_general_licence);
        testrun_test(test_testrun_copyright_gpl_v3_lesser_licence);
        testrun_test(test_testrun_copyright_gpl_v3_affero_licence);

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int all_tests() {

        testrun_init();

        result = testcases_structure();
        if (result < 0)
                return -1;

        testrun_counter += result;

        result = testcases_content();
        if (result < 0)
                return -1;

        testrun_counter += result;

        result = testcases_output_formating();
        if (result < 0)
                return -1;

        testrun_counter += result;

        return testrun_counter;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST EXECUTION                                                  #EXEC
 *
 *      ------------------------------------------------------------------------
 */


testrun_run(all_tests);