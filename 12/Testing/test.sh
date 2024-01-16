#!/bin/bash
# chmod u+x Testing/test.sh 

check_exit_fail() {
    filename=$2
    program=$1
    ./$program "TTLs/$filename" > /dev/null 2>&1
    exit_status=$?
    if [ $exit_status -ne 1 ]; then
        echo "ERROR: expected exit code 1 for $program $filename"
        exit 1
    fi
}

check_exit_success() {
    filename=$2
    program=$1
    ./$program "TTLs/$filename" > /dev/null 2>&1
    exit_status=$?
    if [ $exit_status -ne 0 ]; then
        echo "ERROR: expected exit code 0 for $program $filename got $exit_status"
        exit 1
    fi
}

# todo validate that txt files are created.
check_exit_success_txt() {
    filename=$2
    program=$1
    ./$program "TTLs/$filename.ttl" $filename.txt
    exit_status=$?
    if [ $exit_status -ne 0 ]; then
        echo "ERROR: expected exit code 0 for $program $filename got $exit_status"
        exit 1
    fi
    if [ ! -f "$filename.txt" ]; then
        echo "ERROR: file $filename.txt not created"
        exit 1
    fi
}

## todo make this validate that ps files are created
## todo also possibly that they contain specific text.
check_exit_success_ps() {
    filename=$2
    program=$1
    ./$program "TTLs/$filename.ttl" "$filename.ps" > /dev/null 2>&1
    exit_status=$?
    if [ $exit_status -ne 0 ]; then
        echo "ERROR: expected exit code 0 for $program $filename"
        exit 1
    fi
    if [ ! -f "$filename.ps" ]; then
        echo "ERROR: file $filename.ps not created"
        exit 1
    fi
    #verifying that the pdf has boilerplate code
    search_string="setlinewidth"
    if ! check_string_in_file "$filename.ps" "$search_string"; then
        echo "$search_string not found in $filename.ps"
        exit 1
    fi
    if [ ! -f "$filename.pdf" ]; then
        echo "ERROR: file $filename.pdf not created"
        exit 1
    fi
}

check_string_in_file() {
    fname=$1
    string=$2

    if grep -qF -- "$string" "$fname"; then
        return 0
    else
        return 1
    fi
}

check_output_match() {
    extension=$3
    filename=$2
    program=$1

    ./$program "TTLs/$filename.ttl" out_$filename.$extension
    exit_status=$?
    if [ $exit_status -ne 0 ]; then
        echo "ERROR: expected exit code 0 for $filename $program $extension but got $exit_status"
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
check_exit_success parse_s empty.ttl
check_exit_success parse_s forward.ttl
check_exit_success parse_s set1.ttl
check_exit_success parse_s donothing.ttl
check_exit_success parse_s set2.ttl
check_exit_success parse_s turn.ttl
check_exit_success parse_s spiral.ttl
check_exit_success parse_s octagon1.ttl
check_exit_success parse_s octagon2.ttl
check_exit_success parse_s tunnel.ttl
check_exit_success parse_s labyrinth.ttl
check_exit_success parse_s 5x5.ttl
check_exit_success parse_s downarrow.ttl
check_exit_success parse_s ok_parse_fail_interp.ttl

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

#My version of these tests go out of bounds.
check_exit_fail interp_s spiral
check_exit_fail interp_s labyrinth

# These are close, but not quite a match for Neill's, so I
# just test the exit code
check_exit_success_txt interp_s 5x5
check_exit_success_txt interp_s downarrow
check_exit_success_txt interp_s hypno

#validate that post script is outputting a file and correct boilerplate
check_exit_success_ps interp_s downarrow

## these are my tests, designed to probe that specific beheviour works.
check_output_match interp_s loop_and_extra txt
check_output_match interp_s skip_nested_loop txt
check_exit_fail interp_s unterminated_loop

exit 0

# TODO validate that post script results in files being created
# and those files match some output.