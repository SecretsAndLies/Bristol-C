#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MINS_IN_HOUR 60
#define HOURS_IN_DAY 24


void test(void);
void full_diff(int fhours, int fmins, int shours, int smins);

int main(void){
    test();
    printf("Enter two times : ");
    int first_hours,first_mins;
    scanf("%2d:%2d",&first_hours,&first_mins);

    int second_hours,second_mins;
    scanf("%2d:%2d",&second_hours,&second_mins);
    
    full_diff(first_hours,first_mins,second_hours,second_mins);

}

void full_diff(int fhours, int fmins, int shours, int smins)
{
    int min_diff = 0;
    int carry_hours = 0;
    while (fmins!=smins){
        fmins++;
        min_diff++;
        if(fmins==MINS_IN_HOUR){
            fmins=0;
            carry_hours++;
        }
    }
    
    int hour_diff = 0;
    int hours = fhours+carry_hours;
    while (hours!=shours){
        hours++;
        hour_diff++;
        if (hours==HOURS_IN_DAY){
            hours = 0;
        }
     }
    printf("%02d:%02d\n",hour_diff,min_diff);
}

void test(void){
}
