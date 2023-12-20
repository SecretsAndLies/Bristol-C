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
        echo "ERROR: expected exit code 0 for $filename but got $?"
        exit 1
    fi

    # Compare the output file with the expected results
    if ! diff -q out_$filename.$extension results/out_$filename.$extension > /dev/null; then
        echo "ERROR: Output mismatch for $filename"
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
#interp stuff.
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
check_output_match interp_s downarrow txt
check_output_match interp_s 5x5 txt
check_output_match interp_s spiral txt
check_output_match interp_s labyrinth txt

exit 0