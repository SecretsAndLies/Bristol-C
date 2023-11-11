#include "8q.h"

/*
TODO:
- Speed
- creating a "solutions_data" struct that stores the board list
- returning a string from solutions. This allows testing.
- Testing
- clean up formatting / style.
*/

int main(int argc, char* argv[]) 
{
    test();
    validate_arg_count(argc);
    bool is_verbose = false;
    int size;
    parse_args(argc, argv, &size, &is_verbose);
    int solutions = get_solutions(is_verbose, size);
    printf("%i solutions \n", solutions);
    return 0;
}

/* Returns the number of solutions.
Prints the boards if the verbose flag is set. */
int get_solutions(int is_verbose, int size) 
{
    int sols = 0;

    static Board boards[BOARD_NUM];
    int start = 0;
    int end = 1;
    boards[start] = create_empty_board(size);

    while (start < end) {
        end = add_child_boards(&boards[start], boards, end, &sols, is_verbose);
        start++;
    }
    return sols;
}

/* To avoid the program runnning too slowly for the user
 we only test from 1 - 7. */
void test_get_solutions(void) 
{
    assert(get_solutions(false, 1) == 1);
    assert(get_solutions(false, 2) == 0);
    assert(get_solutions(false, 3) == 0);
    assert(get_solutions(false, 4) == 2);
    assert(get_solutions(false, 5) == 10);
    assert(get_solutions(false, 6) == 4);
    assert(get_solutions(false, 7) == 40);
    // TODO remove.
    // assert(get_solutions(false, 8)==92);
    // assert(get_solutions(false, 9)==352);
    // assert(get_solutions(false, 10)==724);
}

void validate_arg_count(int argc) 
{
    if (argc < 2 || argc > 3) {
        on_error("Error: wrong number of args. \n" CORRECT_USAGE);
    }
}

void on_error(const char* s) 
{
    fprintf(stderr, "%s\n", s);
    exit(EXIT_FAILURE);
}

// TODO: this function doesn't work.
//    I think it's because the argv isn't correctly copied across to the fucntion?
void test_parse_args(void) 
{
    // correct usage (verbose)
    int argc = 3;
    int size = 0;
    bool is_verbose = false;
    char argv[MAX_ARGS][STRING_LEN] = {"./8q", "-verbose", "6"};
    // the below line is needed because argv
    // is an array of pointers,
    // not a 2d array of strings.
    printf("%s \n", argv[1]);
    char* ptrArray[MAX_ARGS];
    parse_args(argc, ptrArray, &size, &is_verbose);
    assert(size == 6);
    assert(is_verbose == true);

    // correct usage non verbose
    argc = 2;
    size = 0;
    is_verbose = false;
    strcpy(argv[1], "10");
    parse_args(argc, ptrArray, &size, &is_verbose);
    assert(size == 10);
    assert(is_verbose == false);
}

// 3 strings, 0, false
void parse_args(int argc, char* argv[], int* size, bool* is_vebose) 
{
    if (argc == 2) {
        *size = convert_and_verify_size(argv[1]);
        if (*size == 0) {
            on_error(
                    "Error: invalid size -"
                    " must be between 1 and 10. \n" CORRECT_USAGE);
        }
    }
    if (argc == 3) {
        if (strcmp("-verbose", argv[1]) != 0) {
            on_error("Error, only valid flag is -verbose. \n" CORRECT_USAGE);
        }
        *size = convert_and_verify_size(argv[2]);
        if (*size == 0) {
            on_error(
                    "Error: invalid size -"
                    " must be between 1 and 10. \n" CORRECT_USAGE);
        }
        *is_vebose = true;
    }
}

void test_convert_and_verify_size(void) 
{
    assert(convert_and_verify_size("7") == 7);
    assert(convert_and_verify_size("0") == 0);
    assert(convert_and_verify_size("11") == 0);
    assert(convert_and_verify_size("1.1") == 1);
    assert(convert_and_verify_size("-flag") == 0);
}

/* Converts the string arg to a number,
 * Returns 0 if invalid number is entered. */
int convert_and_verify_size(char* size_str) 
{
    // atoi returns 0 if it can't do the convert, so this
    // does do some (possibly unspected)
    // things, eg: converts 1.1
    // to 1, but I think reasonable for now.
    int size = atoi(size_str);
    if (size <= 0 || size > 10) {
        return 0;
    }
    return size;
}

int add_child_boards(Board* b, Board boards[BOARD_NUM], int next_index,
                                         int* num_solutions, bool is_verbose) 
{
    for (int r = 0; r < b->size; r++) {
        for (int c = 0; c < b->size; c++) {
            next_index = add_child_board(b, boards, next_index, num_solutions,
                                                                     is_verbose, r, c);
        }
    }
    return next_index;
}

/* Adds a single child board to the list,
 * iff adding a queen at row, col creates a
 * valid, unique board.
 * Returns the next empty index. Increments num_solutions.*/
int add_child_board(Board* b, Board boards[BOARD_NUM], int next_index,
                                        int* num_solutions, bool is_verbose, int r, int c) 
{
    if (can_place_queen(r, c, b)) {
        Board temp = copy_board(*b);
        temp.grid[r][c] = 'Q';
        temp.num_queens++;
        if (board_is_unique(&temp, boards, next_index)) {
            boards[next_index] = temp;
            next_index++;
            if (is_solved_board(temp)) {
                if (is_verbose) {
                    print_board_string(&temp);
                }
                (*num_solutions)++;
            }
        }
    }
    return next_index;
}

void test_add_child_board(void)
{
    Board boards[SMALL_LIST];
    Board c;    // solved board.
    c.num_queens = 8;
    c.size = 8;
    strcpy(c.grid[0], "Q-------");
    strcpy(c.grid[1], "------Q-");
    strcpy(c.grid[2], "----Q---");
    strcpy(c.grid[3], "-------Q");
    strcpy(c.grid[4], "-Q------");
    strcpy(c.grid[5], "---Q----");
    strcpy(c.grid[6], "-----Q--");
    strcpy(c.grid[7], "--Q-----");

    // no new boards should be added.
    assert(add_child_board(&c, boards, 0, 0, false, 0, 1) == 0);
    // modify the board to have one solution left.
    c.num_queens = 7;
    strcpy(c.grid[7], "--------");
    int num_solutions = 0;
    assert(add_child_board(&c, boards, 0, &num_solutions, false, 7, 2) == 1);
    assert(boards[0].grid[7][2] == 'Q');
    assert(num_solutions == 1);
}

bool board_is_unique(Board* a, Board boards[BOARD_NUM], int size) 
{
    for (int i = 0; i < size; i++) {
        if (are_boards_identical(a, &boards[i])) {
            return false;
        }
    }
    return true;
}

void test_board_is_unique(void) 
{
    Board boards[SMALL_LIST];
    Board c;
    c.num_queens = 8;
    c.size = 8;
    strcpy(c.grid[0], "Q-------");
    strcpy(c.grid[1], "------Q-");
    strcpy(c.grid[2], "----Q---");
    strcpy(c.grid[3], "-------Q");
    strcpy(c.grid[4], "-Q------");
    strcpy(c.grid[5], "---Q----");
    strcpy(c.grid[6], "-----Q--");
    strcpy(c.grid[7], "--Q-----");

    boards[0] = c;
    // since you've already added this board
    // should be false.
    assert(board_is_unique(&c, boards, 1) == false);
    Board d = copy_board(c);
    strcpy(d.grid[7], "--------");
    // now we've modified c to be a
    // different board and test again.
    assert(board_is_unique(&d, boards, 1));
    boards[1] = d;
    boards[2] = create_empty_board(8);
    boards[3] = create_empty_board(8);
    boards[4] = create_empty_board(8);
    // d is now in the list.
    assert(board_is_unique(&d, boards, 5) == false);
}

bool is_solved_board(Board b) 
{
    if (b.num_queens == b.size) {
        return true;
    }
    return false;
}

void test_is_solved_board(void) 
{
    Board c;
    c.num_queens = 4;
    c.size = 4;
    strcpy(c.grid[0], "--Q-");
    strcpy(c.grid[1], "Q---");
    strcpy(c.grid[2], "---Q");
    strcpy(c.grid[3], "-Q--");
    assert(is_solved_board(c));
    // modifying c.
    c.num_queens = 3;
    // technically this line isn't needed for test to pass
    // but future code might be different.
    strcpy(c.grid[2], "----");
    assert(is_solved_board(c) == false);
}

Board copy_board(Board b) 
{ 
    return b; 
}

void test_copy_board(void) 
{
    Board c;
    c.num_queens = 4;
    c.size = 4;
    strcpy(c.grid[0], "--Q-");
    strcpy(c.grid[1], "Q---");
    strcpy(c.grid[2], "---Q");
    strcpy(c.grid[3], "-Q--");

    Board d = copy_board(c);
    // check that these boards are
    // pointing to a different spot in memory
    assert(&d != &c);
    // Do they have the same value.
    assert(are_boards_identical(&d, &c));
}

void print_board(Board b) 
{
    printf("================y\n");
    printf("Size: %i\n", b.size);
    printf("Queens: %i\n", b.num_queens);
    for (int r = 0; r < b.size; r++) {
        for (int c = 0; c < b.size; c++) {
            printf("%c", b.grid[r][c]);
        }
        printf("\n");
    }
    printf("\n");
    printf("================\n");
}

void test_add_child_boards(void) 
{
    // this is a solved board.
    Board boards[SMALL_LIST];
    Board c;
    c.num_queens = 8;
    c.size = 8;
    strcpy(c.grid[0], "Q-------");
    strcpy(c.grid[1], "------Q-");
    strcpy(c.grid[2], "----Q---");
    strcpy(c.grid[3], "-------Q");
    strcpy(c.grid[4], "-Q------");
    strcpy(c.grid[5], "---Q----");
    strcpy(c.grid[6], "-----Q--");
    strcpy(c.grid[7], "--Q-----");
    int nxt = 0;
    int sols = 0;
    assert(!add_child_boards(&c, boards, nxt, &sols, false));
    assert(sols == 0);

    // board with only one child board.
    Board d;
    d.num_queens = 7;
    d.size = 8;
    strcpy(d.grid[0], "Q-------");
    strcpy(d.grid[1], "------Q-");
    strcpy(d.grid[2], "----Q---");
    strcpy(d.grid[3], "-------Q");
    strcpy(d.grid[4], "-Q------");
    strcpy(d.grid[5], "---Q----");
    strcpy(d.grid[6], "-----Q--");
    strcpy(d.grid[7], "--------");
    nxt = 0;
    sols = 0;
    nxt = add_child_boards(&d, boards, nxt, &sols, false);
    assert(nxt == 1);
    assert(boards[0].grid[7][2] == 'Q');
    assert(sols == 1);

    // boards2
    // Empty board (checks that it has created n*n boards)
    // and spot checks some.
    Board e = create_empty_board(3);
    nxt = 0;
    sols = 0;
    nxt = add_child_boards(&e, boards, nxt, &sols, false);
    assert(nxt == 9);
    assert(sols == 0);
}

/* Returns if it's possible to place a queen at the given
coordinates. */
bool can_place_queen(int r, int c, Board* b) 
{
    if (!is_valid_cord(r, c, b)) {
        return false;
    }
    // optimization: if no queens, any placement is valid.
    if (b->num_queens == 0) {
        return true;
    }
    // queen in square already.
    if (b->grid[r][c] == 'Q') {
        return false;
    }
    if (queen_in_row(r, b)) {
        return false;
    }
    if (queen_in_col(c, b)) {
        return false;
    }
    if (queen_in_diagonals(r, c, b)) {
        return false;
    }
    return true;
}

bool queen_in_row(int r, Board* b) 
{
    for (int i = 0; i < b->size; i++) {
        if (b->grid[r][i] == 'Q') {
            return true;
        }
    }
    return false;
}

void test_queen_in_row(void) 
{
    Board b;
    b.num_queens = 8;
    b.size = 8;
    strcpy(b.grid[0], "-Q------");
    strcpy(b.grid[1], "--------");
    strcpy(b.grid[2], "---Q----");
    strcpy(b.grid[3], "-------Q");
    strcpy(b.grid[4], "--------");
    strcpy(b.grid[5], "--------");
    strcpy(b.grid[6], "--------");
    strcpy(b.grid[7], "--------");
    assert(queen_in_row(0, &b));
    assert(queen_in_row(2, &b));
    assert(queen_in_row(3, &b));
    assert(queen_in_row(1, &b) == false);
}

bool queen_in_col(int c, Board* b) 
{
    for (int i = 0; i < b->size; i++) {
        if (b->grid[i][c] == 'Q') {
            return true;
        }
    }
    return false;
}

void test_queen_in_col(void) 
{
    Board b;
    b.num_queens = 8;
    b.size = 8;
    strcpy(b.grid[0], "-Q------");
    strcpy(b.grid[1], "--------");
    strcpy(b.grid[2], "---Q----");
    strcpy(b.grid[3], "--------");
    strcpy(b.grid[4], "--------");
    strcpy(b.grid[5], "--------");
    strcpy(b.grid[6], "--------");
    strcpy(b.grid[7], "-------Q");
    assert(queen_in_col(0, &b) == false);
    assert(queen_in_col(1, &b));
    assert(queen_in_col(3, &b));
    assert(queen_in_col(4, &b) == false);
    assert(queen_in_col(7, &b));
}

bool queen_in_diagonals(int orig_r, int orig_c, Board* b) 
{
    // up and left, up and right, down and left, down and right
    int directions[NUM_DIAGONALS][ROW_COL] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for (int d = 0; d < NUM_DIAGONALS; d++) {
        int r = orig_r;
        int c = orig_c;
        while (is_valid_cord(r, c, b)) {
            if (b->grid[r][c] == 'Q') {
                return true;
            }
            r += directions[d][0];
            c += directions[d][1];
        }
    }

    return false;
}

void test_queen_in_diagonals(void) 
{
    Board b = create_empty_board(8);
    strcpy(b.grid[2], "--Q-----");
    assert(queen_in_diagonals(0, 0, &b));    // down and right
    assert(queen_in_diagonals(4, 0, &b));    // up and right
    assert(queen_in_diagonals(0, 4, &b));    // down and left
    assert(queen_in_diagonals(4, 4, &b));    // up and left
    assert(queen_in_diagonals(3, 2, &b) == false);

    Board c = create_empty_board(10);
    assert(queen_in_diagonals(0, 0, &c) == false);    // board is empty
    strcpy(c.grid[0], "Q---------");
    assert(queen_in_diagonals(9, 9, &c) == true);    // up and left
    strcpy(c.grid[0], "---------Q");
    assert(queen_in_diagonals(9, 0, &c) == true);    // up and right
    strcpy(c.grid[0], "----------");                             // reset old top
    strcpy(c.grid[9], "Q---------");
    assert(queen_in_diagonals(0, 9, &c) == true);    // down and left
    strcpy(c.grid[9], "---------Q");
    assert(queen_in_diagonals(0, 0, &c) == true);    // down and right
}

void test_is_valid_cord(void) 
{
    Board b;
    b.num_queens = 8;
    b.size = 8;
    assert(is_valid_cord(0, 8, &b) == false);    // off the board
    assert(is_valid_cord(0, 0, &b));
    assert(is_valid_cord(7, 0, &b));
    assert(is_valid_cord(5, 2, &b));
}

bool is_valid_cord(int r, int c, Board* b) 
{
    if (r >= 0 && c >= 0 && r < b->size && c < b->size) {
        return true;
    }
    return false;
}

bool are_boards_identical(Board* b, Board* c) 
{
    if (b->num_queens != c->num_queens) {
        return false;
    }
    if (b->size != c->size) {
        return false;
    }
    for (int j = 0; j < b->size; j++) {
        for (int i = 0; i < b->size; i++) {
            if (b->grid[j][i] != c->grid[j][i]) {
                return false;
            }
        }
    }
    return true;
}

void board2str(Board* b, char line[STRING_LEN]) 
{
    // loop through every row in each collumns.
    // If a queen is present write the row to arr.
    int i = 0;
    for (int c = 0; c < b->size; c++) {
        for (int r = 0; r < b->size; r++) {
            if (b->grid[r][c] == 'Q') {
                line[i] = (b->size - r) + '0';
                i++;
            }
        }
    }
    line[i] = '\0';
}

void test_board2str(void) 
{
    char line[STRING_LEN];
    Board b;
    b.num_queens = 8;
    b.size = 8;
    strcpy(b.grid[0], "Q-------");
    strcpy(b.grid[1], "------Q-");
    strcpy(b.grid[2], "----Q---");
    strcpy(b.grid[3], "-------Q");
    strcpy(b.grid[4], "-Q------");
    strcpy(b.grid[5], "---Q----");
    strcpy(b.grid[6], "-----Q--");
    strcpy(b.grid[7], "--Q-----");
    board2str(&b, line);
    assert(strcmp("84136275", line) == 0);
}

void print_board_string(Board* b) 
{
    char line[STRING_LEN];
    board2str(b, line);
    printf("%s\n", line);
}

void test_are_boards_indentical(void) 
{
    Board example8;
    example8.num_queens = 8;
    example8.size = 8;
    strcpy(example8.grid[0], "----------");
    strcpy(example8.grid[1], "----------");
    strcpy(example8.grid[2], "----------");
    strcpy(example8.grid[3], "----------");
    strcpy(example8.grid[4], "----------");
    strcpy(example8.grid[5], "----------");
    strcpy(example8.grid[6], "----------");
    strcpy(example8.grid[7], "----------");
    strcpy(example8.grid[8], "----------");
    strcpy(example8.grid[9], "----------");
    assert(are_boards_identical(&example8, &example8));

    Board example8_2;
    example8_2.num_queens = 8;
    example8_2.size = 8;
    strcpy(example8_2.grid[0], "Q-------");
    strcpy(example8_2.grid[1], "------Q-");
    strcpy(example8_2.grid[2], "----Q---");
    strcpy(example8_2.grid[3], "-------Q");
    strcpy(example8_2.grid[4], "-Q------");
    strcpy(example8_2.grid[5], "---Q----");
    strcpy(example8_2.grid[6], "-----Q--");
    strcpy(example8_2.grid[7], "--Q-----");

    assert(are_boards_identical(&example8, &example8_2) == false);
}

Board create_empty_board(int size) 
{
    Board b;
    b.num_queens = 0;
    b.size = size;
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            b.grid[r][c] = '-';
        }
    }
    return b;
}

void test_create_empty_board(void) 
{
    Board b = create_empty_board(8);

    Board example8;
    example8.num_queens = 0;
    example8.size = 8;
    strcpy(example8.grid[0], "----------");
    strcpy(example8.grid[1], "----------");
    strcpy(example8.grid[2], "----------");
    strcpy(example8.grid[3], "----------");
    strcpy(example8.grid[4], "----------");
    strcpy(example8.grid[5], "----------");
    strcpy(example8.grid[6], "----------");
    strcpy(example8.grid[7], "----------");
    strcpy(example8.grid[8], "----------");
    strcpy(example8.grid[9], "----------");

    assert(are_boards_identical(&b, &example8));
}

void test_can_place_queen(void) 
{
    Board b;
    b.num_queens = 8;
    b.size = 8;
    strcpy(b.grid[0], "Q-------");
    strcpy(b.grid[1], "--------");
    strcpy(b.grid[2], "--------");
    strcpy(b.grid[3], "--------");
    strcpy(b.grid[4], "--------");
    strcpy(b.grid[5], "--------");
    strcpy(b.grid[6], "--------");
    strcpy(b.grid[7], "--------");

    assert(can_place_queen(1, 7, &b));    // nothing in row, col, or diagonal
    assert(can_place_queen(6, 3, &b));    // nothing in row, col, or diagonal
    assert(can_place_queen(0, 8, &b) == false);    // off the board
    assert(can_place_queen(2, 0, &b) == false);    // off the board
    assert(can_place_queen(0, 0, &b) == false);    // Q already there.
    assert(can_place_queen(0, 6, &b) == false);    // Q in col (right above)
    assert(can_place_queen(1, 0, &b) == false);    // Q in this col.
    assert(can_place_queen(2, 0, &b) == false);    // Q in this row.

    Board c;
    c.num_queens = 8;
    c.size = 8;
    strcpy(c.grid[0], "Q-------");
    strcpy(c.grid[1], "------Q-");
    strcpy(c.grid[2], "----Q---");
    strcpy(c.grid[3], "-------Q");
    strcpy(c.grid[4], "-Q------");
    strcpy(c.grid[5], "---Q----");
    strcpy(c.grid[6], "-----Q--");
    strcpy(c.grid[7], "--Q-----");
    // this board is already solved,
    // so whereever you put a queen it should be false.
    assert(can_place_queen(0, 0, &c) == false);
    assert(can_place_queen(3, 0, &c) == false);
    assert(can_place_queen(8, 0, &c) == false);
    assert(can_place_queen(0, 2, &c) == false);
    assert(can_place_queen(3, 1, &c) == false);
    assert(can_place_queen(4, 2, &c) == false);
    assert(can_place_queen(7, 4, &c) == false);
    assert(can_place_queen(1, 1, &c) == false);
    assert(can_place_queen(4, 3, &c) == false);
}

void test(void) 
{
    test_are_boards_indentical();
    test_create_empty_board();
    test_is_valid_cord();
    // test_parse_args();
    test_queen_in_row();
    test_queen_in_col();
    test_queen_in_diagonals();
    test_add_child_boards();
    test_add_child_board();
    test_is_solved_board();
    test_board_is_unique();
    test_convert_and_verify_size();
    test_can_place_queen();
    test_copy_board();
    test_board2str();
    test_get_solutions();
}
