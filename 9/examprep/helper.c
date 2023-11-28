#include <stdio.h> // 1
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

// Generates a random number between low and high (inclusive)
int random_number(int low, int high) {
    return low + (rand() % (high - low + 1));
}

// Checks if a number is prime
int is_prime(int number) {
    if (number <= 1) return 0;
    if (number % 2 == 0 && number > 2) return 0;
    for (int i = 3; i <= sqrt(number); i += 2) {
        if (number % i == 0) return 0;
    }
    return 1;
}
// ali simple version.
bool is_prime_ali(long n)
{
    for (long i=2; i<n; i++)
    {
        if(n%i==0){
            return false;
        }
    }
    return true;
}

int isEven(int n)
{
    if (n % 2 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Swaps two integers
void swapint(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Swaps two char
void swapchar(char *a, char *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Calculates the factorial of a number
long long factorial(int n) {
    long long fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

// Finds the greatest common divisor of two numbers
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Checks if a string is a palindrome
bool is_palindrome(char a[])
{
    int i=0;
    int j=strlen(a)-1;
    
    while (i<=j)
    {
        char c = tolower(a[i]);
        char d = tolower(a[j]);
       
        if (!isalpha(c)){
            i++;
            continue;
        }
        
        if (!isalpha(d)){
            j--;
            continue;
        }

        if(c!=d){
            return false;
        }
        i++;
        j--;
    }
    return true;
}

// Converts a string to uppercase
void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

// Performs a linear search in an array
int linear_search(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) return i;
    }
    return -1; // Not found
}

// Sorts an array using bubble sort
void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swapint(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// Reads a line of text into a buffer
void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character if present
    }
}

// Calculates the power of a number
double power(double base, int exponent) {
    double result = 1.0;
    for (int i = 0; i < abs(exponent); i++) {
        result *= base;
    }
    return exponent < 0 ? 1.0 / result : result;
}

// Reverses a string
void reverse_string(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

// Finds the minimum value in an integer array
int min_value(int arr[], int n) {
    int min = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

// Finds the maximum value in an integer array
int max_value(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// takes in a hist and string as 
// int hist[26] ={0};
int str_to_hist(char str[], int hist[]){
    int len = strlen(str);
    for (int i=0; i<len; i++){
        hist[tolower(str[i])-'a']++;
    }
}

// fill array with 1s and 0s
void fill_with_random_values(int arr[10])
{
    for (int i=0; i<10; i++){
        arr[i] = rand()%2;
    }
}

// bool queen_in_diagonals(int orig_r, int orig_c, Board* b) 
// {
//     int directions[8][2] = 
//                     {
//                     {-1,-1}, //UP_AND_LEFT
//                     {-1,1},  //UP_AND_RIGHT
//                     {1,-1}, //DOWN_AND_LEFT
//                     {1,1}, //DOWN_AND_RIGHT
//                      {1,0},//up
//                       {-1,0}, // down
//                        {0,-1}, // left
//                         {0,1}, //right
//     for (int d = 0; d < 4; d++) {
//         int r = orig_r;
//         int c = orig_c;
//         while (is_valid_pos(r, c, w,h)) {
//             if (b->grid[r][c] == QUEEN) {
//                 return true;
//             }
//             r += directions[d][0];
//             c += directions[d][1];
//         }
//     }

//     return false;
// }

// bool is_valid_pos( nt r, int c, int w, int h)
// {
//     if (r >= 0 && c >= 0 && r < h && c < w){
//         return true;    
//     }
//     return false;
// }

// print arr of size 10
void print_arr(int arr[10])
{
   for (int i=0; i<10; i++){
        printf("%i ", arr[i]);
   }
   printf("\n");
}

void double_for_loop(int size, char board[2][2]){
    for (int r=0; r<size; r++){
        for (int c=0; c<size; c++){
            printf("%s \n", board[r][c]);
        }
    }
    
}



bool is_vowel(char c)
{
    switch (c){
        case 'a':
            return true;
        case 'e':
            return true;
        case 'i':
            return true;
        case 'o':
            return true;
        case 'u':
            return true;
        default:
            return false;
    }
}

void sort(char input[], int n){
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (input[j] > input[j + 1]) {
                swapchar(&input[j], &input[j + 1]);
            }
        }
    }
}

void read_lines_from_file(void){
    FILE* fp = fopen("helper.c", "r");
    char str1[100];
    char str2[100];
    // special first line
    fscanf(fp, "%s %s", &str1, &str2);
    printf("%s %s\n", str1, str2);

    char line[180];
    // loop
    // char *fgets(char *str, int n, FILE *stream)
    while(fgets(line,sizeof(line),fp)){
            printf("%s", line);
        }
    fclose(fp);
}

int min (int a, int b){
    if(a<b){return a;}
    else {return b;}
}

// commnand line args example.
//int main(int argc, char *argv[]) {
//    for (int i = 0; i < argc; i++) {
//        printf("%s\n", argv[i]);
//    }
//    return 0;
//}


void simple_if(void){
    if (num > 0) {
    printf("Positive\n");
} else if (num < 0) {
    printf("Negative\n");
} else {
    printf("Zero\n");
}
}

int main() {
    read_lines_from_file();
    char test[8] = "cdsadgc";
    sort(test, strlen(test));
   


    return 0;
}

