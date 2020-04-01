#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"
#include <unistd.h>
/* compile with gcc -lncurses file.c */

#include <string.h>
#include <curses.h>
#include <menu.h>

/* progressbar */
#include "include/progressbar/progressbar.h"
#include "include/progressbar/statusbar.h"

/*genetic algorithm*/
#include "floodit_ga.c"

#define SLEEP_US 100000

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

char *choices[] = {
	"Play",
	"How to Play",
	"Peep how GA play",
	// "Play against",
	"Options",
	"Exit",
	(char *)NULL,
};
void print_menu();
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void play_game();
void ga_play();

int main()
{
	print_menu();
}

void print_menu()
{
	initscr();
	curs_set(0);
	// int row,col;
	// getmaxyx(stdscr,row,col);
	ITEM **my_items;
	int c;
	MENU *my_menu;
	WINDOW *my_menu_win;
	WINDOW *my_menu_win2;
	int n_choices, i;

	/* Initialize curses */
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

	/* Initialize items */
	n_choices = ARRAY_SIZE(choices);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for (i = 0; i < n_choices; ++i)
		my_items[i] = new_item(choices[i], "");
	my_items[n_choices] = (ITEM *)NULL;
	item_opts_off(my_items[1], O_SELECTABLE);
	// item_opts_off(my_items[2], O_SELECTABLE);
	item_opts_off(my_items[3], O_SELECTABLE);

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set fore ground and back ground of the menu */
	set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE);
	set_menu_back(my_menu, COLOR_PAIR(2));
	set_menu_grey(my_menu, COLOR_PAIR(3));

	/* Create the window to be associated with the menu */
	my_menu_win = newwin(10, 40, 4, 4);
	my_menu_win2 = newwin(3, 40, 15, 4);
	keypad(my_menu_win, TRUE);
	keypad(my_menu_win2, TRUE);

	/* Set main window and sub window */
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 5, 20, 3, 10));

	/* Set menu mark to the string " * " */
	set_menu_mark(my_menu, " * ");

	/* Print a border around the main window and print a title */
	box(my_menu_win, 0, 0);
	box(my_menu_win2, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "Flood-it", COLOR_PAIR(1));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	refresh();

	/* Post the menu */
	mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
	mvprintw(LINES - 2, 0, "Up and Down arrow keys to naviage (F2 to Exit)");
	post_menu(my_menu);
	wrefresh(my_menu_win);
	wrefresh(my_menu_win2);
	refresh();

	while ((c = wgetch(my_menu_win)) != KEY_F(2))
	{
		switch (c)
		{
		case KEY_DOWN:
			menu_driver(my_menu, REQ_DOWN_ITEM);
			break;
		case KEY_UP:
			menu_driver(my_menu, REQ_UP_ITEM);
			break;
		case 10: /* Enter */
			wmove(my_menu_win2, 1, 1);
			wclrtoeol(my_menu_win2);
			mvwprintw(my_menu_win2, 1, 1, "Item selected is : %s",
					  item_name(current_item(my_menu)));
			switch (item_index(current_item(my_menu)))
			{
			case 0:
				werase(my_menu_win2); //clear windoow2 output before going to play
				refresh();
				endwin();

				play_game();

				break;
			case 2:
				werase(my_menu_win2); //clear windoow2 output before going to play
				refresh();
				endwin();

				ga_play();

				break;
			default:
				break;
			}
		}
		/*Reprinting some items*/
		mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
		mvprintw(LINES - 2, 0, "Up and Down arrow keys to naviage (F2 to Exit)");
		box(my_menu_win, 0, 0);
		box(my_menu_win2, 0, 0);
		refresh();
		pos_menu_cursor(my_menu);
		wrefresh(my_menu_win);
		wrefresh(my_menu_win2);
	}

	/* Unpost and free all the memory taken up */
	unpost_menu(my_menu);
	for (i = 0; i < n_choices; ++i)
		free_item(my_items[i]);
	free_menu(my_menu);
	endwin();
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
	int length, x, y;
	float temp;

	if (win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if (startx != 0)
		x = startx;
	if (starty != 0)
		y = starty;
	if (width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length) / 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}

void play_game()
{
	int cor;
	tmapa m;
	int semente;
	// WINDOW *my_menu_win3;

	printf("\033c");

	// my_menu_win3 = newwin(10, 40, 4, 4);
	// box(my_menu_win3, 0, 0);
	// wrefresh(my_menu_win3);
	// refresh();

	printf("enter the number of lines: ");
	scanf("%i", &m.nlinhas);

	printf("enter the number of columns: ");
	scanf("%i", &m.ncolunas);

	printf("enter the number of colors: ");
	scanf("%i", &m.ncores);

	// if (argc == 5)
	// 	semente = atoi(argv[4]);
	// else
	semente = 0;
	gera_mapa(&m, semente);

	cor = m.mapa[0][0];

	while (cor != -1)
	{
		printf("\033c");

		printf("type -1 to exit\n\n");

		pinta_mapa(&m, cor);
		mostra_mapa_cor(&m);
		scanf("%d", &cor);
	}
	system("clear");

	// return 0;
}

void ga_play()
{
	// Progress bar
	int max = 20;
	progressbar *smooth = progressbar_new("\033cSuave", max);
	for (int i = 0; i < max; i++)
	{
		usleep(SLEEP_US);
		progressbar_inc(smooth);
	}
	progressbar_finish(smooth);

	genetic_algorithm();

	int i;
	tmapa m;
	tplano *p;
	int intervalo;
	char ch;

	// if (argc > 1)
	// 	intervalo = atoi(argv[1]);
	// else
	intervalo = 100;

	carrega_mapa(&m);
	p = aloca_plano(&m);
	carrega_plano(p);

	printf("\033c");
	mostra_mapa_cor(&m);
	for (i = 0; i < p->passos; i++)
	{
		usleep(1000 * intervalo);
		pinta_mapa(&m, p->cor[i]);
		printf("\033c");
		mostra_mapa_cor(&m);
	}

	int c;
	while ((c = getchar()) != '\n' && c != EOF){} //clear the imput buffer
	getchar();

	system("clear");
	// return 0;
}