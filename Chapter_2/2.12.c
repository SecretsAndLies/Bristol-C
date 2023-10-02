#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>

#define MAX 35

int get_winnings(int guess, int wheel_val, int bet);
bool eval_even_guess(int wheel_val);
bool eval_number_guess(int guess, int wheel_val);
bool eval_odd_guess(int wheel_val);
int get_number(void);
void test(void);

int main(void)
{
    test();
    printf("Make your guess, 0-35. Select 36 for all even numbers, and 37 for all odd numbers. \n");
    int guess = 0;
    scanf("%d", &guess);
    printf("Enter your bet: \n");
    int bet = 0;
    scanf("%d", &bet);
    int wheel_val = 7; // get_number();
    
    int winnings = get_winnings(guess, wheel_val, bet);
    printf("You guessed %i. Wheel was %i. Your winnings are %i \n", guess, wheel_val, winnings);
    

    return 0;
}

int get_winnings(int guess, int wheel_val, int bet)
{
    int winnings = 0;
    bool won = false;
    if (guess<=MAX){
        won = eval_number_guess(guess,wheel_val);
        if(won){
            winnings = (bet*MAX)+bet;        
        }
    }
    if (guess==36){
        won = eval_even_guess(wheel_val); 
        if(won){
            winnings = (bet*2)+bet;  
        }
    }
    if (guess==37){
        won = eval_odd_guess(wheel_val);
        if(won){
            winnings =(bet*2)+bet;
        }
    }
    return winnings;
}

bool eval_odd_guess(int wheel_val)
{
    if (wheel_val==0){
        return false;
    }
    return wheel_val % 2 == 1 ;
}

bool eval_even_guess(int wheel_val)
{
    if (wheel_val==0){
        return false;
    }
    return wheel_val % 2 == 0 ;
}


bool eval_number_guess(int guess, int wheel_val)
{
   return guess==wheel_val; 
}

int get_number(void)
{
    return rand() % (MAX+1);
}
void test(void)
{
    assert(eval_number_guess(1,1)==true);
    assert(eval_number_guess(1,2)==false);

    for (int i=0; i<100; i++){
        assert(get_number()<=MAX);
        assert(get_number()>=0);
    }
}

