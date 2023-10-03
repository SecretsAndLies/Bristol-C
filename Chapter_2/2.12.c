#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define MAX 35

enum bet_type {NUMBER, ODD, EVEN};
typedef enum bet_type bet_type;
typedef int dollars;

int get_guess(bet_type type_of_bet);
dollars get_bet(void);
bet_type get_type_of_bet(void);
void print_win_message(int guess, int wheel_val, int winnings, bet_type type_of_guess);
int get_winnings(int guess, int wheel_val, int bet, bet_type type_of_bet);
bool eval_even_guess(int wheel_val);
bool eval_number_guess(int guess, int wheel_val);
bool eval_odd_guess(int wheel_val);
int get_random_number(void);
void test(void);


int main(void)
{
    test();
   
    bet_type type_of_bet = get_type_of_bet();
    int guess = get_guess(type_of_bet);
    dollars bet = get_bet();
    int wheel_val = get_random_number();
    
    int winnings = get_winnings(guess, wheel_val, bet, type_of_bet);

    print_win_message(guess, wheel_val, winnings, type_of_bet);

    return 0;
}

dollars get_bet(void)
{
    printf("Enter your bet: \n");
        dollars bet = 0;
        if(scanf("%d", &bet) != 1){
            exit(1);
        }
    return bet;
}

int get_guess(bet_type type_of_bet)
{
    int guess = 0;
    
    if (type_of_bet == NUMBER){
        printf("Make your guess, 0-35. \n");
        if(scanf("%d", &guess) != 1){
            exit(1);
        }
     }
    
    return guess;
}

bet_type get_type_of_bet(void)
{
    printf("Do you want to make a guess (enter 0)"
            " or guess odd (1) or even (2) \n");
    bet_type type_of_bet = 0;
    if(scanf("%d", &type_of_bet) != 1){
        exit(1);
    }
    
    return type_of_bet;
}

void print_win_message(int guess, int wheel_val, int winnings, bet_type type_of_guess)
{
    if (type_of_guess == NUMBER){
        printf("You guessed %i. Wheel was %i."
                " Your winnings are %i \n", guess, wheel_val, winnings);
    }
    
    if (type_of_guess == ODD){
        printf("You guessed odd. Wheel was %i."
                " Your winnings are %i \n", wheel_val, winnings); 
    }

    if (type_of_guess == EVEN){
        printf("You guessed even. Wheel was %i."
                " Your winnings are %i \n", wheel_val, winnings);
    }
}

int get_winnings(int guess, int wheel_val, int bet, bet_type type_of_bet)
{
    int winnings = 0;
    bool won = false;
    if (type_of_bet == NUMBER){
        won = eval_number_guess(guess, wheel_val);
        if (won){
            winnings = (bet * MAX) + bet;        
        }
    }
    if (type_of_bet == EVEN){
        won = eval_even_guess(wheel_val); 
        if (won){
            winnings = (bet * 2) + bet;  
        }
    }
    if (type_of_bet == ODD){
        won = eval_odd_guess(wheel_val);
        if (won){
            winnings = (bet * 2) + bet;
        }
    }
    return winnings;
}

bool eval_odd_guess(int wheel_val)
{
    if (wheel_val == 0){
        return false;
    }
    return wheel_val % 2 == 1;
}

bool eval_even_guess(int wheel_val)
{
    if (wheel_val == 0){
        return false;
    }
    return wheel_val % 2 == 0;
}


bool eval_number_guess(int guess, int wheel_val)
{
    return guess == wheel_val; 
}

int get_random_number(void)
{
    return rand() % (MAX+1);
}

void test(void)
{
    assert(get_winnings(20, 20, 1, NUMBER) == 36);
    assert(get_winnings(20, 1, 1, NUMBER) == 0);
    assert(get_winnings(0, 20, 1, EVEN) == 3);
    assert(get_winnings(0, 21, 1, EVEN) == 0);
    assert(get_winnings(0, 0, 1, EVEN) == 0);
    assert(get_winnings(0, 0, 1, ODD) == 0);
    assert(get_winnings(0, 20, 1, ODD) == 0);

    assert(eval_number_guess(1,1) == true);
    assert(eval_number_guess(1,2) == false);

    for (int i=0; i<100; i++){
        assert(get_random_number() <= MAX);
        assert(get_random_number() >= 0);
    }
}

