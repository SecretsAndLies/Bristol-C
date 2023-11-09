#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BOARD 11
#define BOARDS_IN_LIST 1000000 //1M - 10M was causing issues in testing.
// if I do need the full 10M, might be good to change 
//some of the test arrays since they don't need to be so large.

typedef struct Board{
    int num_queens;
    int size;
    char grid[MAX_BOARD][MAX_BOARD];
} Board;

void test(void);

void test_are_boards_indentical(void);
bool are_boards_identical(Board b, Board c);

void print_board_string(Board b);

void print_board(Board b);

Board copy_board(Board source);

bool is_solved_board(Board b);

Board create_empty_board(int size);
void test_create_empty_board(void);

bool board_is_unique(Board a, Board boards[BOARDS_IN_LIST], int size);

bool is_valid_cord(int r, int c, Board b);
void test_is_valid_cord(void);

bool queen_in_row(int r, Board b);
void test_queen_in_row(void);

bool queen_in_col(int c, Board b);
void test_queen_in_col(void);

bool queen_in_diagonals(int r, int c, Board b);
void test_queen_in_diagonals(void);

bool can_place_queen (int r, int c, Board b);
void test_can_place_queen(void);

int add_child_boards(Board b, Board boards[BOARDS_IN_LIST], int next_index, int * num_solutions);
void test_add_child_boards(void);

