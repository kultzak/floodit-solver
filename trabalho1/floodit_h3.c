#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"

int main(int argc, char **argv) {
  int i;
  int cor;
  int aux_cor;
  int mcor;
  int aux_ilha_mapa;
  int *ncorfront;
  tmapa m;
  tfronteira *f;
  tplano *p;

  carrega_mapa(&m);

  f = aloca_fronteira(&m);
  p = aloca_plano(&m);

  ncorfront = (int*) malloc((m.ncores + 1) * sizeof(int));


  cor = m.mapa[0][0];
  aux_cor = m.mapa[0][0];
  fronteira_mapa(&m, f);

  while (f->tamanho > 0) {
    for (i = 1; i <= m.ncores; i++)
      ncorfront[i] = 0;              //inicializa a quantidade das cores da fronteira com 0

    for (i = 0; i < f->tamanho; i++)
      ncorfront[m.mapa[f->pos[i].l][f->pos[i].c]]++; //itera somando cada cor da fronteira, linha - coluna
    mcor = 0;
    aux_ilha_mapa = 0;
    for (i = 1; i <= m.ncores; i++) { //etapa para retornar a cor da fronteira com o maior numero de vertices restantes
      if (ncorfront[i] > 0) { //se a cor está presente na fronteira então vai para a contagem de vertices dessa cor
        printf("Numero de ilhas na cor %d = %d\n", cor, ilha_mapa(&m, cor));
        aux_ilha_mapa = ilha_mapa(&m, cor);
        if (aux_ilha_mapa > mcor) {
          mcor = aux_ilha_mapa;
          printf("cor escolhida =  %d\n", cor);
          aux_cor = cor;
        }
         cor = i;
      }
    }
    printf("\n");
    pinta_mapa(&m, aux_cor);
    insere_plano(p, aux_cor);
    fronteira_mapa(&m, f);
  }
  mostra_plano(p);

  return 0;
}

