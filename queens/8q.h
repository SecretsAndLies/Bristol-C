#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BOARD 11
#define BOARDS_IN_LIST 10000000 //1M - 10M didn't seem to solve bug.
#define SMALL_LIST 100

typedef struct Board{
    int num_queens;
    int size;
    char grid[MAX_BOARD][MAX_BOARD];
} Board;

void test(void);

void test_are_boards_indentical(void);
bool are_boards_identical(Board * b, Board * c);

void parse_args(int argc, char * argv[], int * size, bool * is_vebose);

int convert_and_verify_size(char * size_str);

void print_board_string(Board * b);

void print_board(Board b);

Board copy_board(Board b);

bool is_solved_board(Board b);

void prompt_correct_usage(void);

Board create_empty_board(int size);
void test_create_empty_board(void);

bool board_is_unique(Board * a, Board boards[BOARDS_IN_LIST], int size);
void test_board_is_unique(void);

bool is_valid_cord(int r, int c, Board * b);
void test_is_valid_cord(void);

bool queen_in_row(int r, Board * b);
void test_queen_in_row(void);

bool queen_in_col(int c, Board * b);
void test_queen_in_col(void);

bool queen_in_diagonals(int r, int c, Board * b);
void test_queen_in_diagonals(void);

bool can_place_queen (int r, int c, Board * b);
void test_can_place_queen(void);

int add_child_boards(Board * b, Board boards[BOARDS_IN_LIST], int next_index, int * num_solutions, bool is_verbose);
void test_add_child_boards(void);

int add_child_board(Board * b, Board boards[BOARDS_IN_LIST], int next_index, int * num_solutions, bool is_verbose, int r, int c);
