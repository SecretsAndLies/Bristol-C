#include "ms.h"

bool char_is_correct(char c);
bool str_is_correct_length(
    unsigned width,
    unsigned height,
    int input_len);
board populate_grid(board b, char inp[MAXSQ * MAXSQ + 1]);
bool are_2d_arrays_equal(
    int grid1[MAXSQ][MAXSQ],
    int grid2[MAXSQ][MAXSQ],
    int height,
    int width);
bool str_contains_correct_characters(
    int len,
    char inp[MAXSQ * MAXSQ + 1]);
bool str_mines_leq_totmines(
    unsigned totmines,
    char inp[MAXSQ * MAXSQ + 1],
    int len);
board rule_1(board b);
int count_item(board b, char item_to_find);
int count_ajc_items(board b, int r, int c, char item_to_find);
bool is_valid_pos(board b, int r, int c);
board number_all_with_count_of_ajacent_mines(board b);
bool rule_2(board *b);
bool cell_is_a_number(board b, int r, int c);
void change_adj_unk_to_mines(board *b, int r, int c);
void print_board(board b);
void test_char_is_correct(void);

board solve_board(board b)
{
    // continously implements rule 2 until no more changes are made.
    // TODO: check with Neil if I should use braces even if the output is empty?
    while (rule_2(&b));

    // then implement rule 1.
    b = rule_1(b);
    return b;
}

// TODO: check - should you have useless funcs in final solution?
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
TODO: rewrite and delete this text.
takes in a pointer to a board (so you can change it.)
returns a boolean if the board was changed.
Rule 2: 
if a cell contains a number and has unknown values around it

get number of mines around cell -  true_ajc_mines  3
and the number of unknwons around the cell - ajc_unk   1
if (true_ajc_mines-ajc_unk) == ajc_mines
then those unknown squares must be mines.
*/ 
bool rule_2(board* b)
{
    for (int j = 0; j < b->h; j++){
        for (int i = 0; i < b->w; i++){
            if (cell_is_a_number(*b, j, i)){
                int ajc_unk = count_ajc_items(*b, j, i, UNK);
                if (ajc_unk){
                    int true_ajc_mines = b->grid[j][i] - '0';
                    int ajc_mines = count_ajc_items(*b, j, i, MINE);
                    int mines_left = true_ajc_mines - ajc_unk;
                    if (mines_left == ajc_mines){
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
// TODO: confirm delete or keep?
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
        b = number_all_with_count_of_ajacent_mines(b);
    }
    return b;
}

board number_all_with_count_of_ajacent_mines(board b)
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
    if (r >= 0 && c >= 0 && r < b.w && c < b.w){
        return true;    
    }
    return false;
}

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

bool syntax_check(
    unsigned totmines,
    unsigned width,
    unsigned height,
    char inp[MAXSQ * MAXSQ + 1])
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

bool str_mines_leq_totmines(
    unsigned totmines,
    char inp[MAXSQ * MAXSQ + 1],
    int len)
{
    unsigned mine_count = 0;
    for (int i = 0; i < len; i++){
        char c = inp[i];
        if (c == MINE){
            mine_count++;
        }
    }
    return mine_count <= totmines;
}

bool str_contains_correct_characters(
    int len,
    char inp[MAXSQ * MAXSQ + 1])
{
    for (int i = 0; i < len; i++){
        char c = inp[i];
        if (!char_is_correct(c)){
            return false;
        }
    }
    return true;
}

bool char_is_correct(char c)
{
    if (c == UNK || c == MINE || (c >= '0' && c <= '9')){
        return true;
    }
    return false;
}

bool str_is_correct_length(
    unsigned width,
    unsigned height,
    int input_len)
{
    // TODO: ask - is this bad style?
    // compiler warns about comparing unsigned result
    // with signed, so converting to int.
    if ((int) height * (int) width != input_len){
        return false;
    }
    return true;
}

board make_board(
    int totmines,
    int width, 
    int height, 
    char inp[MAXSQ*MAXSQ+1])
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
    // TODO; check
    // this line should never be reached
    // but if not included compiler warns
    return b;
}

bool are_2d_arrays_equal(
    int grid1[MAXSQ][MAXSQ],
    int grid2[MAXSQ][MAXSQ],
    int height,
    int width)
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
    assert(char_is_correct('9'));
    assert(char_is_correct('x') == false);
    assert(char_is_correct('!') == false);
    assert(char_is_correct('[') == false);
    assert(char_is_correct('#') == false);
}

void test(void)
{
    test_char_is_correct();

    // TODO: consider breaking tests into test functions: eg test_2_d_arrays etc.
    // you can pass grid1, grid2 in as needed.
    int grid1[MAXSQ][MAXSQ] = {
        {'X', 'X', 'X'},
        {'?', '?', 'X'},
        {'?', 'X', '1'},
    };
    char grid_1_str[] = "XXX??X?X1";
    int grid2[MAXSQ][MAXSQ] = {
        {'X', '?', '?'},
        {'?', '?', 'X'},
        {'?', 'X', '1'},
    };
    char grid_2_str[] = "X????X?X1";

    // TESTS FOR are_2d_arrays_equal
    assert(are_2d_arrays_equal(grid1, grid1, 3, 3));
    assert(are_2d_arrays_equal(grid1, grid2, 3, 3) == false);

    // TESTS FOR populate_grid
    board b;
    b.h = 3;
    b.w = 3;
    b = populate_grid(b, grid_1_str);
    assert(are_2d_arrays_equal(b.grid, grid1, 3, 3));

    // TEST make_board
    board c = make_board(5, 3, 3, grid_1_str);
    assert(c.h == 3);
    assert(c.w == 3);
    assert(c.totmines == 5);

    // TESTS FOR cell_is_a_number
    assert(cell_is_a_number(c, 0, 0) == false);
    assert(cell_is_a_number(c, 2, 2));
    assert(cell_is_a_number(c, 1, 0) == false);

    // TESTS FOR board2str
    board d = make_board(5, 3, 3, grid_1_str);
    char output_str_1[MAXSQ * MAXSQ + 1];
    board2str(output_str_1, d);
    assert(strcmp(output_str_1, grid_1_str) == 0);
    // TODO add more tests


    // TESTS FOR str_contains_correct_characters
    assert(str_contains_correct_characters(5, "????i") == false);
    assert(str_contains_correct_characters(5, "aaaaa") == false);
    assert(str_contains_correct_characters(5, "?????"));
    assert(str_contains_correct_characters(5, "???XX"));
    assert(str_contains_correct_characters(12, "0123456789?X"));

    // TESTS FOR str_mines_leq_totmines
    assert(str_mines_leq_totmines(3, "XXX", 3));
    assert(str_mines_leq_totmines(2, "XXX", 3) == false);
    assert(str_mines_leq_totmines(0, "???", 3));
    assert(str_mines_leq_totmines(1, "???", 3));
    assert(str_mines_leq_totmines(2, "X?X13", 5));
    assert(str_mines_leq_totmines(1, "X??13", 5));
    assert(str_mines_leq_totmines(2, "X??13", 5));
    assert(str_mines_leq_totmines(1, "XXX31", 5) == false);

    // TESTS FOR str_is_correct_length
    assert(str_is_correct_length(3, 3, 9));
    assert(str_is_correct_length(4, 4, 16));

    // TESTS FOR syntax_check
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

    // TESTS FOR count_item
    board e = make_board(5, 3, 3, grid_1_str);
    assert(count_item(e, MINE) == 5);
    assert(count_item(e, UNK) == 3);

    board f = make_board(5, 3, 3, grid_2_str);
    assert(count_item(f, MINE) == 3);
    assert(count_item(f, UNK) == 5);

    // TESTS FOR is_valid_pos
    board g = make_board(5, 3, 3, grid_2_str);
    assert(is_valid_pos(g, 1, 1) == true);
    assert(is_valid_pos(g, -1, 0) == false);
    assert(is_valid_pos(g, 0, -1) == false);
    assert(is_valid_pos(g, 2, 3) == false);
    assert(is_valid_pos(g, 3, 2) == false);
    assert(is_valid_pos(g, 2, 3) == false);

    // TESTS FOR count_ajc_items
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

    // TESTS FOR number_all_with_count_of_ajacent_mines
    board i = make_board(4, 3, 3, "XX??XX???");
    board ir = number_all_with_count_of_ajacent_mines(i);
    char outputi[MAXSQ * MAXSQ + 1];
    board2str(outputi, ir);
    assert(strcmp(outputi, "XX33XX122") == 0);

    // TESTS FOR board rule_1;
    board k = make_board(5, 5, 5, "011?013X311XXX113X3101110");
    board kr = rule_1(k);
    char outputk[MAXSQ * MAXSQ + 1];
    board2str(outputk, kr);
    assert(strcmp(outputk, "0111013X311XXX113X3101110") == 0);

    board l = make_board(2, 3, 3, "XX???????");
    board lr = rule_1(l);
    char outputl[MAXSQ * MAXSQ + 1];
    board2str(outputl, lr);
    assert(strcmp(outputl, "XX1221000") == 0);

    // TESTS FOR change_adj_unk_to_mines
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

    // TESTS FOR rule_2
    board n = make_board(5, 5, 5, "0111013X311XXX113?3101110");
    assert(rule_2(&n) == true);
    char outputn[MAXSQ * MAXSQ + 1];
    board2str(outputn, n);
    assert(strcmp(outputn, "0111013X311XXX113X3101110") == 0);
    assert(rule_2(&n) == false);

    // TESTS FOR solve_board
    // TODO: write own tests.

}
