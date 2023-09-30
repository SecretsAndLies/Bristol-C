#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#define LOOPS 10000
#define NUM_TO_END 3

void test(void);
bool is_prime(long n);
long double primes_fraction(long num_to_end);

int main(void)
{
    test();
    long double fract = primes_fraction(NUM_TO_END);
    printf("%Lf \n",fract);

    return 0;
}

long double primes_fraction(long num_to_end)
{
    long double count = 0;
    long double count_that_ends_in_digit = 0;
    for (long i=1; i<LOOPS; i++){
        if (is_prime(i)){
            count++;
            if (i%10==num_to_end){
                count_that_ends_in_digit++;
            }
        }
    }
    long double res = count_that_ends_in_digit / count;
    return res;
}

bool is_prime(long n)
{
    for (long i=2; i<n; i++)
    {
        if(n%i==0){
            return false;
        }
    }
    return true;
}

void test(void)
{
    assert(is_prime(2)==true);
    assert(is_prime(3)==true);
    assert(is_prime(8)==false);
    assert(is_prime(17)==true);
    assert(is_prime(2001)==false);
}
