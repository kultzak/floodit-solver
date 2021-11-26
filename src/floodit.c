#include <curses.h>
#include <menu.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/mapa.h"

/*genetic algorithm*/
#include "floodit_ga.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

void print_footer();

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
void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color);
void play_game();
void ga_play();

int main() { print_menu(); }

void print_menu() {
  // int row,col;
  // getmaxyx(stdscr,row,col);
  ITEM **my_items;  // menu items
  int c;
  MENU *my_menu;
  WINDOW *my_menu_win;
  WINDOW *my_menu_win2;
  int n_choices, i;

  /* Initialize curses */
  initscr();      // first routine when initializing a program
  start_color();  // makes possible to use colors in the courses windows
  curs_set(0);    // hides de cursor
  cbreak();  // terminal mode btween raw mode and cooked mode makes user input
             // imediately available
  noecho();  // disables echoing
  // keypad(stdscr, TRUE); // enables the use of function keys, here is used for
  // each windows more ahead
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

  /* Initialize items */
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for (i = 0; i < n_choices; ++i) my_items[i] = new_item(choices[i], "");
  my_items[n_choices] = (ITEM *)NULL;
  item_opts_off(my_items[1],
                O_SELECTABLE);  // turns off the named options for item; no
                                // other option is changed.
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
  my_menu_win2 = newwin(3, 40, 28, 8);
  keypad(my_menu_win, TRUE);  // enables the use of function keys
  // keypad(my_menu_win2, TRUE);

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
  print_footer();
  post_menu(my_menu);
  wrefresh(my_menu_win);
  wrefresh(my_menu_win2);
  refresh();

  while ((c = wgetch(my_menu_win)) != KEY_F(2)) {
    switch (c) {
      case KEY_DOWN:
        menu_driver(my_menu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(my_menu, REQ_UP_ITEM);
        break;
      case 10: /* Enter */
        wmove(my_menu_win2, 1, 1);
        wclrtoeol(my_menu_win2);
        mvwprintw(my_menu_win2, 1, 1, "%s is disabled",
                  item_name(current_item(my_menu)));
        switch (item_index(current_item(my_menu))) {
          case 0:
            werase(my_menu_win2);  // clear windoow2 output before going to play
            refresh();
            endwin();

            play_game();

            break;
          case 2:
            werase(my_menu_win2);  // clear windoow2 output before going to play
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
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
    refresh();
    pos_menu_cursor(my_menu);
    wrefresh(my_menu_win);
    wrefresh(my_menu_win2);
  }

  /* Unpost and free all the memory taken up */
  unpost_menu(my_menu);
  for (i = 0; i < n_choices; ++i) free_item(my_items[i]);
  free_menu(my_menu);
  endwin();
}

void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color) {
  int length, x, y;
  float temp;

  if (win == NULL) win = stdscr;
  getyx(win, y, x);
  if (startx != 0) x = startx;
  if (starty != 0) y = starty;
  if (width == 0) width = 80;

  length = strlen(string);
  temp = (width - length) / 2;
  x = startx + (int)temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}

void print_footer() {
  mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
  mvprintw(LINES - 2, 0, "Up and Down arrow keys to naviage (F2 to Exit)");
}

void play_game() {
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

  while (cor != -1) {
    printf("\033c");

    printf("type -1 to exit\n\n");

    pinta_mapa(&m, cor);
    mostra_mapa_cor(&m);
    scanf("%d", &cor);
  }
  system("clear");

  // return 0;
}

void ga_play() {
  int i;
  tmapa m;
  tplano *plan;
  int intervalo;

  printf("Insert the map!");

  carrega_mapa(&m);
  plan = aloca_plano(&m);
  *plan = genetic_algorithm(m);

  intervalo = 100;

  int c;
  // while ((c = getchar()) != '\n' && c != EOF){} //clear the imput buffer to
  // see last output getchar();

  printf("\033c");
  mostra_mapa_cor(&m);
  for (i = 0; i < plan->passos; i++) {
    usleep(1000 * intervalo);
    pinta_mapa(&m, plan->cor[i]);
    printf("\033c");
    mostra_mapa_cor(&m);
  }

  mostra_plano(plan);

  while ((c = getchar()) != '\n' && c != EOF) {
  }  // clear the imput buffer
  getchar();

  while ((c = getchar()) != '\n' && c != EOF) {
  }  // clear the input buffer after getchar. it avoids errors from last getchar

  system("clear");
  // return 0;
}