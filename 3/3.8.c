#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#define GOLDEN_RATIO 1.61803398875
#define LEN 10000
#define LOOP 1000

bool fibword_phi(int n, double phi);
void test(void);
void print_arr(bool arr[LEN], int size);
bool fibword_subs(int n);

int main(void)
{
    test();
    return 0;
}

int get_first_incorrect_value(double phi){
    for (int i=1; i<LOOP; i++){
        if(fibword_phi(i, phi)!=fibword_subs(i)){
            return i;
        }
    }
    return -1;
}

bool fibword_phi(int n, double phi)
{
    double n_x_golden_ratio = 
        (double) n * (double) phi;
    double n_add_1_times_golden_ratio =
        ((double) n + (double) 1) * (double) phi;
    double answer = 
        (double) 2 + 
        floor(n_x_golden_ratio) - 
        floor(n_add_1_times_golden_ratio);
    return (int) answer;
}

bool fibword_subs(int n)
{
    bool arr[LEN] = {false};
    
    // handily, this is also the length of the array.
    int empty_element_index = 1; 
    int i=0;
    while (empty_element_index<n){
        if (arr[i] == false){
            arr[empty_element_index] = true;
            arr[empty_element_index+1] = false;
            empty_element_index+=2;
        }
        else{
            arr[empty_element_index] = false;
            empty_element_index++;
        }
        i++;
    }

    // n counter begins at 1, not 0.
    return arr[n-1];
}

void print_arr(bool arr[LEN], int size)
{
    for (int i=0; i<size; i++){
        printf("%i", arr[i]);
    }
    printf("\n");
}

void test(void)
{
    assert(fibword_subs(1)==0);
    assert(fibword_subs(2)==1);
    assert(fibword_subs(3)==0);
    assert(fibword_subs(4)==0);
    assert(fibword_subs(5)==1);
    assert(fibword_subs(6)==0);

    assert(get_first_incorrect_value(1.61)==12);
    assert(get_first_incorrect_value(1.61803)==609);
    
}
