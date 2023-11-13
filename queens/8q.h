#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOARD 11
#define BOARD_NUM 10000000
#define SMALL_LIST 100
#define MEDIUM_LIST 1000
#define STRING_LEN 2000
#define MAX_ARGS 5 
#define CORRECT_USAGE "Usage:\n ./8q 6 \n ./8q -verbose 3 \n"
#define NUM_DIAGONALS 4
#define ROW_COL 2

typedef struct Board {
  int num_queens;
  int size;
  char grid[MAX_BOARD][MAX_BOARD];
} Board;

void test(void);

void on_error(const char* s);

int get_solutions(int size, Board solutions[SMALL_LIST]);
void test_get_solutions(void);

void validate_arg_count(int argc);

void test_are_boards_indentical(void);
bool are_boards_identical(Board* b, Board* c);

void parse_args(int argc, char* argv[], int* size, bool* is_vebose);
void test_parse_args(void);

void test_convert_and_verify_size(void);
int convert_and_verify_size(char* size_str);

void print_board_string(Board* b);
void board2str(Board* b, char line[STRING_LEN]);
void test_board2str(void);

// no test required, this is a testing function.
void print_board(Board b);

Board copy_board(Board b);
void test_copy_board(void);

bool is_solved_board(Board b);
void test_is_solved_board(void);

void prompt_correct_usage(void);

Board create_empty_board(int size);
void test_create_empty_board(void);

bool board_is_unique(Board* a, Board boards[BOARD_NUM], int size);
void test_board_is_unique(void);

bool is_valid_cord(int r, int c, Board* b);
void test_is_valid_cord(void);

bool queen_in_row(int r, Board* b);
void test_queen_in_row(void);

bool queen_in_col(int c, Board* b);
void test_queen_in_col(void);

bool queen_in_diagonals(int r, int c, Board* b);
void test_queen_in_diagonals(void);

bool can_place_queen(int r, int c, Board* b);
void test_can_place_queen(void);

int add_child_boards(Board* b, 
                    Board boards[BOARD_NUM], 
                    int next_index,
                    int* num_solutions, 
                    Board solutions[SMALL_LIST]);
void test_add_child_boards(void);

int add_child_board(Board* b, 
                    Board boards[BOARD_NUM], 
                    int next_index,
                    int* num_solutions, 
                    int r, int c, 
                    Board solutions[SMALL_LIST]);
void test_add_child_board(void);
