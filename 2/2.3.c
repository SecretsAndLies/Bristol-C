#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>

#define RADIUS 1000
#define LOOP 10000

double calculate_pi(int circle_hits, int all_points);
double generate_random_value(int side_len);
bool is_inside_circle(double x, double y, double r);
void test(void);

int main(void)
{
    test();
    int r = RADIUS;
    int circle_hits = 0;
    double pi = 0.0;
    int total_hits = 0;
    for (int i=0; i<LOOP; i++){
        double x = generate_random_value(r);
        double y = generate_random_value(r);
        bool t = is_inside_circle(x,y,r);
        if (t){
            circle_hits++;
        }
        total_hits++;
        pi = calculate_pi(circle_hits,total_hits);
        printf("pi is %f \n",pi);
    }
    return 0;
 }

void test(void)
{
    assert(is_inside_circle(7,9,193)==true);
    assert(is_inside_circle(7,9,8)==false);
   
    // testing lots of inputs to make sure rand is correct.
    for (int i=0; i<100; i++){
        assert(generate_random_value(10)<=10);
        assert(generate_random_value(10)>=0);
    }
}

double calculate_pi(int circle_hits, int all_points)
{
    return (4.0 * (double) circle_hits) / (double) all_points;
}

double generate_random_value(int side_len)
{
     return (double) (rand() % side_len);
}

bool is_inside_circle(double x, double y, double r)
{
    if (pow(x,2)+ pow(y,2) <= pow(r,2)){
        return true;
    }
    return false;
}
