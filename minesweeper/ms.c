#include "ms.h"

bool char_is_correct(char c);
bool str_is_correct_length(int width, int height, int input_len);
board populate_grid(board b, char inp[MAXSQ * MAXSQ + 1]);
bool are_2d_arrays_equal(int grid1[MAXSQ][MAXSQ], int grid2[MAXSQ][MAXSQ], int height, int width);
bool str_contains_correct_characters(char inp[MAXSQ * MAXSQ + 1]);
bool str_mines_leq_totmines(unsigned totmines, char inp[MAXSQ * MAXSQ + 1]);
board rule_1(board b);
int count_item(board b, char item_to_find);
int count_ajc_items(board b, int r, int c, char item_to_find);
bool is_valid_pos(board b, int r, int c);
board number_cells_with_count_of_ajc_mines(board b);
bool rule_2(board *b);
bool cell_is_a_number(board b, int r, int c);
void change_adj_unk_to_mines(board *b, int r, int c);
void print_board(board b);
void test_char_is_correct(void);
void test_solve_board(void);
void test_are_2d_arrays_equal(void);
void test_populate_grid(void);
void test_make_board(void);
void test_cell_is_a_number(void);
void test_board2str(void);
void test_str_contains_correct_characters(void);
void test_str_mines_leq_totmines(void);
void test_str_is_correct_length(void);
void test_syntax_check(void);
void test_count_item(void);
void test_is_valid_pos(void);
void test_count_ajc_items(void);
void test_number_cells_with_count_of_ajc_mines(void);
void test_rule_1(void);
void test_change_adj_unk_to_mines(void);
void test_rule_2(void);

board solve_board(board b)
{
    // continously implements rule 2 until no more changes are made.
    while (rule_2(&b)){
        // Do nothing.
    }

    // then implement rule 1.
    b = rule_1(b);
    return b;
}

void print_board(board b)
{
    printf("------- \n");
    for (int j = 0; j < b.h; j++){
        for (int i = 0; i < b.w; i++){
            printf("%c ",b.grid[j][i]);
        }
        printf("\n");
    }
    printf("------- \n");
}

/*
Returns a boolean if the board was changed.

Rule 2: 
For a known square having the number (true_ajc_mines), with (ajc_unk) unknown 
and (known_ajc_mines) known mines in its
Moore neighbourhood, 
if true_ajc_mines = known_ajc_mines+ajc_unk 
and ajc_unk > 0 then all unknown squares must be mines.
*/ 
bool rule_2(board* b)
{
    for (int j = 0; j < b->h; j++){
        for (int i = 0; i < b->w; i++){
            if (cell_is_a_number(*b, j, i)){
                int ajc_unk = count_ajc_items(*b, j, i, UNK);
                if (ajc_unk){
                    int true_ajc_mines = b->grid[j][i] - '0';
                    int known_ajc_mines = count_ajc_items(*b, j, i, MINE);
                    if (true_ajc_mines == known_ajc_mines+ajc_unk){
                        change_adj_unk_to_mines(b, j, i);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void change_adj_unk_to_mines(board* b, int r, int c)
{
    for (int j = r - 1; j <= r + 1; j++){
        for (int i = c - 1; i <= c + 1; i++){
            // don't include the center cell itself.
            if (!(j == r && i == c)){
                if (is_valid_pos(*b, j, i)){
                    if (b->grid[j][i] == UNK){
                        b->grid[j][i] = MINE;
                    }
                }
            }
        }
    }
}

bool cell_is_a_number(board b, int r, int c)
{
    char cell = b.grid[r][c];
    if (cell == UNK || cell == MINE){
        return false;
    }
    return true;
}

/*
Returns the (possibly changed) board.
Rule 1: If we’ve discovered all the mines on the board already,
then any unknown cell can simply
be numbered with the count of mines in its Moore neighbourhood.
already.
*/
board rule_1(board b)
{
    int found_mines = count_item(b, MINE);
    int mines_remaining = b.totmines - found_mines;
    if (mines_remaining == 0){
        b = number_cells_with_count_of_ajc_mines(b);
    }
    return b;
}

board number_cells_with_count_of_ajc_mines(board b)
{
    for (int j = 0; j < b.h; j++){
        for (int i = 0; i < b.w; i++){
            if (b.grid[j][i] == UNK){
                b.grid[j][i] = count_ajc_items(b, j, i, MINE) + '0';
            }
        }
    }
    return b;
}

bool is_valid_pos(board b, int r, int c)
{
    if (r >= 0 && c >= 0 && r < b.h && c < b.w){
        return true;    
    }
    return false;
}

/* Returns how many of an item are ajacent to specified cell. */
int count_ajc_items(board b, int r, int c, char item_to_find)
{
    int count = 0;

    for (int j = r - 1; j <= r + 1; j++){
        for (int i = c - 1; i <= c + 1; i++){
            // don't include the center cell itself.
            if (!(j == r && i == c)){
                if (is_valid_pos(b, j, i)){
                    if (b.grid[j][i] == item_to_find){
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

/* Returns the number of that item found in the board. */
int count_item(board b, char item_to_find)
{
    int found = 0;
    for (int j = 0; j < b.h; j++){
        for (int i = 0; i < b.w; i++){
            if (b.grid[j][i] == item_to_find){
                found++;
            }
        }
    }
    return found;
}

void board2str(char s[MAXSQ*MAXSQ+1], board b)
{
    int pos = 0;
    for (int j = 0; j < b.h; j++){
        for (int i = 0; i < b.w; i++){
            s[pos] = b.grid[j][i];
            pos++;
        }
    }
    s[pos] = '\0';
}

bool syntax_check(unsigned totmines, unsigned width, unsigned height,char inp[MAXSQ * MAXSQ + 1])
{
    int input_len = strlen(inp);
    if (!str_is_correct_length(width, height, input_len)){
        return false;
    }
    if (!str_contains_correct_characters(inp)){
        return false;
    }
    if (!str_mines_leq_totmines(totmines, inp)){
        return false;
    }
    return true;
}

bool str_mines_leq_totmines(unsigned totmines, char inp[MAXSQ * MAXSQ + 1])
{
    unsigned mine_count = 0;
    for (int i = 0; inp[i]; i++){
        char c = inp[i];
        if (c == MINE){
            mine_count++;
        }
    }
    return mine_count <= totmines;
}

bool str_contains_correct_characters(char inp[MAXSQ * MAXSQ + 1])
{
    for (int i = 0; inp[i]; i++){
        char c = inp[i];
        if (!char_is_correct(c)){
            return false;
        }
    }
    return true;
}

bool char_is_correct(char c)
{
    if (c == UNK || c == MINE || (c >= '0' && c <= '8')){
        return true;
    }
    return false;
}

bool str_is_correct_length(int width, int height, int input_len)
{
    if (height *  width != input_len){
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

    // This line should never be reached. 
    return b;
}

bool are_2d_arrays_equal(int grid1[MAXSQ][MAXSQ], int grid2[MAXSQ][MAXSQ], int height, int width)
{
    for (int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){
            if (grid1[j][i] != grid2[j][i]){
                return false;
            }
        }
    }
    return true;
}

void test(void)
{
    test_char_is_correct();
    test_are_2d_arrays_equal();
    test_populate_grid();
    test_make_board();
    test_cell_is_a_number();
    test_board2str();
    test_str_contains_correct_characters();
    test_str_mines_leq_totmines();
    test_str_is_correct_length();
    test_syntax_check();
    test_count_item();
    test_is_valid_pos();
    test_count_ajc_items();
    test_number_cells_with_count_of_ajc_mines();
    test_rule_1();
    test_change_adj_unk_to_mines();
    test_rule_2();
    test_solve_board();
}

void test_char_is_correct(void)
{
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
    assert(char_is_correct('x') == false);
    assert(char_is_correct('!') == false);
    assert(char_is_correct('[') == false);
    assert(char_is_correct('#') == false);
}

void test_populate_grid(void)
{
    int grid1[MAXSQ][MAXSQ] = {
        {'X', 'X', 'X'},
        {'?', '?', 'X'},
        {'?', 'X', '1'},
    };
    char grid_1_str[] = "XXX??X?X1";
    board b;
    b.h = 3;
    b.w = 3;
    b = populate_grid(b, grid_1_str);
    assert(are_2d_arrays_equal(b.grid, grid1, 3, 3));
}

void test_are_2d_arrays_equal(void)
{
    int grid1[MAXSQ][MAXSQ] = {
        {'X', 'X', 'X'},
        {'?', '?', 'X'},
        {'?', 'X', '1'},
    };
    int grid2[MAXSQ][MAXSQ] = {
        {'X', '?', '?'},
        {'?', '?', 'X'},
        {'?', 'X', '1'},
    };
    assert(are_2d_arrays_equal(grid1, grid1, 3, 3));
    assert(are_2d_arrays_equal(grid1, grid2, 3, 3) == false);
}

void test_count_item(void)
{
    char grid_1_str[] = "XXX??X?X1";
    board e = make_board(5, 3, 3, grid_1_str);
    assert(count_item(e, MINE) == 5);
    assert(count_item(e, UNK) == 3);

    char grid_2_str[] = "X????X?X1";
    board f = make_board(5, 3, 3, grid_2_str);
    assert(count_item(f, MINE) == 3);
    assert(count_item(f, UNK) == 5);

    char grid_3_str[] = "?????????";
    board g = make_board(5, 3, 3, grid_3_str);
    assert(count_item(g, MINE) == 0);
    assert(count_item(g, UNK) == 9);    

    char grid_4_str[] = "XXXX2332";
    board h = make_board(4, 4, 2, grid_4_str);
    assert(count_item(h, MINE) == 4);
    assert(count_item(h, UNK) == 0);    
}

void test_is_valid_pos(void)
{
    char grid_2_str[] = "X????X?X1";
    board g = make_board(5, 3, 3, grid_2_str);
    assert(is_valid_pos(g, 1, 1) == true);
    assert(is_valid_pos(g, -1, 0) == false);
    assert(is_valid_pos(g, 0, -1) == false);
    assert(is_valid_pos(g, 2, 3) == false);
    assert(is_valid_pos(g, 3, 2) == false);
    assert(is_valid_pos(g, 2, 3) == false);

    char grid_3_str[] = "XXXX2???";
    board h = make_board(4, 4, 2, grid_3_str);
    assert(is_valid_pos(h, 1, 1) == true);
    assert(is_valid_pos(h, 0, 0) == true);
    assert(is_valid_pos(h, -1, 0) == false);
    assert(is_valid_pos(h, 0, -1) == false);
    assert(is_valid_pos(h, 2, 0) == false);
    assert(is_valid_pos(h, 0, 4) == false);
    assert(is_valid_pos(h, 2, 4) == false);
}

void test_cell_is_a_number(void){
    char grid_1_str[] = "XXX??X?X1";
    board c = make_board(5, 3, 3, grid_1_str);
    assert(cell_is_a_number(c, 0, 0) == false);
    assert(cell_is_a_number(c, 2, 2));
    assert(cell_is_a_number(c, 1, 0) == false);
}

void test_str_contains_correct_characters(void)
{
    assert(str_contains_correct_characters("????i") == false);
    assert(str_contains_correct_characters("aaaaa") == false);
    assert(str_contains_correct_characters("?????"));
    assert(str_contains_correct_characters("???XX"));
    assert(str_contains_correct_characters("012345678?X"));
}

void test_str_mines_leq_totmines(void)
{
    assert(str_mines_leq_totmines(3, "XXX"));
    assert(str_mines_leq_totmines(2, "XXX") == false);
    assert(str_mines_leq_totmines(0, "???"));
    assert(str_mines_leq_totmines(1, "???"));
    assert(str_mines_leq_totmines(2, "X?X13"));
    assert(str_mines_leq_totmines(1, "X??13"));
    assert(str_mines_leq_totmines(2, "X??13"));
    assert(str_mines_leq_totmines(1, "XXX31") == false);
}

void test_str_is_correct_length(void)
{
    assert(str_is_correct_length(3, 3, 9));
    assert(str_is_correct_length(4, 4, 16));
    assert(str_is_correct_length(4, 4, 17)==false);
}

void test_count_ajc_items(void)
{
    board h = make_board(5, 3, 3, "??X??X??X");
    assert(count_ajc_items(h, 1, 1, MINE) == 3);
    assert(count_ajc_items(h, 0, 0, MINE) == 0);
    assert(count_ajc_items(h, 2, 0, MINE) == 0);
    assert(count_ajc_items(h, 2, 1, MINE) == 2);
    assert(count_ajc_items(h, 0, 2, MINE) == 1);
    assert(count_ajc_items(h, 1, 1, UNK) == 5);
    assert(count_ajc_items(h, 0, 0, UNK) == 3);
    assert(count_ajc_items(h, 2, 0, UNK) == 3);
    assert(count_ajc_items(h, 2, 1, UNK) == 3);
    assert(count_ajc_items(h, 0, 2, UNK) == 2);

    board g = make_board(4, 4, 2, "XXXX2???");
    assert(count_ajc_items(g, 1, 1, MINE) == 3);
    assert(count_ajc_items(g, 1, 2, MINE) == 3);
    assert(count_ajc_items(g, 1, 3, MINE) == 2);
}

void test_number_cells_with_count_of_ajc_mines(void)
{
    board i = make_board(4, 3, 3, "XX??XX???");
    board ir = number_cells_with_count_of_ajc_mines(i);
    char outputi[MAXSQ * MAXSQ + 1];
    board2str(outputi, ir);
    assert(strcmp(outputi, "XX33XX122") == 0);

    board j = make_board(4, 4, 2, "XXXX2???");
    board jr = number_cells_with_count_of_ajc_mines(j);
    char outputj[MAXSQ * MAXSQ + 1];
    board2str(outputj, jr);
    assert(strcmp(outputj, "XXXX2332") == 0);
}


void test_change_adj_unk_to_mines(void)
{
    board m = make_board(2, 3, 3, "XX3??????");
    change_adj_unk_to_mines(&m, 0, 2);
    char outputm[MAXSQ * MAXSQ + 1];
    board2str(outputm, m);
    assert(strcmp(outputm, "XX3?XX???") == 0);

    board m1 = make_board(5, 5, 5, "0111013X311XXX113?3101110");
    change_adj_unk_to_mines(&m1, 3, 1);
    char outputm1[MAXSQ * MAXSQ + 1];
    board2str(outputm1, m1);
    assert(strcmp(outputm1, "0111013X311XXX113X3101110") == 0);
}

void test_board2str(void)
{
    // 3 x 3 
    char grid_d_str[] = "XXX??X?X1";
    board d = make_board(5, 3, 3, grid_d_str);
    char output_str_d[MAXSQ * MAXSQ + 1];
    board2str(output_str_d, d);
    assert(strcmp(output_str_d, grid_d_str) == 0);

    // 1 x 1
    char grid_e_str[] = "?";
    board e = make_board(1, 1, 1, grid_e_str);
    char output_str_e[MAXSQ * MAXSQ + 1];
    board2str(output_str_e, e);
    assert(strcmp(output_str_e, grid_e_str) == 0);

    // 2 x 9
    char grid_g_str[] = "?12345678X12345678";
    board g = make_board(2, 2, 9, grid_g_str);
    char output_str_g[MAXSQ * MAXSQ + 1];
    board2str(output_str_g, g);
    assert(strcmp(output_str_g, grid_g_str) == 0);

    // 10 x 10
    char grid_h_str[] = 
    "?12345678X"
    "876543210?"
    "?12345678X"
    "876543210?"
    "?12345678X"
    "876543210?"
    "?12345678X"
    "876543210?"
    "?12345678X"
    "876543210?";
    board h = make_board(2, 10, 10, grid_h_str);
    char output_str_h[MAXSQ * MAXSQ + 1];
    board2str(output_str_h, h);
    assert(strcmp(output_str_h, grid_h_str) == 0);
}

void test_make_board(void)
{
    //  3 x 3
    board d = make_board(5, 3, 3, "123??X?X8");
    assert(d.h == 3);
    assert(d.w == 3);
    assert(d.totmines == 5);
    assert(d.grid[0][0]=='1');
    assert(d.grid[1][0]=='?');
    assert(d.grid[1][2]=='X');
    assert(d.grid[2][2]=='8');
    
    // 1 x 1
    board e = make_board(1, 1, 1, "?");
    assert(e.h == 1);
    assert(e.w == 1);
    assert(e.totmines == 1);
    assert(e.grid[0][0]=='?');

    // 2 x 9
    board g = make_board(2, 2, 9, "?12345678X12345678");
    assert(g.h == 9);
    assert(g.w == 2);
    assert(g.totmines == 2);
    assert(g.grid[0][0]=='?');
    assert(g.grid[0][1]=='1');
    assert(g.grid[1][0]=='2');
    assert(g.grid[2][1]=='5');
    assert(g.grid[8][0]=='7');
    assert(g.grid[8][1]=='8');

    // 10 x 10
    board h = make_board(2, 10, 10, 
    "?12345678X"
    "876543210?"
    "?12345678X"
    "876543210?"
    "?12345678X"
    "876543210?"
    "?12345678X"
    "876543210?"
    "?12345678X"
    "876543210?"
    );
    assert(h.h == 10);
    assert(h.w == 10);
    assert(h.totmines == 2);
    assert(h.grid[0][0]=='?');
    assert(h.grid[0][1]=='1');
    assert(h.grid[1][0]=='8');
    assert(h.grid[2][1]=='1');
    assert(h.grid[8][0]=='?');
    assert(h.grid[8][1]=='1');
    assert(h.grid[9][9]=='?');
    assert(h.grid[3][9]=='?');
    assert(h.grid[4][5]=='5');
}

void test_syntax_check(void)
{
    assert(syntax_check(3, 3, 3, "XXX??????"));
    assert(syntax_check(3, 3, 3, "XX???????"));
    assert(syntax_check(3, 3, 3, "XX1?2??46"));
    assert(syntax_check(3, 3, 3, "XX1?2??46"));
    assert(syntax_check(3, 4, 4, "XX1?2??46213????"));
    assert(syntax_check(3, 4, 4, "XXXX2??46213????") == false);
    // too many mines
    assert(syntax_check(3, 3, 3, "XXXX?????") == false);
    // too long string
    assert(syntax_check(3, 3, 3, "XXXXXXXXX") == false);
    // has invalid characters
    assert(syntax_check(3, 3, 3, "XXXa?????") == false);

    // testing irregular but valid boards
    assert(syntax_check(1, 1, 1, "X") == true);
    assert(syntax_check(3, 10, 1, "XX??000000") == true);
    assert(syntax_check(7, 6, 3, 
    "XX??00"
    "XX??00"
    "XX??00") == true);

}

void test_rule_1(void)
{
    // 5 x 5
    board k = make_board(5, 5, 5, "011?013X311XXX113X3101110");
    board kr = rule_1(k);
    char outputk[MAXSQ * MAXSQ + 1];
    board2str(outputk, kr);
    assert(strcmp(outputk, "0111013X311XXX113X3101110") == 0);

    // 3 x 3
    board l = make_board(2, 3, 3, "XX???????");
    board lr = rule_1(l);
    char outputl[MAXSQ * MAXSQ + 1];
    board2str(outputl, lr);
    assert(strcmp(outputl, "XX1221000") == 0);

    // 3 x 3 
    // rule one doesn't apply because not all mines have been found
    // so (board doesn't change)
    board m = make_board(2, 3, 3, "111?1????");
    board mr = rule_1(m);
    char outputm[MAXSQ * MAXSQ + 1];
    board2str(outputm, mr);
    assert(strcmp(outputm, "111?1????") == 0);

    // 4 x 2 
    board n = make_board(4, 4, 2, "XXXX2???");
    board nr = rule_1(n);
    char outputn[MAXSQ * MAXSQ + 1];
    board2str(outputn, nr);
    assert(strcmp(outputn, "XXXX2332") == 0);

    // 9 x 7 
    board o = make_board(32, 9, 7, 
    "XXXX?XXXX"
    "4664?4664"
    "XXXX?XXXX"
    "?????????"
    "XXXX?XXXX"
    "4664?4664"
    "XXXX?XXXX");
    board or = rule_1(o);
    char outputo[MAXSQ * MAXSQ + 1];
    board2str(outputo, or);
    assert(strcmp(outputo, 
    "XXXX2XXXX"
    "466444664"
    "XXXX2XXXX"
    "466444664"
    "XXXX2XXXX"
    "466444664"
    "XXXX2XXXX") == 0);
}

void test_rule_2(void)
{
    // 5 x 5
    board n = make_board(5, 5, 5, "0111013X311XXX113?3101110");
    assert(rule_2(&n) == true);
    char outputn[MAXSQ * MAXSQ + 1];
    board2str(outputn, n);
    assert(strcmp(outputn, "0111013X311XXX113X3101110") == 0);
    assert(rule_2(&n) == false);

    // 4 x 2 
    board m = make_board(4, 4, 2, "XXXX3???");
    assert(rule_2(&m)==true);
    char outputm[MAXSQ * MAXSQ + 1];
    board2str(outputm, m);
    assert(strcmp(outputm, "XXXX3X??") == 0);

    // Test where rule 2 doesn't apply so it doesn't change.
    board p = make_board(4, 2, 4, "XX5XXXX?");
    assert(rule_2(&p)==false);
    char outputp[MAXSQ * MAXSQ + 1];
    board2str(outputp, p);
    assert(strcmp(outputp, "XX5XXXX?") == 0);

    // 10 x 10 
    board q = make_board(12, 10, 10, 
    "1110011100"
    "1?2101X210"
    "02X2002X20"
    "012X1012X1"
    "0011100111"
    "1110011100"
    "1X2101X210"
    "02X2002X20"
    "012X1012X1"
    "0011100111"
    );
    assert(rule_2(&q));
    char outputq[MAXSQ * MAXSQ + 1];
    board2str(outputq, q);
    assert(strcmp(outputq, 
    "1110011100"
    "1X2101X210"
    "02X2002X20"
    "012X1012X1"
    "0011100111"
    "1110011100"
    "1X2101X210"
    "02X2002X20"
    "012X1012X1"
    "0011100111") == 0);

}

void test_solve_board(void)
{
    // 3 x 5 grid.
    board n = make_board(1, 3, 5, "???1111?1111???");
    board sn = solve_board(n);
    char outputn[MAXSQ * MAXSQ + 1];
    board2str(outputn, sn);
    assert(strcmp(outputn,"0001111X1111000")==0);

    // board still has unknown values.
    board i = make_board(3, 5, 6, "00000111111?11?11111??????????");
    board si = solve_board(i);
    char outputi[MAXSQ * MAXSQ + 1];
    board2str(outputi, si);
    assert(strcmp(outputi,"00000111111X11X11111??????????")==0);

    // 10 x 10 grid
    board j = make_board(6, 10, 10, 
    "0000000000"
    "1111111110"
    "1?11?11?10"
    "1111111110"
    "0000000000"
    "0000000000"
    "1111111110"
    "1?11?11?10"
    "1111111110"
    "0000000000");
    board sj = solve_board(j);
    char outputj[MAXSQ * MAXSQ + 1];
    board2str(outputj, sj);
    assert(strcmp(outputj,
    "0000000000"
    "1111111110"
    "1X11X11X10"
    "1111111110"
    "0000000000"
    "0000000000"
    "1111111110"
    "1X11X11X10"
    "1111111110"
    "0000000000")==0);

    // already solved board.
    board k = make_board(5, 4, 5, "X111222X1X32123X11X2");
    board sk = solve_board(k);
    char outputk[MAXSQ * MAXSQ + 1];
    board2str(outputk, sk);
    assert(strcmp(outputk,"X111222X1X32123X11X2")==0);

    // completely unsolvable board
    board q = make_board(5, 4, 5, "????????????????????");
    board sq = solve_board(q);
    char outputq[MAXSQ * MAXSQ + 1];
    board2str(outputq, sq);
    assert(strcmp(outputq,"????????????????????")==0);

    // just rule 1 
    board b = make_board(8, 4, 4, "X????XXX??X?XXX2");
    board sb = solve_board(b);
    char outputb[MAXSQ * MAXSQ + 1];
    board2str(outputb, sb);
    assert(strcmp(outputb,"X3322XXX36X4XXX2")==0);

    // just rule 2
    board h = make_board(3, 4, 4, "1111?1?1????????");
    board sh = solve_board(h);
    char outputh[MAXSQ * MAXSQ + 1];
    board2str(outputh, sh);
    assert(strcmp(outputh,"1111X1X1????????")==0);

    // Combo of rule 1 and rule 2
    board a = make_board(5, 5, 5, "XXX??2332100000110???10??");
    board sa = solve_board(a);
    char outputa[MAXSQ * MAXSQ + 1];
    board2str(outputa, sa);
    assert(strcmp(outputa,"XXXX1233210000011000X1000")==0);

    board c = make_board(20, 10, 10, 
    "XXX?1XX?X1"
    "2332123321"
    "0000000000"
    "1100011000"
    "X3322X3321"
    "X?XX3XX?X1"
    "2332223321"
    "0000000000"
    "1?00111000"
    "X1001?1000"
    );
    board sc = solve_board(c);
    char outputc[MAXSQ * MAXSQ + 1];
    board2str(outputc, sc);
    assert(strcmp(outputc,
    "XXXX1XXXX1"
    "2332123321"
    "0000000000"
    "1100011000"
    "X3322X3321"
    "XXXX3XXXX1"
    "2332223321"
    "0000000000"
    "1100111000"
    "X1001X1000"
    )==0);
}
