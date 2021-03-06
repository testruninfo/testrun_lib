#!/usr/bin/env bash
#
#       Copyright 2017 Markus Toepfer
#
#       Licensed under the Apache License, Version 2.0 (the "License");
#       you may not use this file except in compliance with the License.
#       You may obtain a copy of the License at
#
#               http://www.apache.org/licenses/LICENSE-2.0
#
#       Unless required by applicable law or agreed to in writing, software
#       distributed under the License is distributed on an "AS IS" BASIS,
#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#       See the License for the specific language governing permissions and
#       limitations under the License.
#
#       ------------------------------------------------------------------------
#
#       File            testrun_simple_coverage.sh
#       Authors         Markus Toepfer
#       Date            2017-11-30
#
#       Project         testrun.info
#
#       Description     Count functions of folder src vs unit test functions.
#
#                       CONVENTION
#
#                       Each function in any file of the source folder located
#                       "src"
#                       will have a corresponding test function,
#                       using the same name in a file of the unit tests located at
#                       "tests/unit",
#                       with a function name prefix of
#                       "test_".
#
#                       EXAMPLE      function | test_function
#
#                       NOTE         This simple coverage test just covers the
#                                    observance of the given coding convention.
#
#
#       Usage           ./testrun_simple_coverage.sh /path/to/project
#
#       Dependencies    bash, ctags, awk, sed, grep
#
#       Last changed    2018-07-11
#       ------------------------------------------------------------------------

start_time=$(date "+%Y.%m.%d-%H.%M.%S.%N")
PREFIX="test_"

SRCDIR="$1/src"
TESTDIR="$1/tests/unit"
FOLDER_LOGFILE="$1/build/tests/log"

# SET A LOGFILE
LOGFILE="$FOLDER_LOGFILE/coverage.$start_time.log"
touch $LOGFILE
chmod a+w $LOGFILE

echo "-------------------------------------------------------" >> $LOGFILE
echo "               REPORT COVERAGE TESTING"                  >> $LOGFILE
echo "-------------------------------------------------------" >> $LOGFILE
echo "   TIME 	 $start_time" >> $LOGFILE
echo "" >> $LOGFILE

# GENERATE CTAGS SOURCE
cd $SRCDIR
if [ $? -ne 0 ]; then
        exit 1
fi
ctags --c-types=f -R
# remove the ctags stuff, to leave just the function lines
sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions
# remove anything but the function names
awk '{print $1 }' $SRCDIR/functions > $SRCDIR/functionNames
# count the lines
sourceFkt="$(cat functions | wc -l)"
echo "   count source\t" $sourceFkt >> $LOGFILE

# GENERATE CTAGS TESTS
cd $TESTDIR
if [ $? -ne 0 ]; then
        exit 1
fi
ctags --c-types=f -R
# remove the ctags stuff, to leave just the function lines
sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions
# remove anything but the function names
awk '{print $1 }' $TESTDIR/functions > $TESTDIR/functionNames
# count the lines
testFkt="$(cat functions | grep -i ^$PREFIX | wc -l)"
echo "   count tests\t" $testFkt >> $LOGFILE

echo "
UNTESTED: " >> $LOGFILE
# Found functions:
while read line;
do
        grep -n '^'$PREFIX$line'$' $TESTDIR/functionNames > \
        /dev/null || echo $line >> $LOGFILE
done < $SRCDIR/functionNames

if [ $sourceFkt != 0 ]; then
        echo "............................................"  >> $LOGFILE
        echo "COVERAGE: $sourceFkt $testFkt" | \
        awk '{ printf $1 " %.2f %% \n", $3/$2*100}' >> $LOGFILE
fi

cat $LOGFILE
echo "-------------------------------------------------------"
echo ""

# cleanup remove the files we created
rm $SRCDIR/tags
rm $SRCDIR/functions
rm $SRCDIR/functionNames
rm $TESTDIR/tags
rm $TESTDIR/functions
rm $TESTDIR/functionNames

