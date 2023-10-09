#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define SEARCH_NUM 5000000
#define BOARD_SIZE 20

void test(void);
int get_board_difficulty(int board[BOARD_SIZE]);
void swap_two_items_in_arr(int arr[BOARD_SIZE], int i, int j);
int squared_sum_of_three_nums(int i, int j, int k);
void print_arr(int arr[BOARD_SIZE]);

int main(void)
{
    test();
    
    int board[] = {1, 2, 3, 4, 5, 6, 
                    7, 8, 9, 10, 11, 12, 
                    13, 14, 15, 16, 17, 
                    18, 19, 20};
   
    int current_board_difficulty;
    
    for (int i=0; i <SEARCH_NUM; i++){
        current_board_difficulty = get_board_difficulty(board);
        
        int rand1 = rand()%BOARD_SIZE; 
        int rand2 = rand()%BOARD_SIZE; 
        
        swap_two_items_in_arr(board, rand1, rand2);
        
        int next_board_difficulty = get_board_difficulty(board);

        if (next_board_difficulty<current_board_difficulty){
           current_board_difficulty = next_board_difficulty;
        }
        else{
            // swap back to original arr
            swap_two_items_in_arr(board, rand1, rand2);
        }
    }
    printf("Total: %i \n", current_board_difficulty);
    print_arr(board);

    return 0;
}

void swap_two_items_in_arr(int arr[BOARD_SIZE], int i, int j)
{
    int temp = arr[j];
    arr[j] = arr[i];
    arr[i] = temp;
}

void print_arr(int arr[BOARD_SIZE])
{
    for (int i=0; i<BOARD_SIZE; i++){
        printf("%i ", arr[i]);
    }
    printf("\n");

}

int get_board_difficulty(int board[BOARD_SIZE])
{
    int difficulty = 0;

    for(int i=0; i<BOARD_SIZE; i++)
    {
        // I think you could also do this with a %
        // but this seemed clearer.
        if (i==BOARD_SIZE-1){
             difficulty += squared_sum_of_three_nums(
                board[i], 
                board[0], 
                board[1]
            );
        }
        else if(i==BOARD_SIZE-2){
            difficulty += squared_sum_of_three_nums(
                board[i], 
                board[i+1], 
                board[0]
            );
        }
        else {
            difficulty += squared_sum_of_three_nums(
                board[i], 
                board[i+1], 
                board[i+2]
            );
        }
    }
    return difficulty;
}

int squared_sum_of_three_nums(int i, int j, int k)
{
    int sum = i+j+k;
    return sum * sum;
}


void test(void)
{
    int london[] = {20, 1, 18, 4, 13, 
                    6, 10, 15, 2, 17,
                    3, 19, 7, 16, 8, 11,
                    14, 9, 12, 5};

    assert(get_board_difficulty(london)==20478);
}
