#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"
#include <time.h>
#include <unistd.h>


void perturbation(tplano pop[], int ncores)
{
	int r = 6;
	int pos = 0;
	int perturbado = rand() % 10;
	int array_pos[pop[perturbado].passos];
	int array_cor[r];

	for (int pert = 10; pert < 11; ++pert) //onde o indivíduo perturbado será salvo
	{

		for (int i = 0; i < pop[perturbado].passos - 1; i++) {   // fill array
			array_pos[i] = i;
		}
		for (int i = 0; i < pop[perturbado].passos - 1; i++) {  // shuffle array
			int temp = array_pos[i];
			int randomIndex = rand() % pop[perturbado].passos - 1;
			array_pos[i]           = array_pos[randomIndex];
			array_pos[randomIndex] = temp;
		}

		for (int i = 0; i < r; i++) {    // print array
			array_cor[i] = array_pos[i];
		}

		for (int i = 0; i < r; i++)                     //Loop for ascending ordering
		{
			for (int j = 0; j < r; j++)             //Loop for comparing other values
			{
				if (array_cor[j] > array_cor[i])                //Comparing other array elements
				{
					int tmp = array_cor[i];         //Using temporary variable for storing last value
					array_cor[i] = array_cor[j];            //replacing value
					array_cor[j] = tmp;             //storing last value
				}
			}
		}

		pop[pert].passos = pop[perturbado].passos + r;

		int j = 0;
		for (int i = 0; i < pop[pert].passos; ++i)
		{
			if (i - j == array_cor[pos])
			{
				pos++;
				int adn = rand() % (ncores + 1 - 1) + 1;
				if (adn == pop[perturbado].cor[i - 1] || adn == pop[perturbado].cor[i + 1])
				{
					adn++;
					if (adn == ncores + 1)
					{
						adn = adn - 2;
					}
				}
				pop[pert].cor[i] = adn;
				i++;
				j++;
			}
			pop[pert].cor[i] = pop[perturbado].cor[i - j];

		}
		pos = 0;

		// for (int i = 0; i < pop[perturbado].passos; i++)
		// 	printf("%d ", pop[perturbado].cor[i]);
		// printf("\n");

		// for (int i = 0; i < pop[pert].passos; i++)
		// 	printf("%d ", pop[pert].cor[i]);
		// printf("\n");
	}
	printf("OK 4\n");

}


void local_search(tplano pop[], tmapa mo)
{
	tmapa *md;
	tfronteira *f;
	md = aloca_mapa(&mo);


	for (int indi = 0; indi < 10; ++indi)
	{

		pop[52].passos = pop[indi].passos - 1; //LEMBRAR DA PROBABILIDADE TAMBÈM

		for (int pula = 0; pula < pop[52].passos; ++pula)
		{

			for (int i_orig = 0; i_orig < pop[indi].passos - 1; ++i_orig)
			{

				for (int i = 0; i < pop[indi].passos; ++i)
				{

					if (i == pula)
					{
						i_orig++;
					}
					pop[52].cor[i] = pop[indi].cor[i_orig];
					i_orig++;
				}
				// printf("\n");
				copia_mapa(&mo, md);
				f = aloca_fronteira(md);
				// printf("%d\n", pop[52].passos);
				for (int i = 0; i < pop[52].passos; i++) {
					pinta_mapa(md, pop[52].cor[i]);
					fronteira_mapa(md, f);
				}
				if (!f->tamanho) {
					pula = pop[52].passos;
					pop[indi].passos--;
					for (int i = 0; i < pop[indi].passos; ++i)
					{
						pop[indi].cor[i] = pop[52].cor[i];
					}
				}
			}
		}
	}
}

void recombination(tmapa mo, tplano pop[], int k, float lambda, int ncores)
{
	tmapa *md;
	tfronteira *f;
	int *ncorfront;
	int ncor;
	float y;
	float a = -1;
	float b = 1;
	float probe = 0;
	float mprob = 0;
	int pai1 = rand() % 5;
	int pai2 = rand() % (15 + 1 - 5) + 5;

	md = aloca_mapa(&mo);

	for (int i = 0; i < pop[pai2].passos; ++i) //realiza a média das probabilidades dos dois pais
	{
		for (int j = 0; j < ncores; ++j)
		{
			y = ((float)rand() / RAND_MAX) * (b - a) + a;
			pop[50].prob[i][j] = ((pop[pai1].prob[i][j] + pop[pai2].prob[i][j]) / 2) * (1 + y * lambda);
			// y = ((float)rand() / RAND_MAX) * (b - a) + a;
			pop[51].prob[i][j] = ((pop[pai1].prob[i][j] + pop[pai2].prob[i][j]) / 2) * (1 + y * lambda);
			// printf("%f\n", pop[50].prob[i][j]);
		}
	}

	for (int son = 50; son < 52; ++son)
	{
		copia_mapa(&mo, md);
		// // mostra_mapa_cor(md); // para mostrar o mapa colorido

		f = aloca_fronteira(md);
		int passo = 0;

		ncorfront = (int*) malloc((md->ncores + 1) * sizeof(int));
		// cor = md->mapa[0][0];
		fronteira_mapa(md, f);

		while (f->tamanho > 0) {
			for (int i = 1; i <= md->ncores; i++)
				ncorfront[i] = 0;              //inicializa a quantidade das cores da fronteira com 0
			for (int i = 0; i < f->tamanho; i++)
				ncorfront[md->mapa[f->pos[i].l][f->pos[i].c]]++; //itera somando cada cor da fronteira
			ncor = 0;
			for (int i = 1; i <= md->ncores; i++) {

				if (pop[son].prob[passo][i - 1] == 0)
				{
					pop[son].prob[passo][i - 1] = ((ncorfront[i] * k + (float)(rand() % 40 + 1)) / 1000);
				}

				probe = pop[son].prob[passo][i - 1];

				if (ncorfront[i] == 0) {
					pop[son].prob[passo][i - 1] = 0;
					probe = 0;
				}

				if (probe > mprob) {
					mprob = probe;
					//  printf("a melhor probabilidade eh = %f na cor %d\n", prob, i);
					pop[son].cor[passo] = i;
				}

				if (ncorfront[i] > ncor) { //retorna a cor que mais aparece na fronteira
					ncor = ncorfront[i];
					// cor = i;
				}

			}
			pinta_mapa(md, pop[son].cor[passo]);
			// printf("PINTADO NA COR %d DURANTE A ETAPA %d\n", ini_pop[50].cor[passo], passo);
			// printf("\n"); //separa as probalidades
			passo++;
			pop[son].passos = passo;
			mprob = 0;
			fronteira_mapa(md, f);
		}
		// printf("FILHO: %d\n", ini_pop[son].passos); //imprime indivíduo gerado
		// for (i = 0; i < ini_pop[son].passos; i++)
		//   printf("%d ", ini_pop[son].cor[i]);
		// printf("\n");
	}



}

void swap(tplano *xp, tplano *yp)
{
	tplano temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void selectionSort(tplano arr[], int n)
{
	int i, j, min_idx;
	for (i = 0; i < n - 1; i++)
	{
		min_idx = i;
		for (j = i + 1; j < n; j++)
			if (arr[j].passos < arr[min_idx].passos)
				min_idx = j;
		swap(&arr[min_idx], &arr[i]);
	}
}

int main(int argc, char **argv) {
	int i;
	int j;
	int aux_cor;
	int ncor;
	int generation;
	int g_inc;
	int *ncorfront;
	tmapa mo;
	tmapa *md;
	tfronteira *f;
	tplano *p;
	tplano *m_p;
	float probe;
	float mprob;
	float k;
	float d;
	float lambda;
	float inc;
	time_t t;
	time_t endwait;
	time_t start = time(NULL);
	int tam_pop;
	tam_pop = 1000;
	tplano ini_pop[tam_pop];

	probe = 0;
	mprob = 0;
	generation = 0;

//alguns parametros de tunning do algoritmo
	k = 19; // parâmetro de ponderação
	d = 500; //parâmetro de escala
	lambda = 0.1; //parâmetro
	g_inc = 1;
	inc = 0.02;
	time_t seconds = 60; // end loop after this time has elapsed

	endwait = start + seconds;

	carrega_mapa(&mo);
	md = aloca_mapa(&mo);
	srand((unsigned)(time(&t)));

	m_p = aloca_plano(md);

//-----------------------//CRIAR POPULAÇÃO INICIAL//-----------------------//
	for (j = 0; j < tam_pop; j++) {

		copia_mapa(&mo, md);

		f = aloca_fronteira(md);
		p = aloca_plano(md);


		ncorfront = (int*) malloc((md->ncores + 1) * sizeof(int));
		aux_cor = md->mapa[0][0];
		fronteira_mapa(md, f);

		while (f->tamanho > 0) {
			for (i = 1; i <= md->ncores; i++)
				ncorfront[i] = 0;              //inicializa a quantidade das cores da fronteira com 0
			for (i = 0; i < f->tamanho; i++)
				ncorfront[md->mapa[f->pos[i].l][f->pos[i].c]]++; //itera somando cada cor da fronteira
			ncor = 0;
			for (i = 1; i <= md->ncores; i++) {
				//  printf("%d - cor %d,", ncorfront[i], i); //imprime a quantidade de celulas por cor na fronteira

				probe = ((ncorfront[i] * k + (float)(rand() % 40 + 1)) / d);

				if (ncorfront[i] == 0)
					probe = 0;
				//  printf("(%d * %f + %d)/%f = %9.6f\n", ncorfront[i], k, (rand() % 100 + 1), d, prob);

				p->prob[p->passos][i - 1] = probe;
				// printf("%f\n", p->prob[p->passos][i - 1]);

				if (probe > mprob) {
					mprob = probe;
					//  printf("a melhor probabilidade eh = %f na cor %d\n", prob, i);
					aux_cor = i;
				}

				if (ncorfront[i] > ncor) { //retorna a cor que mais aparece na fronteira
					ncor = ncorfront[i];
					// cor = i;
				}

			}
			//printf("\n"); //separa as probalidades
			pinta_mapa(md, aux_cor);
			insere_plano(p, aux_cor);
			mprob = 0;
			fronteira_mapa(md, f);
		}
		// mostra_plano(p);
		ini_pop[j] = *p; //insere um individuo dentro da população
		if (j == 0) {
			copia_plano(p, m_p);
		}
	}

//-----------------------//ORDENAR POPULAÇÃO//-----------------------//
	selectionSort(ini_pop, tam_pop);

	// printf("start time is : %s", ctime(&start));


	while (start < endwait) //função timer para englobar
	{
		start = time(NULL);
		// if (generation == 0)
		// {
		// 	local_search(ini_pop, mo);
		// }

		//recombination(mo, ini_pop, k, lambda, md->ncores); //COMENTANDO ESSA LINHA FUNCIONA COMO UM ILS

		generation++;

		printf("GERAÇÃO: %d\n", generation);
		if (generation % g_inc == 0)
		{
			if (lambda > 1)
			{
				lambda = 0;
			}
			else {
				for (int i = 0; i < 6; ++i)
				{
					perturbation(ini_pop, md->ncores);
					local_search(ini_pop, mo);
				}
				lambda = lambda + inc;
			}
		}
		selectionSort(ini_pop, 52); //se der um sort no tamanho da população ele puxa o 52 que foi decrementado

		printf("MELHOR RESULTADO: %d\n", ini_pop[0].passos);
		// printf("RESULTADO 50: %d\n", ini_pop[49].passos);
		// for (int i = 0; i < 50; ++i) //teste para saber se está passando o array
		// {
		//   printf("%d\n", ini_pop[i].passos);
		// }
		// printf("\033c");

	}


//-----------------------//APRESENTA O MELHOR RESULTADO//-----------------------//
	printf("MELHOR RESULTADO: %d\n", ini_pop[0].passos);
	for (i = 0; i < ini_pop[0].passos; i++)
		printf("%d ", ini_pop[0].cor[i]);
	printf("\n");
	printf("start time is : %s", ctime(&start));
	printf("end time is %s", ctime(&endwait));

	printf("MELHOR RESULTADO: %d\n", ini_pop[0].passos);
	// int b = 0;
	// // while (b < 1000) {
	// // 	printf("%d\n", ini_pop[b].passos);
	// // 	// for (i = 0; i < ini_pop[b].passos; i++)
	// // 	// 	printf("%d ", ini_pop[b].cor[i]);
	// // 	// printf("\n");
	// // 	b++;
	// // }


	return 0;
}

