#!/usr/bin/bash
# This script file should be executable i.e. :    chmod u+x test.sh
# then run it with: ./test.sh

rm -fr tmptst

# Run makefile to create .zip
make zip
# Check there's a .zip file
if [[ -f 8q.zip ]]
then
   echo "PASS - Found 8q.zip"
else
   echo "ERROR - Can't find 8q.zip ???"
   exit 1
fi
mkdir tmptst
cd tmptst
cp ../8q.zip .

# unzip file - this will contain 8q.c etc with any other sub-directories etc.
unzip 8q.zip 
if [[ -f 8q.c ]]
then
   echo "PASS - Found 8q.c"
else
   echo "ERROR - Can't find 8q.c in the zip ???"
   exit 1
fi

if [[ -f 8q_dr.c ]]
then
   echo "PASS - Found 8q_dr.c"
else
   echo "ERROR - Can't find 8q_dr.c in the zip ???"
   exit 1
fi

if [[ -f extension.c ]]
then
   echo "PASS - Found 8q_dr.c"
else
   echo "ERROR - Can't find 8q_dr.c in the zip ???"
   exit 1
fi

if [[ -f 8q.h ]]
then
   echo "PASS - Found 8q.h"
else
   echo "ERROR - Can't find 8q.h in the zip ???"
   exit 1
fi

if [[ -f extension.c ]]
then
   echo "PASS - Found extension.c"
else
   echo "ERROR - Can't find extension.c in the zip ???"
   exit 1
fi

if [[ -f extension.h ]]
then
   echo "PASS - Found extension.h"
else
   echo "ERROR - Can't find extension.h in the zip ???"
   exit 1
fi

if [[ -f extension.txt ]]
then
   echo "PASS - Found extension.txt"
else
   echo "ERROR - Can't find extension.txt in the zip ???"
   exit 1
fi

if [[ -f neillsdl2.h ]]
then
   echo "PASS - Found neillsdl2.h"
else
   echo "ERROR - Can't find neillsdl2.h in the zip ???"
   exit 1
fi

if [[ -f neillsdl2.c ]]
then
   echo "PASS - Found neillsdl2.c"
else
   echo "ERROR - Can't find neillsdl2.c in the zip ???"
   exit 1
fi

if [[ -f Makefile ]]
then
   echo "PASS - Found Makefile"
else
   echo "ERROR - Can't find Makefile in the zip ???"
   exit 1
fi

# Try and compile the code
make 8q_san
if [[ -f 8q_san ]]
then
   echo "PASS - Managed to compile 8q_san.c without warnings"
else
   echo "ERROR - 8q_san.c doesn't compile without warnings ???"
   exit 1
fi

# Does it run ?
# $? is the exit value of the command
./8q_san 4 >& out.txt
if [[ $? -eq 0 ]]
then
   echo "PASS - Managed to run 8q_san 4 without problems"
else
   echo "ERROR - 8q_san 4 runs with problems"
   exit 1
fi

# Did it print the correct number of solutions
# #n is the length of the string n
n=`egrep "2 solutions" out.txt`
if [[ ${#n} -eq 11 ]]
then
   echo "PASS - 8q_san output is correct"
else
   echo "ERROR - 8q_san output is incorrect - was expecting '2 solutions'"
   exit 1
fi

# Does it run with -verbose ?
# output saved in out.txt
# $? is the exit value of the command
./8q_san -verbose 4 >& out.txt
if [[ $? -eq 0 ]]
then
   echo "PASS - Managed to run 8q_san -verbose 4 without problems"
else
   echo "ERROR - 8q_san -verbose 4 runs with problems"
   exit 1
fi

# Did it print the solutions ?
# wc -l is the number of lines (in out.txt)
n=`cat out.txt | wc -l`
if [[ $n -eq 3 ]]
then
   echo "PASS - 8q_san -verbose 4 output is 3 lines"
else
   echo "ERROR - 8q_san output is incorrect - was expecting 3 lines of output, got $n"
   exit 1
fi

######### 8q

# Try and compile the code
make 8q
if [[ -f 8q ]]
then
   echo "PASS - Managed to compile 8q.c without warnings"
else
   echo "ERROR - 8q.c doesn't compile without warnings ???"
   exit 1
fi

# Does it run ?
# $? is the exit value of the command
./8q 4 >& out.txt
if [[ $? -eq 0 ]]
then
   echo "PASS - Managed to run 8q 4 without problems"
else
   echo "ERROR - 8q 4 runs with problems"
   exit 1
fi

make run 8q >& out.txt
if [[ $? -eq 0 ]]
then
   echo "PASS - Managed to run 8q without problems"
else
   echo "ERROR - 8q runs with problems"
   exit 1
fi

# Did it print the correct number of solutions
# #n is the length of the string n
n=`egrep "2 solutions" out.txt`
if [[ ${#n} -eq 11 ]]
then
   echo "PASS - 8q output is correct"
else
   echo "ERROR - 8q output is incorrect - was expecting '2 solutions'"
   exit 1
fi

# Does it run with -verbose ?
# output saved in out.txt
# $? is the exit value of the command
./8q -verbose 4 >& out.txt
if [[ $? -eq 0 ]]
then
   echo "PASS - Managed to run 8q -verbose 4 without problems"
else
   echo "ERROR - 8q -verbose 4 runs with problems"
   exit 1
fi

# Did it print the solutions ?
# wc -l is the number of lines (in out.txt)
n=`cat out.txt | wc -l`
if [[ $n -eq 3 ]]
then
   echo "PASS - 8q -verbose 4 output is 3 lines"
else
   echo "ERROR - 8q output is incorrect - was expecting 3 lines of output, got $n"
   exit 1
fi


# Clean up
cd ..
rm -fr tmptst
