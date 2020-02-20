#include <stdlib.h>
#include <stdio.h>
// #include "mapa.h"
#include <unistd.h>
/* compile with gcc -lncurses file.c */

#include <string.h>
#include <curses.h>
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4



char *choices[] = {
	"Play",
	"Peep how AG play",
	"Peep how ...",
	"Play against",
	"Exit",
	(char *)NULL,
};
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int main()
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
	item_opts_off(my_items[2], O_SELECTABLE);
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
			box(my_menu_win2, 0, 0);
			refresh();
			pos_menu_cursor(my_menu);
			break;
		}
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

// int main(int argc, char **argv) {
//   int cor;
//   tmapa m;
//   int semente;

//   if(argc < 4 || argc > 5) {
//     printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>]\n", argv[0]);
//     exit(1);
//   }

// int main (void)

// {
//         int c = 0;
//         /* Init ncurses mode */
//         initscr ();
//         /* Hide cursor */
//         curs_set (0);
//         while (c < 20) {
//                 /* Print at row 0, col 0 */
//                 mvprintw (0, 0, "%d", c++);
//                 refresh ();
//                 sleep (10);
//         }
//         /* End ncurses mode */
//         endwin();
//         return 0;
// }

// m.nlinhas = atoi(argv[1]);
// m.ncolunas = atoi(argv[2]);
// m.ncores = atoi(argv[3]);

// if(argc == 5)
//   semente = atoi(argv[4]);
// else
//   semente = 0;
// gera_mapa(&m, semente);

// cor = m.mapa[0][0];

// while(cor != -1) {
//   pinta_mapa(&m, cor);
//   mostra_mapa_cor(&m);
//   scanf("%d", &cor);
// }
// return 0;
// }
