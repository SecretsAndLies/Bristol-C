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

Cord get_entrance(Maze m);
void* nfopen(char* fname, char* mode);
void test(void);
void on_error(const char* s);
void print_file(FILE * fp);
void get_height_and_width(FILE * fp, int * height, int * width);
Maze get_maze(char * filename);
bool is_valid_cord(Cord c, Maze m);
void mark_cord(Maze * m, Cord c);
Cord get_exit(Maze m);
bool are_cords_equal(Cord a, Cord b);
int search_and_mark(Maze * m, Cord c, Cord exit);
void print_maze(Maze m);


int main(int argc, char* argv[]){
    test();
    if (argc!=2){
        on_error("Usage: ./maze <filename.txt>");
    }
    Maze m = get_maze(argv[1]);
    Cord entrance = get_entrance(m);
    Cord exit = get_exit(m);
    int a = search_and_mark(&m, entrance, exit);
    if(a){
        print_maze(m);
        return 0;
    }
    return 1;
}

int search_and_mark(Maze * m, Cord c, Cord exit)
{
    mark_cord(m, c); // mark as .
    if (are_cords_equal(c,exit)){
        return 1;
    }
    
    int total = 0;
    // left
    Cord left;
    left.x=c.x-1;
    left.y=c.y;
    if(is_valid_cord(left, *m) && m->grid[left.y][left.x]==' '){
        total+=search_and_mark(m, left, exit);
    }
    // right
    Cord right;
    right.x=c.x+1;
    right.y=c.y;
    if(is_valid_cord(right, *m) && m->grid[right.y][right.x]==' '){
        total+=search_and_mark(m, right, exit);
    }
    // up
    Cord up;
    up.x=c.x;
    up.y=c.y-1;
    if(is_valid_cord(up, *m) && m->grid[up.y][up.x]==' '){
        total+=search_and_mark(m, up, exit);
    }
    // down
    Cord down;
    down.x=c.x;
    down.y=c.y+1;
    if(is_valid_cord(down, *m) && m->grid[down.y][down.x]==' '){
        total+=search_and_mark(m, down, exit);
    }
    return total;

}

bool are_cords_equal(Cord a, Cord b)
{
    return a.x==b.x && a.y==b.y;
}

void mark_cord(Maze * m, Cord c){
    if(!is_valid_cord(c, *m)){
        return;
    }
    if(m->grid[c.y][c.x]==' '){
        m->grid[c.y][c.x]='.';
    }
}

Cord get_exit(Maze m){
   Cord c;
   // on right side
    for (int i=0; i<m.h; i++){
        if(m.grid[i][m.w-1]==' '){
            c.x = m.w-1;
            c.y = i;
            return c;
        }
   }

    for (int i=0; i<m.w; i++){
        if(m.grid[m.h-1][i]==' '){
            c.x = i;
            c.y = m.h-1;
            return c;
        }
    }

    fprintf(stderr, "no exit found. \n");
    exit(EXIT_FAILURE);

    // this will never happen
    return c;
 
}

Cord get_entrance(Maze m){
   Cord c;
   // on left side
    for (int i=0; i<m.h; i++){
        if(m.grid[i][0]==' '){
            c.x = 0;
            c.y = i;
            return c;
        }
   }

    for (int i=0; i<m.w; i++){
        if(m.grid[0][i]==' '){
            c.x = i;
            c.y = 0;
            return c;
        }
    }

    fprintf(stderr, "no entrance found. \n");
    exit(EXIT_FAILURE);

    // this will never happen
    return c;
 
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
bool is_valid_cord(Cord c, Maze m){
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
    assert(is_valid_cord(c,m));
    c.x = 3;
    c.y = 0;
    assert(is_valid_cord(c,m)==false);
    c.x = 2;
    c.y = 0;
    assert(is_valid_cord(c,m));

    // if this were real, I'd remove.
    Maze j = get_maze("maze.txt");
    Cord d = get_entrance(j);
    assert(d.x==0);
    assert(d.y==1);
    Cord e = get_exit(j);
    printf("%c %c \n", e.x, e.y);
    assert(e.x==9);
    assert(e.y==8);
    
    mark_cord(&j,d);

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
