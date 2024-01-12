#!/bin/bash
# chmod u+x Testing/test.sh 

check_exit_fail() {
    filename=$2
    program=$1
    ./$program "TTLs/$filename" > /dev/null 2>&1
    if [ $? -ne 1 ]; then
        echo "ERROR: expected exit code 1 for $filename but got $?"
        exit 1
    fi
}

check_exit_sucess() {
    filename=$2
    program=$1
    ./$program "TTLs/$filename" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo "ERROR: expected exit code 0 for $filename but got $?"
        exit 1
    fi
}

check_output_match() {
    extension=$3
    filename=$2
    program=$1

    ./$program "TTLs/$filename.ttl" out_$filename.$extension

    if [ $? -ne 0 ]; then
        echo "ERROR: expected exit code 0 for $filename $program $extension but got $?"
        exit 1
    fi

    # Compare the output file with the expected results
    if ! diff -q out_$filename.$extension results/out_$filename.$extension > /dev/null; then
        echo "ERROR: Output mismatch for $filename"
        exit 1
    fi

}

check_exit_fail_with_string() {
    program=$1
    filename=$2
    expected_string=$3
    extra_arg=$4
    if [ -n "$extra_arg" ]; then
        output=$("./$program" "$filename" "$extra_arg" 2>&1)
        exit_code=$?
    else
        output=$("./$program" "$filename" 2>&1)
        exit_code=$?
    fi
    if [ $exit_code -ne 1 ]; then
        echo "ERROR: expected non 0 exit code for $filename $program but got $exit_code"
        exit 1
    fi
    if [[ "$output" != *"$expected_string"* ]]; then
        echo "ERROR: expected string '$expected_string' not found in the output for $filename"
        exit 1
    fi
}

check_exit_fail parse_s fail_parse_ok_interp.ttl
check_exit_sucess parse_s empty.ttl
check_exit_sucess parse_s forward.ttl
check_exit_sucess parse_s set1.ttl
check_exit_sucess parse_s donothing.ttl
check_exit_sucess parse_s set2.ttl
check_exit_sucess parse_s turn.ttl
check_exit_sucess parse_s spiral.ttl
check_exit_sucess parse_s octagon1.ttl
check_exit_sucess parse_s octagon2.ttl
check_exit_sucess parse_s tunnel.ttl
check_exit_sucess parse_s labyrinth.ttl
check_exit_sucess parse_s 5x5.ttl
check_exit_sucess parse_s downarrow.ttl
check_exit_sucess parse_s ok_parse_fail_interp.ttl

check_exit_fail_with_string parse_s "forward.ttl" "Wrong number of arguments" "test.ps"
check_exit_fail_with_string parse_s ".ttl" "Invalid filename: too short" ""
check_exit_fail_with_string parse_s "aaa.tt" "Invalid filename: must end with .ttl" ""
#open non existant file
check_exit_fail_with_string parse_s "aaa.ttl" "Cannot open file" ""
# File contains words that are too long.
check_exit_fail_with_string parse_s "TTLs/too_long_word.ttl" "File contains words that are too long" ""
# File is too long (too many words).
check_exit_fail_with_string parse_s "TTLs/too_long_file.ttl" "File is too long (too many words)" ""

#INTERP TESTS:
check_exit_fail interp_s ok_parse_fail_interp.ttl
#wrong extension
check_exit_fail interp_s ok_parse_fail_interp.ttl blah.doc
#no extension
check_exit_fail interp_s ok_parse_fail_interp.ttl blah
#too many args.
check_exit_fail interp_s ok_parse_fail_interp.ttl blah.txt blah.ps 
#invalid file
check_exit_fail interp_s fakefile.ttl 

check_output_match interp_s forward txt
check_output_match interp_s empty txt
check_output_match interp_s donothing txt
check_output_match interp_s fail_parse_ok_interp txt
check_output_match interp_s set1 txt
check_output_match interp_s set2 txt
check_output_match interp_s turn txt
check_output_match interp_s tunnel txt
check_output_match interp_s octagon1 txt
check_output_match interp_s octagon2 txt
#todo I can't get these to work.
check_output_match interp_s 5x5 txt
check_output_match interp_s spiral txt
check_output_match interp_s downarrow txt
check_output_match interp_s labyrinth txt

exit 0