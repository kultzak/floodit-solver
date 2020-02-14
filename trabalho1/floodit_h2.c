#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"

int main(int argc, char **argv) {
  int i;
  int cor;
  tmapa m;
  tfronteira *f;
  tplano *p;

  carrega_mapa(&m);

  f = aloca_fronteira(&m);
  p = aloca_plano(&m);
  
  cor = m.mapa[0][0];
  fronteira_mapa(&m, f);

 // printf("OK");

  i = 10;
  while(f->tamanho > 0) {
      cor = i;
      i++;
      if (i == m.ncores + 1){
      i = 1;
      }
      pinta_mapa(&m, cor);
      insere_plano(p, cor);
      fronteira_mapa(&m, f);
  }
  mostra_plano(p);
  

  return 0;
}
  
