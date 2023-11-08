#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 256
#define MAZE_N 20

typedef struct Cord {
    int x;
    int y;
} Cord;

typedef struct Maze {
    int w;
    int h;
    char grid[MAZE_N][MAZE_N];
} Maze;

void* nfopen(char* fname, char* mode);
void test(void);
void on_error(const char* s);
void print_file(FILE * fp);
void get_height_and_width(FILE * fp, int * height, int * width);
Maze get_maze(char * filename);
bool is_valid_coord(Cord c, Maze m);


int main(int argc, char* argv[]){
    test();
    if (argc!=2){
        on_error("Usage: ./maze <filename.txt>");
    }
    Maze m = get_maze(argv[1]);
    printf("%s", m.grid[0]);

    return 0;
}

Maze get_maze(char * filename)
{
    FILE* fp = nfopen(filename, "r");
    int width;
    int height;
    get_height_and_width(fp, &height, &width);
    Maze m;
    m.h = height;
    m.w = width;

    char line[LINE_LEN];
    int i = 0;
    while (fgets(line, sizeof(line), fp)!=NULL){
        strcpy(m.grid[i], line);
        i++;
    }

    fclose(fp);
    return m;
}

// does this cord fall off the edge of the maze?
bool is_valid_coord(Cord c, Maze m){
    if (c.x>=0 && c.y>=0 && c.x<m.w && c.y<m.h){
        return true;
    }
    return false;
}
// find the entrance of the maze
// and return a cord.
// does a solution exist (ie is there an exit and an entrance - end of the board.)
// you can tell you're at the edge of the board if 
// mark the solution with full stops (can explore all paths.

void get_height_and_width(FILE * fp, int * height, int * width)
{
    char first_line[LINE_LEN];
    fgets(first_line, sizeof(first_line), fp);
    if(sscanf(first_line, "%i %i", width, height)!=2){
        on_error("can't find width height in file.");
    }
      
}

void print_file(FILE * fp)
{
    char line[LINE_LEN];
    while (fgets(line, sizeof(line), fp)){
        printf("%s", line);
    }
}

void print_maze(Maze m)
{
    for (int r=0; r<m.h; r++){
        for (int c=0; c<m.w; c++){
            printf("%c ", m.grid[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}

void test(void)
{
    FILE* fp = nfopen("maze.txt", "r");
    //print_maze(fp);
    
    fclose(fp);
    Maze m;
    m.h = 1;
    m.w = 3;

    Cord c;
    c.x = 0;
    c.y = 0;
    assert(is_valid_coord(c,m));
    c.x = 3;
    c.y = 0;
    assert(is_valid_coord(c,m)==false);
    c.x = 2;
    c.y = 0;
    assert(is_valid_coord(c,m));

    // if this were real, I'd remove.
    Maze m = get_maze("maze.txt");
    
    //TODO TEST THE PRINT maze function. (just see if it works.)
    //
}




void* nfopen(char* fname, char* mode)
{
   FILE* fp;
   fp = fopen(fname, mode);
   if(!fp){
      on_error("Cannot open file");
   }
   return fp;
}

void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}
