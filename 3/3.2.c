#include <stdio.h>
#include <assert.h>

#define SECS_IN_MIN 60
#define MINUTES_10 10*SECS_IN_MIN
#define SECONDS_10 10
#define NUMBER_OF_BUTTONS 3

void test(void);
int button_pushes_required(int time);

int main(void)
{
    test();
    int time;
    
    printf("Enter your time \n");
    while (scanf("%d", &time) == 1) {
        printf("Button pushes required = %i \n", \
                button_pushes_required(time));
        printf("Enter your time \n");
    }
 
    return 0;
}

int button_pushes_required(int time)
{
    int buttons[NUMBER_OF_BUTTONS] = \
    {MINUTES_10, SECS_IN_MIN, SECONDS_10};
    int i = 0;
    int buttons_required = 0;
    while (i<NUMBER_OF_BUTTONS)
    {
        if(time >= buttons[i]){
            time = time-buttons[i];
            buttons_required++;
        }
        else{
            i++;
        }
    }
   
    // covers the case that there's a remainder.
    if (time){
        buttons_required++;
    }

    return buttons_required;
}

void test(void)
{
    assert(button_pushes_required(90)==4);
    assert(button_pushes_required(25)==3);
    assert(button_pushes_required(705)==7);
}
