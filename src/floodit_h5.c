#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 10
#define INF 999999999

typedef int signed_integer;

signed_integer n,m,mink;
bool vst[MAX][MAX];

signed_integer flood_path[4][2] = {
    {-1,0},
    {1,0},
    {0,1},
    {0,-1}
};

//flood and paint all possible cells... the root is (i,j)
signed_integer flood_and_paint(signed_integer cur_grid[MAX][MAX],signed_integer i, signed_integer j, signed_integer beta, signed_integer alpha, signed_integer colors[]){
    //invalid cell
    if (vst[i][j] || i < 0 || i >= n || j < 0 || j >= m)
        return 0;

    //mark existent colors
    colors[cur_grid[i][j]] = 1;

    //only alpha and beta colors counts
    if (cur_grid[i][j] != beta && cur_grid[i][j] != alpha)
        return 0;

    //mark (i,j) as visited and change its color
    vst[i][j] = true;
    cur_grid[i][j] = alpha;

    //floodit !
    signed_integer ret = 1;
    for (signed_integer k = 0; k < 4; k++)
        ret += flood_and_paint(cur_grid,i + flood_path[k][0], j + flood_path[k][1], beta, alpha, colors);

    //how many cells change
    return ret;
}

void backtrack(signed_integer cur_grid[MAX][MAX],signed_integer k,signed_integer _cont, signed_integer alpha) {
    //bigger number of clicks for this solution ? ... getting back
    if(k >= mink)
        return;

    signed_integer colors[10];
    memset(vst, false, sizeof(vst));
    memset(colors, 0, sizeof(colors));

    signed_integer beta = cur_grid[0][0];
    signed_integer cont = flood_and_paint(cur_grid, 0, 0, beta, alpha, colors);

    //there are alpha colors to change and no beta colors to change
    colors[alpha] = 1;
    colors[beta]  = 0;

    //all squares on same color
    if (cont == n * m) {
        mink = k;
        return;
    }

    //this solution is equals to another ? ... getting back
    if (cont == _cont)
        return;

    ++k;//new click

    //copy this matrix and backtrack
    signed_integer copy[MAX][MAX];
    for (signed_integer c = 0; c < 10; ++c){
        if (colors[c] && c != cur_grid[0][0]) {
            memcpy(copy, cur_grid,n*m*sizeof(signed_integer));
            backtrack(copy,k,cont,c);
        }
    }
}

void cleanBuffer(){
     while (getchar() != '\n');
}

int main(void) {
    signed_integer grid[MAX][MAX];
    scanf("%d %d",&n,&m);
    for (signed_integer i = 0; i < n; ++i) {
        cleanBuffer();
        for (signed_integer j = 0; j < m; ++j){
            grid[i][j] = getchar() - '0';
        }
    }
    mink = INF;
    backtrack(grid,0, 0, grid[0][0]);
    printf("%d\n",mink);
    return 0;
}