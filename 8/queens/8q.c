#include "8q.h"

/* Returns the number of solutions and populates
the given array with solution boards. */
int get_solutions(int size, Board solutions[MEDIUM_LIST]) 
{
    int sols = 0;

    static Board boards[BOARD_NUM];
    int start = 0;
    int end = 1;
    boards[start] = create_empty_board(size);

    while (start < end) {
        end = add_child_boards(&boards[start], 
                                boards, 
                                end, 
                                &sols, 
                                solutions,
                                start);
        start++;
    }

    return sols;
}

/* To avoid the program runnning too slowly for the user
 we only test boards 1 - 7. We spot check returning boards
 along the way. */
void test_get_solutions(void) 
{
    Board solutions[MEDIUM_LIST];
    assert(get_solutions(1,solutions) == 1);
    assert(solutions[0].grid[0][0] == QUEEN);
    assert(solutions[0].num_queens == 1);
    assert(solutions[0].size == 1);

    assert(get_solutions(2,solutions) == 0);
    assert(get_solutions(3,solutions) == 0);

    assert(get_solutions(4,solutions) == 2);
    assert(solutions[0].grid[0][0] == BLANK);
    Board d;
    d.num_queens = 4;
    d.size = 4;
    strcpy(d.grid[0], "-Q--");
    strcpy(d.grid[1], "---Q");
    strcpy(d.grid[2], "Q---");
    strcpy(d.grid[3], "--Q-");
    assert(are_boards_identical(&d, &solutions[0]));
    Board e;
    e.num_queens = 4;
    e.size = 4;
    strcpy(e.grid[0], "--Q-");
    strcpy(e.grid[1], "Q---");
    strcpy(e.grid[2], "---Q");
    strcpy(e.grid[3], "-Q--");
    assert(are_boards_identical(&e, &solutions[1]));

    assert(get_solutions(5,solutions) == 10);
    assert(get_solutions(6,solutions) == 4);

    assert(get_solutions(7,solutions) == 40);
    Board f;
    f.num_queens = 7;
    f.size = 7;
    strcpy(f.grid[0],"Q------");
    strcpy(f.grid[1],"--Q----");
    strcpy(f.grid[2],"----Q--");
    strcpy(f.grid[3],"------Q");
    strcpy(f.grid[4],"-Q-----");
    strcpy(f.grid[5],"---Q---");
    strcpy(f.grid[6],"-----Q-");
    assert(are_boards_identical(&f, &solutions[0]));
}

void validate_arg_count(int argc) 
{
    if (argc < 2 || argc > 3) {
        on_error("Error: wrong number of args. \n" 
                CORRECT_USAGE);
    }
}

void on_error(const char* s) 
{
    fprintf(stderr, "%s\n", s);
    exit(EXIT_FAILURE);
}

void test_parse_args(void) 
{
    // correct usage (verbose)
    int argc = 3;
    int size = 0;
    bool is_verbose = false;
    char args[MAX_ARGS][SMALL_STR];
    char * argv[SMALL_STR];
    strcpy(args[0],"./8q");
    strcpy(args[1],"-verbose");
    strcpy(args[2],"6");
    for (int i = 0; i<MAX_ARGS; i++) {
        argv[i] = args[i];
    }

    parse_args(argc, argv, &size, &is_verbose);
    assert(size == 6);
    assert(is_verbose == true);

    // correct usage non verbose
    argc = 2;
    size = 0;
    is_verbose = false;
    strcpy(args[1], "10");
    parse_args(argc, argv, &size, &is_verbose);
    assert(size == 10);
    assert(is_verbose == false);

    // incorrect usage causes errors, so is tested in 
    // the bash script.
}

void parse_args(int argc, char* argv[], 
                int* size, bool* is_vebose) 
{
    if (argc == 2) {
        *size = convert_and_verify_size(argv[1]);
        if (*size == 0) {
            on_error(
                    "Error: invalid size -"
                    " must be between 1 and 10. \n" 
                    CORRECT_USAGE);
        }
    }
    if (argc == 3) {
        if (strcmp("-verbose", argv[1]) != 0) {
            on_error("Error, only valid flag is -verbose. \n" 
            CORRECT_USAGE);
        }
        *size = convert_and_verify_size(argv[2]);
        if (*size == 0) {
            on_error(
                    "Error: invalid size -"
                    " must be between 1 and 10. \n" 
                    CORRECT_USAGE);
        }
        *is_vebose = true;
    }
}

void test_convert_and_verify_size(void) 
{
    assert(convert_and_verify_size("7") == 7);
    assert(convert_and_verify_size("10") == 10);
    assert(convert_and_verify_size("0") == 0);
    assert(convert_and_verify_size("11") == 0);
    assert(convert_and_verify_size("1.1") == 1);
    assert(convert_and_verify_size("-flag") == 0);
}

/* Converts the string arg to a number,
 * Returns 0 if invalid number is entered. */
int convert_and_verify_size(char* size_str) 
{
    int size;
    if(sscanf(size_str, "%i", &size) != 1) {
        return 0;
    }
    if (size <= 0 || size > 10) {
        return 0;
    }
    return size;
}

int add_child_boards(Board* b, 
                    Board boards[BOARD_NUM], 
                    int next_index,
                    int* num_solutions, 
                    Board solutions[MEDIUM_LIST],
                    int start) 
{
    // no action required for already solved boards.
    if (is_solved_board(*b)) {
        return next_index;
    }
    for (int r = 0; r < b->size; r++) {
        for (int c = 0; c < b->size; c++) {
            next_index = add_child_board(b, 
                                        boards, 
                                        next_index, 
                                        num_solutions, 
                                        r, c, 
                                        solutions,
                                        start);
        }
    }
    return next_index;
}

/* Adds a single child board to the list,
 * iff adding a queen at row, col creates a
 * valid, unique board.
 * Returns the next empty index in boards. 
 * Increments num_solutions.*/
int add_child_board(Board* b, 
                    Board boards[BOARD_NUM], 
                    int next_index,
                    int* num_solutions, 
                    int r, int c,
                    Board solutions[MEDIUM_LIST],
                    int start) 
{
    if (can_place_queen(r, c, b)) {
        Board temp = copy_board(*b);
        temp.grid[r][c] = QUEEN;
        temp.num_queens++;
        if (board_is_unique(&temp, boards, 
                            next_index, start)) {
            boards[next_index] = temp;
            next_index++;
            if (is_solved_board(temp)) {
                solutions[*num_solutions] = temp;
                (*num_solutions)++;
            }
        }
    }
    return next_index;
}

void test_add_child_board(void)
{
    Board boards[MEDIUM_LIST];
    Board solutions[MEDIUM_LIST];
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

    // no new boards should be added because board is solved.
    assert(add_child_board(&c, boards, 
                            0, 0, 0, 1, 
                            solutions, 0) == 0);
    // modify the board to have one solution left.
    c.num_queens = 7;
    strcpy(c.grid[7], "--------");
    int num_solutions = 0;
    // add a queen in the wrong position.
    assert(add_child_board( &c, 
                            boards, 
                            0, 
                            &num_solutions, 
                            0, 
                            1, 
                            solutions, 0) == 0);
    assert(num_solutions == 0);
    // add a queen in the right position.
    assert(add_child_board( &c, 
                            boards,
                            0, 
                            &num_solutions, 
                            7, 
                            2, 
                            solutions, 0) == 1);
    assert(boards[0].grid[7][2] == QUEEN);
    assert(num_solutions == 1);
}

bool board_is_unique(Board* a, Board boards[BOARD_NUM], 
                    int size, int start) 
{
    int i=size-1;
    // we loop through backwards, because
    // similar boards are most likely to be to the left
    // of this board.
    while (i>=start) {
        // stop the loop when the number of queens is
        // different to the board we're on.
        if (a->num_queens<boards[i].num_queens) {
            return false;
        }
        if (are_boards_identical(a, &boards[i])) {
            return false;
        }
        i--;
    }
    return true;
}

void test_board_is_unique(void) 
{
    Board boards[MEDIUM_LIST];
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
    assert(board_is_unique(&c, boards, 1, 0) == false);
    Board d = copy_board(c);
    strcpy(d.grid[7], "--------");
    // now we've modified c to be a
    // different board and test again.
    assert(board_is_unique(&d, boards, 1, 0));
    boards[1] = d;
    boards[2] = create_empty_board(8);
    boards[2].num_queens = 8;
    boards[3] = create_empty_board(8);
    boards[3].num_queens = 8;
    boards[4] = create_empty_board(8);
    boards[4].num_queens = 8;
    // d is now in the list.
    assert(board_is_unique(&d, boards, 5, 0) == false);

    // add 9x9 and 10x10 boards
    // The number of queens is important in these boards 
    // as it's used for the algorithm.
    boards[5] = create_empty_board(9);
    boards[5].num_queens = 9;
    boards[6] = create_empty_board(10);
    boards[6].num_queens = 10;
    assert(board_is_unique(&d, boards, 5, 0) == false);

    // don't start at zero and still find the board.
    assert(board_is_unique(&d, boards, 5, 1) == false);
    Board e = copy_board(d);
    // modify board, should now be unique.
    strcpy(d.grid[4], "--------");
    assert(board_is_unique(&e, boards, 5, 1) == false);
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
    Board d = create_empty_board(4);
    d.num_queens=4;
    assert(is_solved_board(d));
    // modifying c.
    d.num_queens = 3;
    assert(is_solved_board(d) == false);

    Board c = create_empty_board(8);
    assert(is_solved_board(c) == false);
    c.num_queens = 8;
    assert(is_solved_board(c));
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
    assert(&d.grid!=&c.grid);
    assert(&d.num_queens!=&c.num_queens);
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
    Board boards[MEDIUM_LIST];
    Board solutions[MEDIUM_LIST];
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
    assert(!add_child_boards(&c, boards, 
                            nxt, &sols, solutions, 0));
    assert(sols == 0);

    // board with only one child board. (modification of c)
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
    nxt = add_child_boards(&d, boards, nxt, 
                        &sols, solutions, 0);
    assert(nxt == 1);
    assert(boards[0].grid[7][2] == QUEEN);
    assert(sols == 1);
    assert(are_boards_identical(&c, &solutions[0]));

    // Empty board (checks that it has created n*n boards)
    // and spot checks first boards.
    Board e = create_empty_board(3);
    nxt = 0;
    sols = 0;
    nxt = add_child_boards(&e, boards, nxt, 
                            &sols, solutions, 0);
    assert(nxt == 9);
    assert(sols == 0);
    assert(boards[0].grid[0][0]==QUEEN);
    assert(boards[0].grid[0][1]==BLANK);
    assert(boards[1].grid[0][0]==BLANK);
    assert(boards[1].grid[0][1]==QUEEN);
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
    if (b->grid[r][c] == QUEEN) {
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
        if (b->grid[r][i] == QUEEN) {
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

    Board empty = create_empty_board(4);
    assert(queen_in_row(0, &empty) == false);
    assert(queen_in_row(1, &empty) == false);
    assert(queen_in_row(2, &empty) == false);
    assert(queen_in_row(3, &empty) == false);

    Board d;
    d.num_queens = 6;
    d.size = 6;
    strcpy(d.grid[0], "Q------");
    strcpy(d.grid[1], "-------");
    strcpy(d.grid[2], "-----Q-");
    strcpy(d.grid[3], "-------");
    strcpy(d.grid[4], "--Q----");
    strcpy(d.grid[5], "-------");
    
    assert(queen_in_row(0, &d));
    assert(queen_in_row(1, &d) == false);
    assert(queen_in_row(2, &d));
    assert(queen_in_row(3, &d) == false);
    assert(queen_in_row(4, &d));
    assert(queen_in_row(5, &d) == false);
}

bool queen_in_col(int c, Board* b) 
{
    for (int i = 0; i < b->size; i++) {
        if (b->grid[i][c] == QUEEN) {
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

    Board c;
    c.num_queens = 3;
    c.size = 9;
    strcpy(c.grid[0], "-Q-------");
    strcpy(c.grid[1], "---------");
    strcpy(c.grid[2], "---Q-----");
    strcpy(c.grid[3], "---------");
    strcpy(c.grid[4], "---------");
    strcpy(c.grid[5], "---------");
    strcpy(c.grid[6], "---------");
    strcpy(c.grid[7], "--------Q");
    strcpy(c.grid[8], "---------");
    strcpy(c.grid[9], "---------");

    assert(queen_in_col(0, &c) == false);
    assert(queen_in_col(1, &c));
    assert(queen_in_col(3, &c));
    assert(queen_in_col(4, &c) == false);
    assert(queen_in_col(8, &c));
}

bool queen_in_diagonals(int orig_r, int orig_c, Board* b) 
{
    int directions[NUM_DIAGONALS][ROW_COL] = 
                    {{UP_AND_LEFT}, {UP_AND_RIGHT}, 
                    {DOWN_AND_LEFT}, {DOWN_AND_RIGHT}};

    for (int d = 0; d < NUM_DIAGONALS; d++) {
        int r = orig_r;
        int c = orig_c;
        while (is_valid_cord(r, c, b)) {
            if (b->grid[r][c] == QUEEN) {
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
    // down and right
    assert(queen_in_diagonals(0, 0, &b));    
    // up and right
    assert(queen_in_diagonals(4, 0, &b));    
    // down and left
    assert(queen_in_diagonals(0, 4, &b));    
    // up and left
    assert(queen_in_diagonals(4, 4, &b));    
    assert(queen_in_diagonals(3, 2, &b) == false);

    Board c = create_empty_board(10);
    // board is empty so no queens anywhere.
    assert(queen_in_diagonals(0, 0, &c) == false);    
    assert(queen_in_diagonals(3, 2, &c) == false);    
    strcpy(c.grid[0], "Q---------");
    // up and left
    assert(queen_in_diagonals(9, 9, &c) == true);    
    assert(queen_in_diagonals(0, 9, &c) == false);    
    strcpy(c.grid[0], "---------Q");
    // up and right
    assert(queen_in_diagonals(9, 0, &c) == true);    
    assert(queen_in_diagonals(9, 9, &c) == false);    
    // reset old top
    strcpy(c.grid[0], "----------");                             
    strcpy(c.grid[9], "Q---------");
    // down and left
    assert(queen_in_diagonals(0, 9, &c) == true);    
    assert(queen_in_diagonals(9, 7, &c) == false);    
    strcpy(c.grid[9], "---------Q");
    // down and right
    assert(queen_in_diagonals(0, 0, &c) == true);   
}

void test_is_valid_cord(void) 
{
    Board b;
    b.num_queens = 8;
    b.size = 8;
    // off the board
    assert(is_valid_cord(0, 8, &b) == false);
    assert(is_valid_cord(8, 0, &b) == false);
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
    int i = 0;
    for (int c = 0; c < b->size; c++) {
        for (int r = 0; r < b->size; r++) {
            if (b->grid[r][c] == QUEEN) {
                int rank_pos = b->size - r;
                if(rank_pos==10) {
                    line[i] = 'A';
                }
                else {
                    line[i] = rank_pos + '0';
                }
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

    Board f = create_empty_board(10);
    f.num_queens = 10;  
    f.grid[0][4] = QUEEN;
    f.grid[1][9] = QUEEN;
    f.grid[2][3] = QUEEN;
    f.grid[3][8] = QUEEN;
    f.grid[4][2] = QUEEN;
    f.grid[5][7] = QUEEN;
    f.grid[6][1] = QUEEN;
    f.grid[7][6] = QUEEN;
    f.grid[8][0] = QUEEN;
    f.grid[9][5] = QUEEN;

    board2str(&f, line);
    assert(strcmp("2468A13579", line) == 0);

    Board c = create_empty_board(3);
    c.grid[0][0] = QUEEN;
    c.grid[2][1] = QUEEN;
    board2str(&c, line);
    assert(strcmp("31", line) == 0);

    Board d = create_empty_board(5);
    d.grid[2][1] = QUEEN;
    d.grid[1][3] = QUEEN;
    d.grid[0][0] = QUEEN;
    board2str(&d, line);
    assert(strcmp("534", line) == 0);
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

    assert(!are_boards_identical(&example8, &example8_2));

    Board a = create_empty_board(9);
    Board b = create_empty_board(9);
    assert(are_boards_identical(&a, &b) == true);
    a.grid[0][0] = QUEEN;
    a.num_queens++;
    assert(are_boards_identical(&a, &b) == false);
    b.grid[0][0] = QUEEN;
    b.num_queens++;
    assert(are_boards_identical(&a, &b) == true);
}

Board create_empty_board(int size) 
{
    Board b;
    b.num_queens = 0;
    b.size = size;
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            b.grid[r][c] = BLANK;
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

    Board b5 = create_empty_board(5);

    Board example5;
    example5.num_queens = 0;
    example5.size = 5;
    strcpy(example5.grid[0], "-----");
    strcpy(example5.grid[1], "-----");
    strcpy(example5.grid[2], "-----");
    strcpy(example5.grid[3], "-----");
    strcpy(example5.grid[4], "-----");

    assert(are_boards_identical(&b5, &example5));

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

    // nothing in row, col, or diagonal
    assert(can_place_queen(1, 7, &b)); 
    // nothing in row, col, or diagonal
    assert(can_place_queen(6, 3, &b));    
    // off the board
    assert(can_place_queen(0, 8, &b) == false);   
    // off the board
    assert(can_place_queen(2, 0, &b) == false);    
    // Q already there.
    assert(can_place_queen(0, 0, &b) == false);   
    // Q in col (right above)
    assert(can_place_queen(0, 6, &b) == false);    
    // Q in this col.
    assert(can_place_queen(1, 0, &b) == false);   
    // Q in this row.
    assert(can_place_queen(2, 0, &b) == false);   
    // Q in this diagonal.
    assert(can_place_queen(7, 7, &b) == false);   

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

    // reverse of above, all positions are valid.
    Board d = create_empty_board(10);
    for (int r = 0; r<d.size; r++) {
        for (int c = 0; c<d.size; c++) {
            assert(can_place_queen(r, c, &d));
        }
    }
}

void test(void) 
{
    test_are_boards_indentical();
    test_create_empty_board();
    test_is_valid_cord();
    test_parse_args();
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
