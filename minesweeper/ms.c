#include "ms.h"

bool char_is_correct(char c);
bool str_is_correct_length(unsigned width, 
                            unsigned height, 
                            int input_len);

board populate_grid(board b, char inp[MAXSQ*MAXSQ+1]);
bool are_2d_arrays_equal(
                                int grid1[MAXSQ][MAXSQ], 
                                int grid2[MAXSQ][MAXSQ],
                                int height,
                                int width
                                );
bool str_contains_correct_characters(int len, char inp[MAXSQ*MAXSQ+1]);
bool str_mines_leq_totmines(unsigned totmines, char inp[MAXSQ*MAXSQ+1], int len);
void rule_1(board b);
void mark_all_unknowns_as_mines(board b);
int get_number_item(board b, char item_to_find);



board solve_board(board b)
{
    // rule_1(board b);
    return b;
}

/*
if there's N unknown spots left on the board, and you know there's 
N mines remaining (totmines - found_mines) 
you can fill every question mark with mines
*/
void rule_1(board b)
{
    int found_mines = get_number_item(b, MINE);
    int mines_remaining = b.totmines - found_mines;
    int unknowns =  get_number_item(b, UNK);
    if (unknowns == mines_remaining){
        mark_all_unknowns_as_mines(b);
    }
}

void mark_all_unknowns_as_mines(board b)
{
    // TODO: this won't work - you must return the board.
    for (int j=0; j<b.h; j++){
        for (int i=0; i<b.w; i++){
            if (b.grid[j][i]==UNK){
                b.grid[j][i] = MINE;
            }
        }
    }

}

int get_number_item(board b, char item_to_find)
{
    int found = 0;
    for (int j=0; j<b.h; j++){
        for (int i=0; i<b.w; i++){
            if (b.grid[j][i]==item_to_find){
                found++;
            }
        }
    }
    return found;
}



void board2str(char s[MAXSQ*MAXSQ+1], board b)
{
    int pos = 0;
    for (int j=0; j<b.h; j++){
        for(int i=0; i<b.w; i++){
           s[pos] = b.grid[j][i];
        }
    }
    s[pos] = '\0';
}

bool syntax_check(
                    unsigned totmines, 
                    unsigned width, 
                    unsigned height, 
                    char inp[MAXSQ*MAXSQ+1]
                )
{
    int input_len = strlen(inp);
    if (!str_is_correct_length(width, height, input_len)){
        return false;
    }
    if (!str_contains_correct_characters(input_len, inp)){
        return false;
    }
    if (!str_mines_leq_totmines(totmines, inp, input_len)){
        return false;
    }
    return true;
}

bool str_mines_leq_totmines(unsigned totmines, char inp[MAXSQ*MAXSQ+1], int len)
{
    unsigned mine_count = 0;
    for (int i=0; i<len; i++){
        char c = inp[i];
        if (c==MINE){
            mine_count++;
        }
    }
    return mine_count<=totmines;
}

bool str_contains_correct_characters(int len, char inp[MAXSQ*MAXSQ+1])
{
    for (int i=0; i<len; i++){
        char c = inp[i];
        if(!char_is_correct(c)){
            return false;
        }
    }
    return true;
}

bool char_is_correct(char c)
{
    if(c==UNK || c==MINE || (c>='0' && c<='9')){
        return true;
    }
    return false;

}

bool str_is_correct_length(unsigned width, 
                            unsigned height, 
                            int input_len)
{
    // compiler warns about comparing unsigned result
    // with signed, so converting to int.
    if ( (int) height * (int) width != input_len){
        return false;
    }
    return true;
}

board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1])
{
    board b;
    b.w = width;
    b.h = height;
    b.totmines = totmines;

    return populate_grid(b, inp);
}

board populate_grid(board b, char inp[MAXSQ*MAXSQ+1])
{
    int str_index = 0;
    int str_len = strlen(inp);

    for (int j=0; j<b.h; j++){
        for (int i=0; i<b.w; i++){
            b.grid[j][i] = inp[str_index];
            str_index++;
            if (str_index==str_len){
                return b;
            }
        }
    }
    // this line should never be reached
    // but if not included compiler warns
    return b;
}

bool are_2d_arrays_equal(
                        int grid1[MAXSQ][MAXSQ], 
                        int grid2[MAXSQ][MAXSQ],
                        int height,
                        int width
                        )
{
    for (int j=0; j<height; j++){
        for (int i=0; i<width; i++){
            if(grid1[j][i]!= grid2[j][i]){
                return false;
            }
        }
    }
    return true;
}

void test(void)
{
    int grid1[MAXSQ][MAXSQ] = {
                                      {'X','X','X'},
                                      {'?','?','X'},
                                      {'?','X','1'},
                                      };
    char grid_1_str[] = "XXX??X?X1";
    int grid2[MAXSQ][MAXSQ] = {
                                      {'X','?','?'},
                                      {'?','?','X'},
                                      {'?','X','1'},
                                      };

    // TESTS FOR are_2d_arrays_equal
    assert(are_2d_arrays_equal(grid1,grid1,3,3));
    assert(are_2d_arrays_equal(grid1,grid2,3,3)==false);

    // TESTS FOR populate_grid
    board b;
    b.h = 3;
    b.w = 3;
    b = populate_grid(b, grid_1_str);
    assert(are_2d_arrays_equal(b.grid,grid1,3,3));
    // TODO: test other sized grids for population.

    // TEST make_board
    board c = make_board(5,3,3,grid_1_str);
    assert(c.h==3);
    assert(c.w==3);
    assert(c.totmines==5);


    // TESTS FOR board2str
    board d = make_board(5,3,3,grid_1_str);
    char output_str_1[MAXSQ*MAXSQ+1];
    board2str(output_str_1,d);
    assert(strcmp(output_str_1,grid_1_str));
    // TODO add more tests

    // TESTS FOR bool char_is_correct(char c)
    assert(char_is_correct(UNK));
    assert(char_is_correct(MINE));
    assert(char_is_correct('0'));
    assert(char_is_correct('1'));
    assert(char_is_correct('2'));
    assert(char_is_correct('3'));
    assert(char_is_correct('4'));
    assert(char_is_correct('5'));
    assert(char_is_correct('6'));
    assert(char_is_correct('7'));
    assert(char_is_correct('8'));
    assert(char_is_correct('9'));
    assert(char_is_correct('x')==false);
    assert(char_is_correct('!')==false);
    assert(char_is_correct('[')==false);
    assert(char_is_correct('#')==false);

    // TESTS FOR str_contains_correct_characters
    assert(str_contains_correct_characters(5,"????i")==false);
    assert(str_contains_correct_characters(5,"aaaaa")==false);
    assert(str_contains_correct_characters(5,"?????"));
    assert(str_contains_correct_characters(5,"???XX"));
    assert(str_contains_correct_characters(12,"0123456789?X"));
    
    // TESTS FOR str_mines_leq_totmines
    assert(str_mines_leq_totmines(3, "XXX", 3));
    assert(str_mines_leq_totmines(2, "XXX", 3)==false);
    assert(str_mines_leq_totmines(0, "???", 3));
    assert(str_mines_leq_totmines(1, "???", 3));
    assert(str_mines_leq_totmines(2, "X?X13", 5));
    assert(str_mines_leq_totmines(1, "X??13", 5));
    assert(str_mines_leq_totmines(2, "X??13", 5));
    assert(str_mines_leq_totmines(1, "XXX31", 5)==false);


    // TESTS FOR str_is_correct_length
    assert(str_is_correct_length(3, 3, 9));
    assert(str_is_correct_length(4, 4, 16));

    // TESTS FOR syntax_check
    assert(syntax_check(3,3,3,"XXX??????"));
    assert(syntax_check(3,3,3,"XX???????"));
    assert(syntax_check(3,3,3,"XX1?2??46"));
    assert(syntax_check(3,3,3,"XX1?2??46"));
    assert(syntax_check(3,4,4,"XX1?2??46213????"));
    assert(syntax_check(3,4,4,"XXXX2??46213????")==false);

    // too many mines
    assert(syntax_check(3,3,3,"XXXX?????")==false);
    // too long string
    assert(syntax_check(3,3,3,"XXXXXXXXX")==false);
    // has invalid characters
    assert(syntax_check(3,3,3,"XXXa?????")==false);


// UNTESTED:
// void rule_1(board b);
// void mark_all_unknowns_as_mines(board b);
// int get_number_of_found_mines(board b);
// int get_number_of_unknowns(board b);


}
