#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#define STOP -999

int square_root_of_a_sqd_plus_b_sqd(int a, int b);
void test(void);
bool is_valid_triangle(int a, int b, int c);
bool sides_are_valid(int a, int b, int c);
bool side_is_valid(int a);

int main(void)
{
    test();
    int a,b,c;
    do {
         printf("Enter your three sides (enter %i as one of your nums to quit).\n",STOP);
         if(scanf("%i%i%i",&a,&b,&c)!=3){
             printf("Error, you need to enter three numbers\n");            
        }
        else{
           if(is_valid_triangle(a,b,c)){
                 printf("valid triangle.\n");
           }
           else{
                printf("invalid triangle \n");
           }    
        }
    }
    while (a!=STOP && b!=STOP && c!=STOP);
}

bool sides_are_valid(int a, int b, int c)
{
    if (side_is_valid(a) && side_is_valid(b) && side_is_valid(c)){
        return true;
    }
    return false;
}

bool side_is_valid(int a)
{
    if (a<=0){
        return false;
    }
    return true;
}

bool is_valid_triangle(int a, int b, int c)
{
  if (!sides_are_valid(a,b,c)){
      return false;
  }
  if (a==b==c){
    return true;
  }
    
  if (a==b || b == c || a==c) {
    return true;
  }

  if (a!=b && a!=c && b!=c){
    return true;
  }

  //we need to know which side is largest
  if(a>b && a>c){
    if(a == square_root_of_a_sqd_plus_b_sqd(b,c)){
        return true;
    }
  }
  if(b>a && b>c){
    if(b == square_root_of_a_sqd_plus_b_sqd(a,c)){
        return true;
    }
  }
  if(c>b && c>a){
    if(c == square_root_of_a_sqd_plus_b_sqd(a,b)){
        return true;
    }
  }
  return false;
}

int square_root_of_a_sqd_plus_b_sqd(int a, int b){
    double a_sqd = (double) (a*a);
    double b_sqd = (double) (b*b);
    
    double c = a_sqd + b_sqd;
    return round(sqrt(c)); 
}

void test(void)
{
    // equalatoral -- all same
    assert(is_valid_triangle(7,7,7) == true);

    // isosolies -- one differnet
    assert(is_valid_triangle(7,17,7) == true);
    assert(is_valid_triangle(7,1,1) == true);
    assert(is_valid_triangle(4,100,100) == true);

    // scalene - all different
    assert(is_valid_triangle(34,32,15) == true);

    // right angle c = root a2 + b2
    assert(is_valid_triangle(3,5,4)==true);
    assert(is_valid_triangle(4,3,5)==true);
    assert(is_valid_triangle(5,3,4)==true);
    
    // invalid
    assert(is_valid_triangle(0,17,7) == false);
    assert(is_valid_triangle(0,0,0) == false);
    assert(is_valid_triangle(-1,100,100) == false);

   // side is valid
   assert(side_is_valid(-1)==false);
   assert(side_is_valid(0)==false);
   assert(side_is_valid(1)==true);

   // other func
   assert(square_root_of_a_sqd_plus_b_sqd(3,4)==5);
   assert(square_root_of_a_sqd_plus_b_sqd(6,8)==10);
}

