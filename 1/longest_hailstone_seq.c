#include <stdlib.h>
#include <stdio.h>
//#define NDEBUG
#include <assert.h>
#define LIMIT 10000000

long long hailstone(long long n);
long long length_of_hailstone_seq(long long n);
void test(void);
long long which_number_has_longest_seq(long long max);
long long which_num_has_largest_num_in_seq(long long max);
long long max_num_in_seq(long long n);

int main (void)
{
    test();
    long long longest = which_number_has_longest_seq(LIMIT);
    printf("The number with the longest sequence between 1 and 10m is %lli \n",longest);
    long long largest = which_num_has_largest_num_in_seq(LIMIT);
    printf("The number with the largest num in the"
            "sequence between 1 and 10m is %lli \n",largest);
    return 0;
}

long long which_num_has_largest_num_in_seq(long long max)
{

    long long biggest_num=0;
    long long num;

    for (long long i=1; i<=max; i++){
        long long curr_max = max_num_in_seq(i);
        if (curr_max>biggest_num){
            num=curr_max;
        }
    }
    return num;
}

long long which_number_has_longest_seq(long long max)
{
    long long longest_length=0;
    long long starting_num;
    for (long long i=1; i<=max; i++){
        long long curr_len = length_of_hailstone_seq(i);
        if (curr_len>longest_length){
            starting_num = i;
            longest_length = curr_len;
        }
    }
    return starting_num;
}

long long length_of_hailstone_seq(long long n)
{
    long long count = 1; // we start at 1 to count the original number too.    
    while(n>1) {
        n=hailstone(n);
        count++;    
    }
    return count;
}


long long max_num_in_seq(long long n)
{
    long long max_num = 0;
    while(n>1) {
        n=hailstone(n);
        if (n>max_num){
        max_num=n;
        }
    }
    return max_num;
}

long long hailstone(long long n){
    if (n==1){
        return 1;
    }
    if (n%2==0){
        return n/2;
    }
    // otherwise, it's an odd number.
    return (3*n)+1;
}

void test(void)
{
    assert(hailstone(77)==232);
    assert(hailstone(232)==116);
    assert(hailstone(58)==29);
    assert(length_of_hailstone_seq(6)==9);
    assert(length_of_hailstone_seq(11)==15);
}
