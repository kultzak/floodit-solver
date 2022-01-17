#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "lib/mapa.h"


void insere_plano(tplano *p, int cor) {
  p->cor[p->passos] = cor;
  p->passos++;
}


void remove_plano(tplano *p) {
  p->passos--;
}

void copia_plano(tplano *po, tplano *pd) {
  int i;

  pd->solucao = po->solucao;
  pd->passos = po->passos;
  for (i = 0; i < po->passos; i++)
    pd->cor[i] = po->cor[i];
}


tplano * aloca_plano(tmapa *m) {
  tplano *p;
  int i, j;

  p = (tplano *) malloc(sizeof(tplano));
  p->solucao = 0;
  p->passos = 0;
  p->cor = (int *) malloc(m->nlinhas * m->ncolunas * sizeof(int));

  p->prob = (float**) malloc(m->nlinhas * m->ncolunas * sizeof(float*));
  for (i = 0; i < m->nlinhas * m->ncolunas; i++){
    p->prob[i] = (float*) malloc(m->ncores * sizeof(float));
    for (j = 0; j < m->ncores; j++)
      p->prob[i][j] = 0;
  }
  return p;
}


void libera_plano(tplano *p) {
  free(p->cor);
  free(p);
}

void carrega_plano(tplano *p) {
  int i;

  scanf("%d", &(p->passos));
  for (i = 0; i < p->passos; i++)
    scanf("%d", &(p->cor[i]));
}

void mostra_plano(tplano *p) {
  int i;

  printf("%d\n", p->passos);
  for (i = 0; i < p->passos; i++)
    printf("%d ", p->cor[i]);
  printf("\n");
}

void simula_plano(tmapa *m, tplano *p) {
  int i;
  tmapa *ms;

  ms = aloca_mapa(m);
  copia_mapa(m, ms);
  mostra_mapa(ms);
  printf("\n");
  for (i = 0; i < p->passos; i++) {
    pinta_mapa(ms, p->cor[i]);
    printf("%d\n", p->cor[i]);
    mostra_mapa(ms);
    printf("\n");
  }
  libera_mapa(ms);
}

tfronteira * aloca_fronteira(tmapa *m) {
  tfronteira *f;

  f = (tfronteira *) malloc(sizeof(tfronteira));
  f->tamanho = 0;
  f->pos = (tpos *) malloc(m->nlinhas * m->ncolunas * sizeof(tpos));
  return f;
}

void libera_fronteira(tfronteira *f) {
  free(f->pos);
  free(f);
}

void mostra_fronteira(tfronteira *f) {
  int i;

  for (i = 0; i < f->tamanho; i++)
    printf("(%d,%d):%d ", f->pos[i].l, f->pos[i].c, f->pos[i].v);
  printf("\nFRONTEIRA: %d\n", f->tamanho);
}

int compara_pos(int l1, int c1, int l2, int c2) {
  if (l1 < l2)
    return -1;
  if (l1 > l2)
    return 1;
  if (c1 < c2)
    return -1;
  if (c1 > c2)
    return 1;
  return 0;
}

void insere_fronteira(tfronteira *f, int l, int c, int v) {
  int i;
  int j;
  int cmp;

  for (i = 0; i < f->tamanho; i++) {
    cmp = compara_pos(f->pos[i].l, f->pos[i].c, l, c);
    if (!cmp)
      return;
    if (cmp > 0)
      break;
  }
  for (j = f->tamanho; j > i; j--) {
    f->pos[j].l = f->pos[j - 1].l;
    f->pos[j].c = f->pos[j - 1].c;
    f->pos[j].v = f->pos[j - 1].v;
  }
  f->pos[i].l = l;
  f->pos[i].c = c;
  f->pos[i].v = v;
  f->tamanho++;
}

void copia_mapa(tmapa *mo, tmapa *md)  {
  int i, j;

  for (i = 0; i < md->nlinhas; i++)
    for (j = 0; j < md->ncolunas; j++)
      md->mapa[i][j] = mo->mapa[i][j];
}

tmapa * aloca_mapa(tmapa *mo) {
  int i;
  tmapa *md;

  md = (tmapa *) malloc(sizeof(tmapa));
  md->nlinhas = mo->nlinhas;
  md->ncolunas = mo->ncolunas;
  md->ncores = mo->ncores;
  md->mapa = (int**) malloc(md->nlinhas * sizeof(int*));
  for (i = 0; i < md->nlinhas; i++)
    md->mapa[i] = (int*) malloc(md->ncolunas * sizeof(int));
  return md;
}

void libera_mapa(tmapa *m)  {
  int i;

  for (i = 0; i < m->nlinhas; i++)
    free(m->mapa[i]);
  free(m->mapa);
  free(m);
}

void gera_mapa(tmapa *m, int semente) {
  int i, j;

  if (semente)
    srand(semente);
  else
    srand(time(NULL));
  m->mapa = (int**) malloc(m->nlinhas * sizeof(int*));
  for (i = 0; i < m->nlinhas; i++) {
    m->mapa[i] = (int*) malloc(m->ncolunas * sizeof(int));
    for (j = 0; j < m->ncolunas; j++)
      m->mapa[i][j] = 1 + rand() % m->ncores;
  }
}

void carrega_mapa(tmapa *m) {
  int i, j;

  scanf("%d", &(m->nlinhas));
  scanf("%d", &(m->ncolunas));
  scanf("%d", &(m->ncores));
  m->mapa = (int**) malloc(m->nlinhas * sizeof(int*));
  for (i = 0; i < m->nlinhas; i++) {
    m->mapa[i] = (int*) malloc(m->ncolunas * sizeof(int));
    for (j = 0; j < m->ncolunas; j++)
      scanf("%d", &(m->mapa[i][j]));
  }
}

void mostra_mapa(tmapa *m) {
  int i, j;

  printf("%d %d %d\n", m->nlinhas, m->ncolunas, m->ncores);
  for (i = 0; i < m->nlinhas; i++) {
    for (j = 0; j < m->ncolunas; j++)
      if (m->ncores > 10)
        printf("%02d ", m->mapa[i][j]);
      else
        printf("%d ", m->mapa[i][j]);
    printf("\n");
  }
}

void salva_mapa(tmapa *m){
    int i, j;
    char r;
    //the space before %c clear a blank character left in a previous scanf. https://stackoverflow.com/questions/18372421/scanf-is-not-waiting-for-user-input
    printf("Do you want to save this map?[y/n]");

    r = getchar();
    if (r == '\n') r = getchar();
    while (r != 'n' && r != 'N' && r != 'y' && r != 'Y')
    {
      printf("invalid input, enter the choice again (y/Y/n/N): ");
      r = getchar();
      if (r == '\n') r = getchar();
    }
    if (r == 'y' || r == 'Y')
    {
      FILE *fp;
      // the array is cast as a const char*.
      char result[100];   
      char buffer[128];
      strcpy(result,"Output_"); // copy string one into the result.
      sprintf(buffer, "%d", m->nlinhas);
      strcat(result,buffer); // append string two to the result.
      strcat(result,"_"); 
      sprintf(buffer, "%d", m->ncolunas);
      strcat(result,buffer); // append string two to the result.
      strcat(result,"_"); 
      sprintf(buffer, "%d", m->ncores);
      strcat(result,buffer); // append string two to the result.
      strcat(result,".txt"); 
      fp = fopen(result, "w");
      fprintf(fp,"%d %d %d\n", m->nlinhas, m->ncolunas, m->ncores);
      for (i = 0; i < m->nlinhas; i++) {
        for (j = 0; j < m->ncolunas; j++)
          if (m->ncores > 10)
            fprintf(fp,"%02d ", m->mapa[i][j]);
          else
            fprintf(fp,"%d ", m->mapa[i][j]);
        fprintf(fp,"\n");
      }
      fclose(fp); //Don't forget to close the file when finished
      printf("Map saved as: %d_%d_%d.fldmap\n",m->nlinhas,m->ncolunas,m->ncores);
      printf("Type anything to continue to game.");
      getchar(); //just to wait showing the save path
    }
}

void salva_plano(){
    char r;
    //the space before %c clear a blank character left in a previous scanf. https://stackoverflow.com/questions/18372421/scanf-is-not-waiting-for-user-input
    printf("Do you want to save this solution for this map?[y/n]");

    r = getchar();
    if (r == '\n') r = getchar();
    while (r != 'n' && r != 'N' && r != 'y' && r != 'Y')
    {
      printf("invalid input, enter the choice again (y/Y/n/N): ");
      r = getchar();
      if (r == '\n') r = getchar();
    }
    if (r == 'y' || r == 'Y')
    {
      FILE *fp;
      // the array is cast as a const char*.
      char result[100];   
      strcpy(result,"Solution_"); // copy string one into the result.
      strcat(result,".txt"); 
      fp = fopen(result, "w");
      fprintf(fp,"Solução\n");
      fclose(fp); //Don't forget to close the file when finished
      printf("The solution was saved as: Solution_.fldplan\n");
      printf("Type anything to continue to go back to main menu.");
      getchar(); //just to wait showing while the plan path 
    }
}

void mostra_mapa_cor(tmapa *m) {
  int i, j;
  char* cor_ansi[] = { "\x1b[0m",
                       "\x1b[31m", "\x1b[32m", "\x1b[33m",
                       "\x1b[34m", "\x1b[35m", "\x1b[36m",
                       "\x1b[37m", "\x1b[30;1m", "\x1b[31;1m",
                       "\x1b[32;1m", "\x1b[33;1m", "\x1b[34;1m",
                       "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m"
                     };

  if (m->ncores > 15) {
    mostra_mapa(m);
    return;
  }
  printf("%d %d %d\n", m->nlinhas, m->ncolunas, m->ncores);
  for (i = 0; i < m->nlinhas; i++) {
    for (j = 0; j < m->ncolunas; j++)
      if (m->ncores >= 10)
        printf("%s%02d%s ", cor_ansi[m->mapa[i][j]], m->mapa[i][j], cor_ansi[0]);
      else
        printf("%s%d%s ", cor_ansi[m->mapa[i][j]], m->mapa[i][j], cor_ansi[0]);
    printf("\n");
  }
}

void pinta(tmapa *m, int l, int c, int fundo, int cor) {
  m->mapa[l][c] = cor;
  if (l < m->nlinhas - 1 && m->mapa[l + 1][c] == fundo)
    pinta(m, l + 1, c, fundo, cor);
  if (c < m->ncolunas - 1 && m->mapa[l][c + 1] == fundo)
    pinta(m, l, c + 1, fundo, cor);
  if (l > 0 && m->mapa[l - 1][c] == fundo)
    pinta(m, l - 1, c, fundo, cor);
  if (c > 0 && m->mapa[l][c - 1] == fundo)
    pinta(m, l, c - 1, fundo, cor);
}

void pinta_mapa(tmapa *m, int cor) {
  if (cor == m->mapa[0][0])
    return;
  pinta(m, 0, 0, m->mapa[0][0], cor);
}

void limpa_mapa(tmapa *m) {
  int i;
  int j;

  for (i = 0; i < m->nlinhas; i++)
    for (j = 0; j < m->ncolunas; j++)
      if (m->mapa[i][j] < 0)
        m->mapa[i][j] *= -1;
}

void fronteira(tmapa *m, int l, int c, int fundo, tfronteira *f) {
  if (m->mapa[l][c] == fundo) {
    m->mapa[l][c] *= -1;
    if (l < m->nlinhas - 1)
      fronteira(m, l + 1, c, fundo, f);
    if (c < m->ncolunas - 1)
      fronteira(m, l, c + 1, fundo, f);
    if (l > 0)
      fronteira(m, l - 1, c, fundo, f);
    if (c > 0)
      fronteira(m, l, c - 1, fundo, f);
  }
  else if (m->mapa[l][c] != -fundo) {
    insere_fronteira(f, l, c, 0);
  }
}

void fronteira_mapa(tmapa *m, tfronteira *f) {
  f->tamanho = 0;
  fronteira(m, 0, 0, m->mapa[0][0], f);
  limpa_mapa(m);
}

//#define ROW 5
//#define COL 5

// A function to check if a given cell (row, col) can be included in DFS
int isSafe(tmapa *m, int row, int col, bool visited[][m->ncolunas], int cor)
{
  // row number is in range, column number is in range and value is 1
  // and not yet visited
  int ok;
  if (row < 0 || col < 0 || row >= m->nlinhas || col >= m->ncolunas)
    return 0;

  //printf("col = %d x row %d \n", col, row);

  if (m->mapa[row][col] == cor) {
    ok = 1;
  } else {
    ok = 0;
  }
  return (ok && !visited[row][col]);
}

// A utility function to do DFS for a 2D boolean matrix. It only considers
// the 8 neighbours as adjacent vertices
void DFS(tmapa *m, int row, int col, bool visited[][m->ncolunas], int cor)
{
  // These arrays are used to get row and column numbers of 8 neighbours
  // of a given cell
  static int rowNbr[] = { -1,  0, 0, 1};
  static int colNbr[] = {  0, -1, 1, 0};

  // Mark this cell as visited
  visited[row][col] = true;

  // Recur for all connected neighbours
  for (int k = 0; k < 4; ++k)
    if (isSafe(m, row + rowNbr[k], col + colNbr[k], visited, cor) )
      DFS(m, row + rowNbr[k], col + colNbr[k], visited, cor);
}

// The main function that returns count of islands in a given boolean
// 2D matrix
//int ilha_mapa(int M[][COL])
int ilha_mapa(tmapa *m, int cor)
{
  // Make a bool array to mark visited cells.
  // Initially all cells are unvisited
  bool visited[m->nlinhas][m->ncolunas];
  memset(visited, 0, sizeof(visited));

  // Initialize count as 0 and travese through the all cells of
  // given matrix
  int count = 0;
  for (int i = 0; i < m->nlinhas; ++i)
    for (int j = 0; j < m->ncolunas; ++j) {
      if (m->mapa[i][j] == cor && !visited[i][j]) // If a cell with value 1 is not
      { // visited yet, then new island found
        DFS(m, i, j, visited, cor);     // Visit all cells in this island.
        ++count;                   // and increment island count
      }
    }


  return count;
}

// Driver program to test above function
// int main()
// {
//   int M[][COL] =
//   { {1, 1, 0, 0, 0},
//     {0, 1, 0, 0, 1},
//     {0, 0, 1, 0, 1},
//     {0, 0, 0, 0, 0},
//     {1, 0, 1, 0, 1}
//   };

//   printf("Number of islands is: %d\n", ilha_mapa(M));

//   return 0;
// }
