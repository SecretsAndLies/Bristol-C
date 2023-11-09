#include "8q.h"

int main(void)
{
    // todo eventually you'll need to use the command line args.
    // two possible arguments the number and - verbose
    // eg: ./8q -verbose 6 or ./8q 6
    // int num_solutions = 0;
    // //bool is_verbose = true;
    // int size = 8;
    // Board start = create_empty_board(size);
    // Board boards[BOARDS_IN_LIST];
    // int num_boards = 0;

// // TODO take in the arguments. Get the number, and set the verbose flag.

    test();
}

/* Returns if it's possible to place a queen at the given
coordinates. */
// bool can_place_queen(int r, int c, Board b)
// {
//     if (!is_valid_cord(r, c, b)){
//         return false;
//     }   
//     // queen in square already.
//     if(b.grid[r][c]=='Q'){
//         return false;
//     }
//     if (queen_in_row(r,c,b)){
//         return false;
//     }
//     if (queen_in_col(r,c,b)){
//         return false;
//     }
//     if (queen_in_diagonals(r,c,b)){
//         return false;
//     }
//     return true;
// }

// TODO. you have a lot of functions that copy structs. 
// Using pointers might make all of them more efficient.
bool queen_in_row(int r, Board b)
{
    for (int i=0; i<b.size; i++){
        if(b.grid[r][i]=='Q'){
            return true;
        }
    }
    return false;
}

void test_queen_in_row(void){
    Board b;
    b.num_queens =8;
    b.size = 8;
    strcpy(b.grid[0], "-Q------");
    strcpy(b.grid[1], "--------");
    strcpy(b.grid[2], "---Q----");
    strcpy(b.grid[3], "-------Q");
    strcpy(b.grid[4], "--------");
    strcpy(b.grid[5], "--------");
    strcpy(b.grid[6], "--------");
    strcpy(b.grid[7], "--------");
    assert(queen_in_row(0,b));
    assert(queen_in_row(2,b));
    assert(queen_in_row(3,b));
    assert(queen_in_row(1,b)==false);

}

// bool queen_in_col(int c, Board b)
// {
//     // increase the row count until you go off the board.

//     // descrese the row count till you go off the board.
//     return false;
// }

// bool queen_in_diagonals(int r, int c, Board b)
// {
//     return false;
// }


void test_is_valid_cord(void)
{
    Board b;
    b.num_queens = 8;
    b.size = 8;
    assert(is_valid_cord(0,8,b)==false); // off the board
    assert(is_valid_cord(0,0,b));
    assert(is_valid_cord(7,0,b));
    assert(is_valid_cord(5,2,b));

}

bool is_valid_cord(int r, int c, Board b)
{
    if (r>=0 && c>=0 && r<b.size && c<b.size){
        return true;
    }
    return false;
}

bool are_boards_identical(Board b, Board c)
{
    if (b.num_queens!=c.num_queens){
        return false;
    }
     if (b.size!=c.size){
        return false;
    }
    for (int j=0; j<b.size; j++){
        for (int i=0; i<b.size; i++){
            if(b.grid[j][i]!=c.grid[j][i]){
                return false;
            }
        }
    }
    return true;
}

// TODO create a testable version?
void print_board_string(Board b)
{
    // loop through every row in each collumns. 
    // If a queen is present print the row.
    for (int c=0; c<b.size; c++){
        for (int r=0; r<b.size; r++){
            if(b.grid[r][c]=='Q'){
                printf("%i",b.size-r);
            }
        }
    }
    printf("\n");
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
    assert(are_boards_identical(example8,example8)); 

    Board example8_2;
    example8_2.num_queens =8;
    example8_2.size = 8;
    strcpy(example8_2.grid[0], "Q-------");
    strcpy(example8_2.grid[1], "------Q-");
    strcpy(example8_2.grid[2], "----Q---");
    strcpy(example8_2.grid[3], "-------Q");
    strcpy(example8_2.grid[4], "-Q------");
    strcpy(example8_2.grid[5], "---Q----");
    strcpy(example8_2.grid[6], "-----Q--");
    strcpy(example8_2.grid[7], "--Q-----");

    assert(are_boards_identical(example8,example8_2)==false); 
}

Board create_empty_board(int size)
{
    Board b;
    b.num_queens = 0;
    b.size = size;
    for (int r=0; r<size; r++){
        for (int c=0; c<size; c++){
            b.grid[r][c]='-';
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

    are_boards_identical(b, example8);

}

void test_can_place_queen(void)
{
    // Board b;
    // b.num_queens =8;
    // b.size = 8;
    // strcpy(b.grid[0], "Q-------");
    // strcpy(b.grid[1], "--------");
    // strcpy(b.grid[2], "--------");
    // strcpy(b.grid[3], "--------");
    // strcpy(b.grid[4], "--------");
    // strcpy(b.grid[5], "--------");
    // strcpy(b.grid[6], "--------");
    // strcpy(b.grid[7], "--------");
    
    // assert(can_place_queen(1,7,b)); // nothing in row, col, or diagonal
    // assert(can_place_queen(0,8,b)==false); // off the board
    // assert(can_place_queen(2,0,b)==false); // off the board
    // assert(can_place_queen(0,0,b)==false); // Q already there.
    // assert(can_place_queen(0,6,b)==false); // Q in col (right above)
    // assert(can_place_queen(1,0,b)==false); // Q in this row.
    // assert(can_place_queen(2,0,b)==false); // Q in this row.


    }

void test(void)
{
    test_are_boards_indentical();
    test_create_empty_board();
    test_is_valid_cord();
    test_queen_in_row();
    // test_queen_in_diagonals();
    // test_queen_in_col();
    // test_can_place_queen();

}
