#include "8q.h"

int main(int argc, char * argv[])
{
    test();
    // todo eventually you'll need to use the command line args.
    // two possible arguments the number and - verbose
    // eg: ./8q -verbose 6 or ./8q 6
// // TODO take in the arguments. Get the size, and set the verbose flag.
    // //bool is_verbose = true;

    if(argc<2 || argc>3){
        prompt_correct_usage();
        return 1;
    }

    bool is_verbose=false;
    int size;
    parse_args(argc, argv, &size, &is_verbose);
    
    int num_solutions = 0;
    static Board boards[BOARDS_IN_LIST];
    int start_index = 0;
    int end_index = 1;
    boards[start_index]=create_empty_board(size);
    
    while (start_index<end_index){
        end_index = add_child_boards(boards[start_index], boards, end_index, &num_solutions, is_verbose);
        start_index++;
    }

    printf("%i solutions \n", num_solutions);
    return 0;
}

// TODO; idk if this needs testing...)
void parse_args(int argc, char * argv[], int * size, bool * is_vebose)
{
    // TODO move the size logic into it's own func since it's repeated.
    if (argc==2){
        *size = convert_and_verify_size(argv[1]);
    }
    if(argc==3){
        if(strcmp("-verbose", argv[1])!=0){
            prompt_correct_usage();
        }
        *size = convert_and_verify_size(argv[2]);
        *is_vebose = true; 
    }
}

// TODO test
int convert_and_verify_size(char * size_str)
{
    // atoi returns 0 if it can't do the convert, so this
    // also covers user doing weird inputs (eg floats, non numbers)
    int size = atoi(size_str);
    if(size<=0 || size>10){
        prompt_correct_usage();
        exit(EXIT_FAILURE);
    }
    return size;
}

void prompt_correct_usage(void)
{
        printf("You must supply one or two arguments. \n"
                "You must give the size of the board as an int between 1-10 \n"
                "Optionally, you can also add a verbose flag. \n"
                "Example usage: ./8q 6 OR ./8q -verbose 3 \n");
}

//    Board f = create_empty_board(8);
//    num_index = 0;
//    num_solutions=0;
//    add_child_boards(f, boards, next_index, &num_solutions);
//    assert(num_solutions==92)

/* Adds all possible next 
child boards from the specified board b
 to the given array.
 Returns the size of the array (ie the next_index)
 Also checks if a board is a solution and modifies that number.
  */
int add_child_boards(Board b, Board boards[BOARDS_IN_LIST], int next_index, int * num_solutions, bool is_verbose)
{
    for (int r=0; r<b.size; r++){
        for (int c=0; c<b.size; c++){
            // TODO: move this into own func to avoid nested ifs
            if(can_place_queen(r,c,b)){
                Board temp = copy_board(b);
                temp.grid[r][c]='Q';
                temp.num_queens++;
                if (board_is_unique(temp, boards, next_index)){
                    boards[next_index] = temp;
                    next_index++;
                    if(is_solved_board(temp)){
                        if(is_verbose){
                            print_board_string(temp);
                        }
                        (*num_solutions)++;
                    }                  
                }
            }
        }
    }
    return next_index;
}

// TODO: potentially createa a create_child_board funct.
// This should do the logic bit from add child board, and determine
// if you should add the board you just made (temp) to the array or not.
// (ie can you place queen, is it unique.)

// TODO: you might want to create funcs that return boards for testing. 
// eg: create solved board_8
// this way you don't need to have the same strings in different tests? Ask Neil.

bool board_is_unique(Board a, Board boards[BOARDS_IN_LIST], int size)
{
    for (int i=0; i<size; i++){
        if(are_boards_identical(a, boards[i])){
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
    assert(board_is_unique(c, boards, 1)==false);
    strcpy(c.grid[7], "--------");
    // now we've modified c to be a different board and test again.
    assert(board_is_unique(c, boards, 1));
    // TODO: ideally we'd add more to this function.
}


bool is_solved_board(Board b)
{
    if (b.num_queens==b.size){
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
    assert(is_solved_board(c)==false);
}


// TODO: check if this is the best way to do this?
Board copy_board(Board source)
{
    Board dest;
    dest.size = source.size;
    dest.num_queens = source.num_queens;

    for (int r=0; r<source.size; r++){
        for (int c=0; c<source.size; c++){
            dest.grid[r][c]=source.grid[r][c];
        }
    }
    return dest;
}

void print_board(Board b)
{
    printf("================y\n");
    printf("Size: %i\n", b.size);
    printf("Queens: %i\n", b.num_queens);
    for (int r=0; r<b.size; r++){
        for (int c=0; c<b.size; c++){
            printf("%c",b.grid[r][c]);
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
    int next_index = 0;
    int num_solutions = 0;
    assert(add_child_boards(c, boards, next_index, &num_solutions, false)==0);
    assert(num_solutions==0);

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
    next_index = 0;
    num_solutions=0;
    next_index = add_child_boards(d, boards, next_index, &num_solutions, false);
    assert(next_index==1);
    assert(boards[0].grid[7][2]=='Q');
    assert(num_solutions==1);

    // boards2
    // Empty board (checks that it has created n*n boards)
    // and spot checks some.
    Board e = create_empty_board(3);
    next_index = 0;
    num_solutions=0;
    next_index = add_child_boards(e, boards, next_index, &num_solutions, false);
    assert(next_index==9);
    assert(num_solutions==0);
    
}

/* Returns if it's possible to place a queen at the given
coordinates. */
bool can_place_queen(int r, int c, Board b)
{
  if (!is_valid_cord(r, c, b)){
    return false;
  }
  // queen in square already.
  if (b.grid[r][c] == 'Q'){
    return false;
  }
  if (queen_in_row(r, b)){
    return false;
  }
  if (queen_in_col(c, b)){
    return false;
  }
  if (queen_in_diagonals(r, c, b)){
    return false;
  }
  return true;
}

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

void test_queen_in_row(void)
{
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

bool queen_in_col(int c, Board b)
{
    for (int i=0; i<b.size; i++){
        if(b.grid[i][c]=='Q'){
            return true;
        }
    }
    return false;
}

void test_queen_in_col(void)
{
    Board b;
    b.num_queens =8;
    b.size = 8;
    strcpy(b.grid[0], "-Q------");
    strcpy(b.grid[1], "--------");
    strcpy(b.grid[2], "---Q----");
    strcpy(b.grid[3], "--------");
    strcpy(b.grid[4], "--------");
    strcpy(b.grid[5], "--------");
    strcpy(b.grid[6], "--------");
    strcpy(b.grid[7], "-------Q");
    assert(queen_in_col(0,b)==false);
    assert(queen_in_col(1,b));
    assert(queen_in_col(3,b));
    assert(queen_in_col(4,b)==false);
    assert(queen_in_col(7,b));
}

// TODO: this is too long. Split it up.
bool queen_in_diagonals(int r, int c, Board b)
{
    // up and left
    // TODO: if you wanted you could start at r-1 and c-1
    // since you've already checked that.
    int j = r;
    int i = c;
    while (is_valid_cord(j, i, b)){
        if(b.grid[j][i]=='Q'){
            return true;
        }
        j--;
        i--;
    }

    // up and right
    j = r;
    i = c;
    while (is_valid_cord(j, i, b)){
        if(b.grid[j][i]=='Q'){
            return true;
        }
        j--;
        i++;
    }

    // down and left
    j = r;
    i = c;
    while (is_valid_cord(j, i, b)){
        if(b.grid[j][i]=='Q'){
            return true;
        }
        j++;
        i--;
    }

    // down and right
    j = r;
    i = c;
    while (is_valid_cord(j, i, b)){
        if(b.grid[j][i]=='Q'){
            return true;
        }
        j++;
        i++;
    }
    return false;
}

void test_queen_in_diagonals(void)
{
    Board b;
    b.num_queens = 8;
    b.size = 8;
    strcpy(b.grid[0], "--------");
    strcpy(b.grid[1], "--------");
    strcpy(b.grid[2], "--Q-----");
    strcpy(b.grid[3], "--------");
    strcpy(b.grid[4], "--------");
    strcpy(b.grid[5], "--------");
    strcpy(b.grid[6], "--------");
    strcpy(b.grid[7], "--------");
    assert(queen_in_diagonals(0,0,b)); //down and right
    assert(queen_in_diagonals(4,0,b)); // up and right
    assert(queen_in_diagonals(0,4,b)); // down and left 
    assert(queen_in_diagonals(4,4,b)); // up and left
    assert(queen_in_diagonals(3,2,b)==false);
}

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
// also this prints a new line at the end.
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
    Board b;
    b.num_queens =8;
    b.size = 8;
    strcpy(b.grid[0], "Q-------");
    strcpy(b.grid[1], "--------");
    strcpy(b.grid[2], "--------");
    strcpy(b.grid[3], "--------");
    strcpy(b.grid[4], "--------");
    strcpy(b.grid[5], "--------");
    strcpy(b.grid[6], "--------");
    strcpy(b.grid[7], "--------");
    
    assert(can_place_queen(1,7,b)); // nothing in row, col, or diagonal
    assert(can_place_queen(6,3,b)); // nothing in row, col, or diagonal
    assert(can_place_queen(0,8,b)==false); // off the board
    assert(can_place_queen(2,0,b)==false); // off the board
    assert(can_place_queen(0,0,b)==false); // Q already there.
    assert(can_place_queen(0,6,b)==false); // Q in col (right above)
    assert(can_place_queen(1,0,b)==false); // Q in this col.
    assert(can_place_queen(2,0,b)==false); // Q in this row.

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
    assert(can_place_queen(0,0,c)==false);
    assert(can_place_queen(3,0,c)==false);
    assert(can_place_queen(8,0,c)==false);
    assert(can_place_queen(0,2,c)==false);
    assert(can_place_queen(3,1,c)==false);
    assert(can_place_queen(4,2,c)==false);
    assert(can_place_queen(7,4,c)==false);
    assert(can_place_queen(1,1,c)==false);
    assert(can_place_queen(4,3,c)==false);

    }

void test(void)
{
    test_are_boards_indentical();
    test_create_empty_board();
    test_is_valid_cord();
    test_queen_in_row();
    test_add_child_boards();
    test_is_solved_board();
    test_board_is_unique();
}
