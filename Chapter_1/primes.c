#include <stdlib.h>
#include <stdio.h>
//#define NDEBUG
#include <assert.h>
#include <stdbool.h>

void test(void);
bool is_prime(int n);
void print_n_primes(int n);

int main (void)
{
    test();
    print_n_primes(3000);
}

void print_n_primes(int n)
{
   int num_of_primes = 0;
   int num_to_check = 1;
    while (num_of_primes<n){
       if (is_prime(num_to_check)){
           printf("%i \n",num_to_check);
           num_of_primes++;
        }    
       num_to_check++;
    }
}


bool is_prime(int n)
{
    if (n==1){
        return false;    
    }
    if (n==2){
        return true;
    }    

    for (int i=2; i<n; i++){
        if (n%i==0){
            return false;
        }
    }
    return true;
}

void test(void)
{
    assert(is_prime(1)==false);
    assert(is_prime(2)==true);
    assert(is_prime(7)==true);
    assert(is_prime(10)==false);
    assert(is_prime(11)==true);
}
