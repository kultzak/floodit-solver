#include "floodit_ga.h"
#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"
#include <time.h>
#include <unistd.h>

/* progressbar */
#include "include/progressbar/progressbar.h"
#include "include/progressbar/statusbar.h"

#define SLEEP_US 1000000


void perturbation(tplano pop[], int ncores)
{
	int r = 6;
	int pos = 0;
	int array_cor[r];

	for (int pert = 10; pert < 15; ++pert) //onde o indivíduo perturbado será salvo
	{
		int perturbado = rand() % 10;
		int array_pos[pop[perturbado].passos];

		for (int i = 0; i < pop[perturbado].passos - 1; i++) {
			array_pos[i] = i;
		}

		for (int i = 0; i < pop[perturbado].passos - 1; i++) {
			int temp = array_pos[i];
			int randomIndex = rand() % (pop[perturbado].passos - 1);
			array_pos[i]           = array_pos[randomIndex];
			array_pos[randomIndex] = temp;
		}

		for (int i = 0; i < r; i++) {
			array_cor[i] = array_pos[i];
		}

		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < r; j++)
			{
				if (array_cor[j] > array_cor[i])
				{
					int tmp = array_cor[i];
					array_cor[i] = array_cor[j];
					array_cor[j] = tmp;
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
	}

}


void local_search(tplano pop[], tmapa mo, int t_busca)
{
	tmapa *md;
	tfronteira *f;
	md = aloca_mapa(&mo);

	for (int indi = 0; indi < t_busca; ++indi)
	{

		pop[52].passos = pop[indi].passos - 1;

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
				// printf("OK 5\n");
				for (int i = 0; i < pop[52].passos; i++) {
					pinta_mapa(md, pop[52].cor[i]);
					fronteira_mapa(md, f);
				}
				// printf("OK 6\n");
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

	for (int son = 10; son < 20; ++son) //local para salvar os filhos recombinados
	{

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

		copia_mapa(&mo, md);

		f = aloca_fronteira(md);
		int passo = 0;

		ncorfront = (int*) malloc((md->ncores + 1) * sizeof(int));

		fronteira_mapa(md, f);

		while (f->tamanho > 0) {
			for (int i = 1; i <= md->ncores; i++)
				ncorfront[i] = 0;
			for (int i = 0; i < f->tamanho; i++)
				ncorfront[md->mapa[f->pos[i].l][f->pos[i].c]]++;
			ncor = 0;
			for (int i = 1; i <= md->ncores; i++) {

				if (pop[son].prob[passo][i - 1] == 0)
				{
					pop[son].prob[passo][i - 1] = ((ncorfront[i] * k + (float)(rand() % 100 + 1)) / 1000);
				}

				probe = pop[son].prob[passo][i - 1];

				if (ncorfront[i] == 0) {
					pop[son].prob[passo][i - 1] = 0;
					probe = 0;
				}

				if (probe > mprob) {
					mprob = probe;
					pop[son].cor[passo] = i;
				}

				if (ncorfront[i] > ncor) {
					ncor = ncorfront[i];
				}
			}
			pinta_mapa(md, pop[son].cor[passo]);
			passo++;
			pop[son].passos = passo;
			mprob = 0;
			fronteira_mapa(md, f);
		}
	}
	// printf("RESULTADOS: \n"); //imprime uma amostra da população
	// int c = 0;
	// while (c < 30) {
	// 	printf("%d\n", pop[c].passos);
	// 	c++;
	// }

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

tplano genetic_algorithm(tmapa mo) {
	int i;
	int j;
	int aux_cor;
	int ncor;
	int generation;
	int g_inc;
	int *ncorfront;
	// tmapa mo;
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
	int t_busca;
	tam_pop = 1000;
	tplano ini_pop[tam_pop];

	probe = 0;
	mprob = 0;
	generation = 0;

//-----------------------//ALGUNS PARÂMETOR DE AJUSTE//-----------------------//
	k = 16; // ponderação na probabilidade
	d = 500; //escala da probabilidade
	lambda = 0.1; //diversificador de filhos
	g_inc = 1; //passo do incremento do diversificador
	inc = 0.2; // fator de incremento do diversificador
	time_t seconds = 60; // timer para teste (iniciando depois que a pop_ini é gerada)
	int flag = 1; //0 -> desabilita a recombinação e a perturbação
	t_busca = 20; //numero de indivíduos da busca local


	endwait = start + seconds;

	// printf("insira o tabuleiro: \n");
	// carrega_mapa(&mo);

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
				ncorfront[i] = 0;
			for (i = 0; i < f->tamanho; i++)
				ncorfront[md->mapa[f->pos[i].l][f->pos[i].c]]++;
			ncor = 0;
			for (i = 1; i <= md->ncores; i++) {

				probe = ((ncorfront[i] * k + (float)(rand() % 40 + 1)) / d);

				if (ncorfront[i] == 0)
					probe = 0;

				p->prob[p->passos][i - 1] = probe;

				if (probe > mprob) {
					mprob = probe;

					aux_cor = i;
				}

				if (ncorfront[i] > ncor) {
					ncor = ncorfront[i];
				}

			}
			pinta_mapa(md, aux_cor);
			insere_plano(p, aux_cor);
			mprob = 0;
			fronteira_mapa(md, f);
		}

		ini_pop[j] = *p; //insere um individuo dentro da população
		if (j == 0) {
			copia_plano(p, m_p);
		}
	}

//-----------------------//ORDENAR POPULAÇÃO//-----------------------//
	selectionSort(ini_pop, tam_pop);

//-----------------------//PROCEDIMENTO EVOLUTIVO//-----------------------//

	// Progress bar
	// int max = 60;
	long int max  = (long)seconds;

	// progressbar *smooth = progressbar_new("\033cSuave", max);
	// for (int i = 0; i < max; i++)
	// {
	// 	usleep(SLEEP_US);
	// 	progressbar_inc(smooth);
	// }
	// progressbar_finish(smooth);



	progressbar *smooth = progressbar_new("\033cSolving", max); //create progressbar

	double diff_t = 0;
	double old_dif = 60;
	double actual_diff = 0;
	while (start < endwait) //função timer para englobar
	{
		start = time(NULL);

		diff_t = difftime(endwait, start); //calculates how many seconds did passed to increment progress bar
		actual_diff = old_dif - diff_t;
		// // printf("loop time is : %s", ctime(&start));
		// printf("Execution time = %f\n", old_dif);
		// printf("Execution time = %f\n", diff_t);
		// printf("Execution time = %f\n", actual_diff);
		old_dif = diff_t; 

		for (int i = 0; i < actual_diff; i++)
		{
			progressbar_inc(smooth); //increase progrees in progressbar
		}



		if (md->nlinhas > 49) {
			flag = 0;
			t_busca = 5;
		}

		if (flag == 1) //flag para instâncias menores
		{
			if (generation == 0)
			{
				for (int i = 0; i < 6; ++i)
				{
					local_search(ini_pop, mo, t_busca);
				}
			}

			recombination(mo, ini_pop, k, lambda, md->ncores); //COMENTANDO ESSA LINHA FUNCIONA COMO UM ILS

			generation++;

			// printf("GERAÇÃO: %d\n", generation);

			if (generation % g_inc == 0)
			{
				if (lambda > 1)
				{
					lambda = 0;
				}
				else {
					perturbation(ini_pop, md->ncores);
					for (int i = 0; i < 6; ++i)
					{
						local_search(ini_pop, mo, t_busca);
					}
					lambda = lambda + inc;
				}
			}

			selectionSort(ini_pop, 52);
		}


		else { //para instâncias onde o tempo do cruzamento e mutação e região da busca não compensam

			if (md->nlinhas > 99)
				t_busca = 1;
			local_search(ini_pop, mo, t_busca);
			generation++;
			selectionSort(ini_pop, 52);
		}

		printf("\033c");

	}
	progressbar_finish(smooth); //finish progressbar



//-----------------------//APRESENTA O MELHOR RESULTADO//-----------------------//
	// printf("\033c");

	// printf("%d\n", ini_pop[0].passos);
	// for (i = 0; i < ini_pop[0].passos; i++)
	// 	printf("%d ", ini_pop[0].cor[i]);
	// printf("\n");

	// printf("start time is : %s", ctime(&start));
	// printf("end time is %s", ctime(&endwait));

	// printf("MELHOR RESULTADO: \n");
	// int b = 0;
	// while (b < 50) {
	// 	printf("%d\n", ini_pop[b].passos);
	// 	// for (i = 0; i < ini_pop[b].passos; i++)
	// 	// 	printf("%d ", ini_pop[b].cor[i]);
	// 	// printf("\n");
	// 	b++;
	// }




	return ini_pop[0];
}

// int main(){
// 	return genetic_algorithm();
// }

