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

# test extension with invalid filename args

exit 0