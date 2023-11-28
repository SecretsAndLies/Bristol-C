#include <stdio.h>
#include <stdlib.h>
#define N 21
void fillboard(int board[N][N]);
void printboard(int board[N][N]);
int randN(int low, int high);
void mutateboard(int board[N][N]);
void swap_2d_array(int board[N][N], int r, int c, int sr, int sc);
void smutate(int board[N][N]);

int main(void){
    int board[N][N];
    fillboard(board);
    printboard(board);
    for (int i=0; i<N*N*N*N; i++){
        smutate(board);
    }
    printboard(board);
}

void fillboard(int board[N][N]){
    for(int r=0; r<N; r++){
        for (int c=0; c<N; c++){
            board[r][c]=rand()%10;
        }
    }
}

void smutate(int board[N][N]){
    int r = randN(0, N-1);
    int c = randN(0, N-1);
    int r2 = randN(0, N-1);
    int c2 = randN(0, N-1);
    int dis = (r*c)-(N/2*N/2);
    int dis2 = (r2*c2)-(N/2*N/2);
    if (dis<dis2){
        if(board[r][c]>board[r2][c2]){
            swap_2d_array(board, r, c, r2, c2);
        }
    }
}

void mutateboard(int board[N][N]){
    int r = randN(0, N-1);
    int c1 = randN(1,N-1);
    if(board[r][c1-1]>board[r][c1]){
        swap_2d_array(board, r,c1-1,r,c1);
    };

    int r1 = randN(1,N-1);
    int c = randN(0,N-1);
    if(board[r1][c]<board[r1-1][c]){
        swap_2d_array(board, r1,c,r1-1,c);
    }

}

void swap_2d_array(int board[N][N], int r, int c, int sr, int sc)
{
    int temp = board[r][c];
    board[r][c]=board[sr][sc];
    board[sr][sc]=temp; 
}

/* gives a random number between low and high, inclusive*/
int randN(int low, int high){
    // Scale and shift the output of rand() to the desired range [low, high]
    return low + (rand() % (high - low + 1));
}

void printboard(int board[N][N]){
    for(int r=0; r<N; r++){
        for (int c=0; c<N; c++){
            printf("%i", board[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}
    

