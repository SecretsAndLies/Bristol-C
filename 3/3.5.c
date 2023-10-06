#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LINE_LEN 20
#define LINES_TO_PRINT 20

void generate_results(int rule);
int rule_110(int l, int m, int r);
int rule_124(int l, int m, int r);
int rule_30(int l, int m, int r);
int rule_90(int l, int r);
void copy_arr(int source[LINE_LEN], int target[LINE_LEN]);
void fill_with_1_in_location_n(int n, int arr[LINE_LEN]);
void print_arr(int arr[LINE_LEN]);
void fill_with_random_values(int arr[LINE_LEN]);
void generate_next_line(int line[LINE_LEN], int next[LINE_LEN], int rule);
void test(void);

int main(void)
{
    test();

    int rule;
    printf("Enter the rule you want (110, 30, 90, 124)\n");
    if (scanf("%d", &rule)!=1){
        exit(1);
    }
    
    generate_results(rule);

    return 0;
}

void generate_results(int rule)
{
    int top[LINE_LEN] = {0};
    printf("Do you want to have a random first line? (y,n)\n");
    char random_selector = getchar();
    if (random_selector=='y'){
        fill_with_random_values(top);
    }
    else{
        fill_with_1_in_location_n(LINE_LEN/2, top); 
    }

    print_arr(top);

    for (int i=0; i<LINES_TO_PRINT; i++){
        int next[LINE_LEN] = {0};
        generate_next_line(top, next, rule);
        print_arr(next);

        // resets so next is now top (next will get overwritten
        // in the next loop, so doesn't matter
        // that it has garbage).
        copy_arr(next,top);
    }
}

void copy_arr(int source[LINE_LEN], int target[LINE_LEN])
{
    for(int i=0; i<LINE_LEN; i++){
       target[i]=source[i]; 
    }
}

void generate_next_line(int line[LINE_LEN], int next[LINE_LEN], int rule)
{
    int l,m,r;

    for (int i=0; i<LINE_LEN; i++){
        m = line[i];

        if (i == 0){
            l = 0;
            r = line[i+1];
        }

        else if(i == LINE_LEN-1){
            l = line[i-1];
            r = 0;
        }
        else{
            r = line[i+1];
            l = line[i-1];
        }
      
        if(rule==110){
            next[i] = rule_110(l, m, r);
        }
        if(rule==124){
            next[i] = rule_124(l, m, r);
        }
        if(rule==30){
            next[i] = rule_30(l, m, r);
        }
        if(rule==90){
            next[i] = rule_90(l, r);
        }
    }
    
}

int rule_110(int l, int m, int r)
{
    int sum = l + m + r;
    if (sum == 3 || sum == 0){
        return 0;
    }
    
    if (sum == 2){
        return 1;
    }

    if (l==1){
        return 0;
    }
    return 1;
}

int rule_124(int l, int m, int r)
{
    return !rule_110(l,m,r);
}

int rule_30(int l, int m, int r)
{
    return l^(m|r);
}

int rule_90(int l, int r)
{
    return l^r;
}

void fill_with_1_in_location_n(int n, int arr[LINE_LEN]){
    arr[n]=1;
}

void fill_with_random_values(int arr[LINE_LEN])
{
    for (int i=0; i<LINE_LEN; i++){
        arr[i] = rand()%2;
    }
}


void print_arr(int arr[LINE_LEN])
{
   for (int i=0; i<LINE_LEN; i++){
        printf("%i ", arr[i]);
   }
   printf("\n");
}

void test(void)
{
    assert(rule_110(0,0,0)==0);
    assert(rule_110(1,1,1)==0);
    assert(rule_110(1,1,0)==1);
    assert(rule_110(1,0,1)==1);
    assert(rule_110(0,1,1)==1);
    assert(rule_110(1,0,0)==0);
    assert(rule_110(0,1,0)==1);
    assert(rule_110(0,0,1)==1);

    assert(rule_124(0, 0, 0) == 1);
    assert(rule_124(0, 0, 1) == 0);
    assert(rule_124(0, 1, 0) == 0);
    assert(rule_124(0, 1, 1) == 0);
    assert(rule_124(1, 0, 0) == 1);
    assert(rule_124(1, 0, 1) == 0);
    assert(rule_124(1, 1, 0) == 0);
    assert(rule_124(1, 1, 1) == 1);

    assert(rule_30(0, 0, 0) == 0);
    assert(rule_30(0, 0, 1) == 1);
    assert(rule_30(0, 1, 0) == 1);
    assert(rule_30(0, 1, 1) == 1);
    assert(rule_30(1, 0, 0) == 1);
    assert(rule_30(1, 0, 1) == 0);
    assert(rule_30(1, 1, 0) == 0);
    assert(rule_30(1, 1, 1) == 0);

    assert(rule_90(0, 0) == 0);
    assert(rule_90(0, 1) == 1);
    assert(rule_90(0, 0) == 0);
    assert(rule_90(0, 1) == 1);
    assert(rule_90(1, 0) == 1);
    assert(rule_90(1, 1) == 0);
    assert(rule_90(1, 0) == 1);
    assert(rule_90(1, 1) == 0);


}
