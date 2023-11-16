#include "extension.h"

int main(int argc, char* argv[]) 
{
    ext_test();
    validate_ext_arg_count(argc);
    bool is_visualize = false;
    int size;
    parse_ext_args(argc, argv, &size, &is_visualize);
    Board solutions[MEDIUM_LIST];
    Board start = create_empty_board(size);
    int num_solutions = 0;
    ext_solve(0, &start, solutions, &num_solutions);
    for(int i=0; i<num_solutions; i++){
        print_board_string(&solutions[i]);
    }
    printf("%i solutions\n", num_solutions);
    if(is_visualize){
        draw_boards(solutions, num_solutions);
    }
    return 0;
}

void validate_ext_arg_count(int argc) 
{
    if (argc < 2 || argc > 3) {
        on_error("Error: wrong number of args. \n" 
                CORRECT_EXT_USAGE);
    }
}

void ext_test(void)
{
    Board start = create_empty_board(10);
    Board solutions[MEDIUM_LIST];
    int num_solutions = 0;
    ext_solve(0, &start, solutions, &num_solutions);
    assert(num_solutions==724);

    start = create_empty_board(1);
    num_solutions = 0;
    ext_solve(0, &start, solutions, &num_solutions);
    assert(num_solutions==1);
    assert(solutions[0].grid[0][0] == QUEEN);

    start = create_empty_board(2);
    num_solutions = 0;
    ext_solve(0, &start, solutions, &num_solutions);
    assert(num_solutions==0);

    start = create_empty_board(3);
    num_solutions = 0;
    ext_solve(0, &start, solutions, &num_solutions);
    assert(num_solutions==0);

    start = create_empty_board(4);
    num_solutions = 0;
    ext_solve(0, &start, solutions, &num_solutions);
    assert(num_solutions==2);
    assert(solutions[0].grid[0][0]==BLANK);
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

    start = create_empty_board(5);
    num_solutions = 0;
    ext_solve(0, &start, solutions, &num_solutions);
    assert(num_solutions==10);

    start = create_empty_board(6);
    num_solutions = 0;
    ext_solve(0, &start, solutions, &num_solutions);
    assert(num_solutions==4);

    start = create_empty_board(7);
    num_solutions = 0;
    ext_solve(0, &start, solutions, &num_solutions);
    assert(num_solutions==40);
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

void ext_solve(int row, Board *current_board, 
                Board solutions[MEDIUM_LIST], 
                int *num_solutions)
{
    // base case, found a solution, add to the list.
    if (row == current_board->size) {
        solutions[*num_solutions] = *current_board;
        (*num_solutions)++;
        return;
    }

    // otherwise, place queens in each col.
    for (int col = 0; col < current_board->size; col++) {
        if (can_place_queen(row, col, current_board)) {
            // place queens
            current_board->grid[row][col] = QUEEN;
            current_board->num_queens++;

            // Recurse to the next row (which will place queens etc)
            ext_solve(row + 1, current_board, solutions, num_solutions);

            // Backtrack: remove the queen so it can be used
            // in the next loop.
            current_board->grid[row][col] = BLANK;
            current_board->num_queens--;
        }
    }
    return;
}

void parse_ext_args(int argc, char* argv[], int* size, 
                        bool * is_visualize) 
{
    if (argc == 2) {
        *size = convert_and_verify_size(argv[1]);
        if (*size == 0) {
            on_error(
                    "Error: invalid size -"
                    " must be between 1 and 10. \n" 
                    CORRECT_EXT_USAGE);
        }
    }
    if (argc == 3) {
        if (strcmp("-visualize", argv[1]) != 0) {
            on_error("Error, only valid flag is -visualize. \n" 
            CORRECT_EXT_USAGE);
        }
        *size = convert_and_verify_size(argv[2]);
        if (*size == 0) {
            on_error(
                    "Error: invalid size -"
                    " must be between 1 and 10. \n" 
                    CORRECT_EXT_USAGE);
        }
        *is_visualize = true;
    }
}


void draw_boards(Board solutions[MEDIUM_LIST], 
                int num_solutions)
{
    SDL_Simplewin sw;        
    Neill_SDL_Init(&sw);
    int i = 0;
    do {
        sdl_draw_board(solutions[i], &sw);
        SDL_Delay(MILLISECONDDELAY);
        i++;
    } while (!sw.finished && i<num_solutions);
    SDL_Quit();
    atexit(SDL_Quit);
}

void sdl_draw_board(Board b, SDL_Simplewin * sw)
{
    SDL_Rect rectangle;
    rectangle.w = RECTSIZE;
    rectangle.h = RECTSIZE;
    draw_chess_board(b.size, rectangle, sw);
    for (int r = 0; r < b.size; r++) {
        for (int c = 0; c < b.size; c++) {
            if (b.grid[r][c] == QUEEN) {
                draw_queen(sw, r, c);
            }
        }
    }
    Neill_SDL_UpdateScreen(sw);
    Neill_SDL_Events(sw);
}

void draw_queen(SDL_Simplewin * sw, int y, int x)
{
    // convert to pixels.
    x = x * QUEEN_SIZE;
    y = y * QUEEN_SIZE;
    // base
    draw_yellow_rectangle(QUEEN_BASE_W, QUEEN_BASE_H, 
                          QUEEN_BASE_X_OFFSET + x,
                          QUEEN_BASE_Y_OFFSET + y, sw);
    draw_yellow_rectangle(QUEEN_SPINE_W, QUEEN_SPINE_H,
                          QUEEN_SPINE_1_X_OFFSET + x,
                          QUEEN_SPINE_1_Y_OFFSET + y, sw);
    draw_yellow_rectangle(QUEEN_SPINE_2_W, QUEEN_SPINE_2_H,
                          QUEEN_SPINE_2_X_OFFSET + x,
                          QUEEN_SPINE_2_Y_OFFSET + y, sw);
    draw_yellow_rectangle(QUEEN_SPINE_W, QUEEN_SPINE_H,
                          QUEEN_SPINE_3_X_OFFSET + x,
                          QUEEN_SPINE_3_Y_OFFSET + y, sw);
}

void draw_yellow_rectangle(int w, int h, 
                        int x, int y, SDL_Simplewin * sw)
{
    Neill_SDL_SetDrawColour(sw, YELLOW);

    SDL_Rect rect;
    rect.w = w * QUEEN_SIZE;
    rect.h = h * QUEEN_SIZE;
    rect.x = x * QUEEN_SIZE; 
    rect.y = y * QUEEN_SIZE;
    SDL_RenderFillRect(sw->renderer, &rect);
}

void draw_chess_board(int size, 
                    SDL_Rect rectangle, SDL_Simplewin * sw) 
{
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            rectangle.x = x * RECTSIZE;
            rectangle.y = y * RECTSIZE;
            if( ( x + y) % 2 == 0){
                Neill_SDL_SetDrawColour(sw, WHITE);
                SDL_RenderFillRect(sw->renderer, &rectangle);
            }
            else{
                Neill_SDL_SetDrawColour(sw, BLACK);
                SDL_RenderFillRect(sw->renderer, &rectangle);
            }
        }
    }
}
