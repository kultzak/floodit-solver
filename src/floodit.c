#include <curses.h>
#include <menu.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "lib/mapa.h"

/*genetic algorithm*/
#include "floodit_ga.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

int termx, termy;
WINDOW *my_menu_win, *my_menu_win2;
ITEM **my_items;  // menu items
MENU *my_menu;
int c;
int n_choices, i;

char *main_menu_choices[] = {
    "Play as human",
    "Solve as non-human",
    "How to Play",
    // "Play against",
    "Options",
    "Exit",
    (char *)NULL,
};
char *play_menu_choices[] = {
    "Generate new map",
    "Load map",
    "Back",
    (char *)NULL,
};
char *solvers_menu_choices[] = {
    "Genetic algorithm",
    "Back",
    (char *)NULL,
};

void terminal_start();
void terminal_stop();
void initialize_menu();
void initialize_menu2();
void initialize_menu3();
void initialize_menu4();
void print_main_menu();
void print_play_menu();
void print_solvers_menu();
const char * print_loadmap_menu();
void print_footer();
void print_menu_title(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color);
void control_main_menu();
void control_play_menu();
void control_solvers_menu();
const char * control_mapselect_menu();
void unpost_n_free_menu();
void play_game();
void config_generate_map();
void config_generate_map_n_play();
void load_map_n_play();
void ga_play();
void get_window_dimensions();
void resizehandler(int);
static int parse_ext(const struct dirent *dir);

void terminal_start() {
  /* Initialize curses */
  initscr();  // first routine when initializing a ncurses program
  // mvprintw(LINES - 1, col/2, "Up and Down arrow keys to navigate (F2 to
  // Exit)");
  start_color();  // makes possible to use colors in the courses windows
  curs_set(0);    // hides de cursor
  cbreak();  // terminal mode between raw mode and cooked mode makes user input
             // immediately available
  noecho();  // disables echoing
  keypad(stdscr, TRUE);  // enables the use of function keys, here is used for
  // each windows more ahead
  refresh();
}

void terminal_stop() { endwin(); }

void get_window_dimensions() { getmaxyx(stdscr, termy, termx); }

void initialize_menu() {
  /* initialize color used in menus*/
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

  /* Initialize items */
  n_choices = ARRAY_SIZE(main_menu_choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for (i = 0; i < n_choices; ++i) my_items[i] = new_item(main_menu_choices[i], "");
  my_items[n_choices] = (ITEM *)NULL;
  item_opts_off(my_items[2], O_SELECTABLE);  // turns off the named options for item; no other option is changed.
  item_opts_off(my_items[3], O_SELECTABLE);

  /* Crate menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Set fore ground and back ground of the menu */
  set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE);
  set_menu_back(my_menu, COLOR_PAIR(2));
  set_menu_grey(my_menu, COLOR_PAIR(3));
  set_menu_mark(my_menu, " > ");
}

void initialize_menu2() {
  /* initialize color used in menus*/
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

  /* Initialize items */
  n_choices = ARRAY_SIZE(play_menu_choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for (i = 0; i < n_choices; ++i) my_items[i] = new_item(play_menu_choices[i], "");
  my_items[n_choices] = (ITEM *)NULL;
  // item_opts_off(my_items[1], O_SELECTABLE);  // turns off the named options for item; no other option is changed.

  /* Crate menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Set fore ground and back ground of the menu */
  set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE);
  set_menu_back(my_menu, COLOR_PAIR(2));
  set_menu_grey(my_menu, COLOR_PAIR(3));
  set_menu_mark(my_menu, " + ");
}

void initialize_menu4() {
  /* initialize color used in menus*/
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

  /* Initialize items */
  n_choices = ARRAY_SIZE(solvers_menu_choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for (i = 0; i < n_choices; ++i) my_items[i] = new_item(solvers_menu_choices[i], "");
  my_items[n_choices] = (ITEM *)NULL;
  // item_opts_off(my_items[1], O_SELECTABLE);  // turns off the named options for item; no other option is changed.

  /* Crate menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Set fore ground and back ground of the menu */
  set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE);
  set_menu_back(my_menu, COLOR_PAIR(2));
  set_menu_grey(my_menu, COLOR_PAIR(3));
  set_menu_mark(my_menu, " + ");
}

void initialize_menu3() {
  /* initialize color used in menus*/
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);	

	struct dirent **namelist; //dir entry struct

  n_choices = scandir(".", &namelist, parse_ext, alphasort);
  if (n_choices < 0) {
      perror("scandir");
  }
  
  /* Create items */
  if(n_choices < 900){ //to avoid calloc below exceeding maximum object size
    my_items = (ITEM **)calloc(n_choices+1, sizeof(ITEM *));
  }
  for(i = 0; i < n_choices; ++i)
          my_items[i] = new_item(namelist[i]->d_name,"");

  my_items[n_choices] = (ITEM *)NULL; //for some reason this null
  free(namelist);
  // item_opts_off(my_items[0], O_SELECTABLE);  // turns off the named options for item; no other option is changed.

  
	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

  /* Set fore ground and back ground of the menu */
  set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE);
  set_menu_back(my_menu, COLOR_PAIR(2));
  set_menu_grey(my_menu, COLOR_PAIR(3));
  set_menu_mark(my_menu, " * ");
}

void control_main_menu() {
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
            terminal_stop();

            print_play_menu();
            exit(0);

            break;
          case 1:
            werase(my_menu_win2);  // clear windoow2 output before going to play
            refresh();
            terminal_stop();

            print_solvers_menu();
            exit(0);

            break;

            // werase(my_menu_win2);  // clear windoow2 output before going to play
            // refresh();
            // terminal_stop();

            // print_solvers_menu();
            // ga_play();

            // break;
          default:
            break;
        }
    }
    /*Reprinting some items*/
    print_footer();
    // box(my_menu_win, 0, 0);
    box(my_menu_win2, 0, 0);
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
    // pos_menu_cursor(my_menu);
    wrefresh(my_menu_win);
    wrefresh(my_menu_win2);
    refresh();
  }
}

const char * control_solvers_play_menu(tmapa * m) {
  while ((c = wgetch(my_menu_win))) {
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
          case 0: //new map
            werase(my_menu_win2);  // clear windoow2 output before going to play
            refresh();
            terminal_stop();

            config_generate_map(m);
            return "GERANDO UM NOVO MAPA";

            break;
          case 1: //load map
            werase(my_menu_win2);  // clear windoow2 output before going to play
            refresh();
            terminal_stop();

            // load_map_n_play();
            // print_main_menu();
            // exit(0);

            return "CARREGANDO UM MAPA JÁ EXISTENTE";


            break;
          case 2: //back
            werase(my_menu_win2);  // clear windoow2 output before going to play
            refresh();
            terminal_stop();

            system("clear");
            print_main_menu();
            exit(0);

            break;
          default:
            break;
        }
    /*Reprinting some items*/
    print_footer();
    // box(my_menu_win, 0, 0);
    box(my_menu_win2, 0, 0);
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
    // pos_menu_cursor(my_menu);
    wrefresh(my_menu_win);
    wrefresh(my_menu_win2);
    refresh();
    }
  }
}

void control_play_menu() {
  while ((c = wgetch(my_menu_win))) {
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
          case 0: //new map
            werase(my_menu_win2);  // clear windoow2 output before going to play
            refresh();
            terminal_stop();

            config_generate_map_n_play();
            print_main_menu();
            exit(0);

            break;
          case 1: //load map
            werase(my_menu_win2);  // clear windoow2 output before going to play
            refresh();
            terminal_stop();

            load_map_n_play();
            print_main_menu();
            exit(0);


            break;
          case 2: //back
            werase(my_menu_win2);  // clear windoow2 output before going to play
            refresh();
            terminal_stop();

            system("clear");
            print_main_menu();
            exit(0);

            break;
          default:
            break;
        }
    /*Reprinting some items*/
    print_footer();
    // box(my_menu_win, 0, 0);
    box(my_menu_win2, 0, 0);
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
    // pos_menu_cursor(my_menu);
    wrefresh(my_menu_win);
    wrefresh(my_menu_win2);
    refresh();
    }
  }
}

void control_solvers_menu() {
  while ((c = wgetch(my_menu_win))) {
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
          case 0: //new map
            werase(my_menu_win2);  // clear windoow2 output before going to play
            refresh();
            terminal_stop();

            system("clear");
            ga_play();
            exit(0);

            break;
          case 1: //back
            werase(my_menu_win2);  // clear windoow2 output before going to play
            refresh();
            terminal_stop();

            system("clear");
            print_main_menu();
            exit(0);

            break;
          default:
            break;
        }
    /*Reprinting some items*/
    print_footer();
    // box(my_menu_win, 0, 0);
    box(my_menu_win2, 0, 0);
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
    // pos_menu_cursor(my_menu);
    wrefresh(my_menu_win);
    wrefresh(my_menu_win2);
    refresh();
    }
  }
}

const char * control_mapselect_menu(){
  while((c = wgetch(my_menu_win))) {
    switch(c) {	
      case KEY_DOWN:
        menu_driver(my_menu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(my_menu, REQ_UP_ITEM);
        break;
      case KEY_NPAGE:
        menu_driver(my_menu, REQ_SCR_DPAGE);
        break;
      case KEY_PPAGE:
        menu_driver(my_menu, REQ_SCR_UPAGE);
        break;
      case KEY_F(2): //Back
        werase(my_menu_win2);  // clear windoow2 output before going to play
        refresh();
        terminal_stop();

        // system("clear");
        print_play_menu();
        exit(0);

        break;
      case 10: /* Enter */
        wmove(my_menu_win2, 1, 1);
        wclrtoeol(my_menu_win2);
        mvwprintw(my_menu_win2, 1, 1, "Loading map %s", item_name(current_item(my_menu)));
        werase(my_menu_win2);  // clear windoow2 output before going to play
        refresh();
        terminal_stop();

        printf("\033c");

        return item_name(current_item(my_menu));

        // break;
        goto exit_loop; //uggly, you are a bad person

    }
    // break;
    /*Reprinting some items*/
    print_footer();
    // box(my_menu_win, 0, 0);
    box(my_menu_win2, 0, 0);
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
    // pos_menu_cursor(my_menu);
    wrefresh(my_menu_win);
    wrefresh(my_menu_win2);
    refresh();
	}
  exit_loop: ;
}

void unpost_n_free_menu(){
  unpost_menu(my_menu);
  free_menu(my_menu);
  for(i = 0; i < n_choices; ++i)
          free_item(my_items[i]);
	endwin();
}

void resizehandler(int sig) {
  terminal_stop();
  terminal_start();
  clear();

  get_window_dimensions();

  //FIXME: create a routine to resize terminal to an area smaller than menu size
  /* Create the window to be associated with the menu or move the existent according to desired strategy*/
  /* recreating the window  clears the current menu state however avoid bug when
  resizing terminal to an area smaller than the menu */
  // my_menu_win = newwin(10, 40, 4, (termx / 2) - 20);
  // my_menu_win2 = newwin(3, 40, 15, (termx / 2)- 20);
  mvwin(my_menu_win, 4, (termx / 2) - 20);
  mvwin(my_menu_win2, 15, (termx / 2) - 20);
  keypad(my_menu_win, TRUE);  // enables the use of function keys

  /* Set main window and sub window */
  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 5, 20, 3, 10));

  /* Print a border around the main window and print a title */
  box(my_menu_win, 0, 0);
  box(my_menu_win2, 0, 0);
  print_menu_title(my_menu_win, 1, 0, 40, "Flood-it", COLOR_PAIR(1)); //TODO: declare menu title globally to be correclty resized
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 2, 39, ACS_RTEE);

  print_footer();
  post_menu(my_menu);
  wrefresh(my_menu_win);
  wrefresh(my_menu_win2);
  refresh();
}

int main() {  terminal_start(); print_main_menu(); }

void print_main_menu() {

  get_window_dimensions();
  initialize_menu();

 //FIXME: resize properly when in the game window
  signal(SIGWINCH, resizehandler); //executes the resizehandler function at each resize signal

  /* Create the window to be associated with the menu */
  my_menu_win = newwin(10, 40, 4, (termx / 2) - 20);
  my_menu_win2 = newwin(3, 40, 15, (termx / 2) - 20);
  keypad(my_menu_win, TRUE);  // enables the use of function keys

  /* Set main window and sub window */
  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 5, 20, 3, 10));

  /* Print a border around the main window and print a title */
  box(my_menu_win, 0, 0);
  box(my_menu_win2, 0, 0);
  print_menu_title(my_menu_win, 1, 0, 40, "Flood-it", COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 2, 39, ACS_RTEE);

  /* Post the menu */
  print_footer();
  post_menu(my_menu);
  wrefresh(my_menu_win);
  wrefresh(my_menu_win2);
  refresh();

  //lasso that controls every menu operation event
  control_main_menu();
  printf("BYE BYE");
  getchar();
  getchar();

	/* Unpost and free all the memory taken up */
  unpost_n_free_menu();
}

void print_play_menu() {
  // terminal_start();
  get_window_dimensions();
  initialize_menu2();

 //FIXME: resize properly when in the game window
  signal(SIGWINCH, resizehandler); //executes the resizehandler function at each resize signal

  /* Create the window to be associated with the menu */
  my_menu_win = newwin(10, 40, 4, (termx / 2) - 20);
  my_menu_win2 = newwin(3, 40, 15, (termx / 2) - 20);
  keypad(my_menu_win, TRUE);  // enables the use of function keys

  /* Set main window and sub window */
  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 5, 20, 3, 10));

  /* Print a border around the main window and print a title */
  box(my_menu_win, 0, 0);
  box(my_menu_win2, 0, 0);
  print_menu_title(my_menu_win, 1, 0, 40, "Flood-itão", COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 2, 39, ACS_RTEE);

  /* Post the menu */
  print_footer();
  post_menu(my_menu);
  wrefresh(my_menu_win);
  wrefresh(my_menu_win2);
  refresh();

  //lasso that controls every menu operation event
  control_play_menu();

	/* Unpost and free all the memory taken up */
  // unpost_n_free_menu(); //free just 
}

const char * print_solvers_play_menu(char * alg_name, tmapa * m) {
  // terminal_start();
  get_window_dimensions();
  initialize_menu2();

 //FIXME: resize properly when in the game window
  signal(SIGWINCH, resizehandler); //executes the resizehandler function at each resize signal

  /* Create the window to be associated with the menu */
  my_menu_win = newwin(10, 40, 4, (termx / 2) - 20);
  my_menu_win2 = newwin(3, 40, 15, (termx / 2) - 20);
  keypad(my_menu_win, TRUE);  // enables the use of function keys

  /* Set main window and sub window */
  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 5, 20, 3, 10));

  /* Print a border around the main window and print a title */
  box(my_menu_win, 0, 0);
  box(my_menu_win2, 0, 0);
  print_menu_title(my_menu_win, 1, 0, 40, "                  ", COLOR_PAIR(1));
  print_menu_title(my_menu_win, 1, 0, 40, alg_name, COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 2, 39, ACS_RTEE);

  /* Post the menu */
  print_footer();
  post_menu(my_menu);
  wrefresh(my_menu_win);
  wrefresh(my_menu_win2);
  refresh();

  //lasso that controls every menu operation event
  return control_solvers_play_menu(m);

	/* Unpost and free all the memory taken up */
  // unpost_n_free_menu(); //free just 
}

void print_solvers_menu(){
   // terminal_start();
  get_window_dimensions();
  initialize_menu4();

 //FIXME: resize properly when in the game window
  signal(SIGWINCH, resizehandler); //executes the resizehandler function at each resize signal

  /* Create the window to be associated with the menu */
  my_menu_win = newwin(10, 40, 4, (termx / 2) - 20);
  my_menu_win2 = newwin(3, 40, 15, (termx / 2) - 20);
  keypad(my_menu_win, TRUE);  // enables the use of function keys

  /* Set main window and sub window */
  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 5, 20, 3, 10));

  /* Print a border around the main window and print a title */
  box(my_menu_win, 0, 0);
  box(my_menu_win2, 0, 0);
  print_menu_title(my_menu_win, 1, 0, 40, "Flood-it Solvers", COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 2, 39, ACS_RTEE);

  /* Post the menu */
  print_footer();
  post_menu(my_menu);
  wrefresh(my_menu_win);
  wrefresh(my_menu_win2);
  refresh();

  //lasso that controls every menu operation event
  control_solvers_menu();

	/* Unpost and free all the memory taken up */
  // unpost_n_free_menu(); //free just 

}

const char * print_loadmap_menu(){
  // terminal_start();
  get_window_dimensions();
  initialize_menu3();

   //FIXME: resize properly when in the game window
  signal(SIGWINCH, resizehandler); //executes the resizehandler function at each resize signal

  /* Create the window to be associated with the menu */
  my_menu_win = newwin(10, 40, 4, (termx / 2) - 20);
  my_menu_win2 = newwin(3, 40, 15, (termx / 2) - 20);
  keypad(my_menu_win, TRUE);  // enables the use of function keys

	/* Set main window and sub window */
  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1)); //menu position inside window
	set_menu_format(my_menu, 5, 1); //make the menu scrollable

  /* Print a border around the main window and print a title */
  box(my_menu_win, 0, 0);
  box(my_menu_win2, 0, 0);
  print_menu_title(my_menu_win, 1, 0, 40, "Flood-it", COLOR_PAIR(1));
  mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
        
	/* Post the menu */
	// attron(COLOR_PAIR(2)); 
  print_footer();
	// mvprintw(LINES - 1, 0, "Use PageUp and PageDown to scoll down or up a page of items"); //commented to standardize footer
	// attroff(COLOR_PAIR(2));
	post_menu(my_menu);
	wrefresh(my_menu_win);
  wrefresh(my_menu_win2);
	refresh();

                              //lasso that controls every menu operation event
  const char * selected_map = control_mapselect_menu();

	/* Unpost and free all the memory taken up */
  // unpost_n_free_menu();
  return selected_map;
}

void print_menu_title(WINDOW *win, int starty, int startx, int width,
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
  mvprintw(LINES - 3, (termx / 2) - 20,
           "Press <ENTER> to see the option selected");
  mvprintw(LINES - 2, (termx / 2) - 20,
           "Up and Down arrow keys to navigate (F2 to Exit)");
}

//TODO: give option to save map and result to file
void play_game(tmapa *m) {
  int cor;

  printf("\033c");

  cor = m->mapa[0][0];

  int *ptro=NULL;
  int len=2;

  //allocates initially 
  ptro=(int*)malloc(len*sizeof(int));

  int array_pos = 0;
  while (cor != -1) {
    printf("\033c");

    printf("type -1 to exit\n\n");
    int loop;

    printf("Previous Moves: "); //TODO: store only previous valid moves
    for(loop = 0; loop < array_pos; loop++)
      printf("%d ", ptro[loop]);

    printf("\n");

    //TODO: treatment to avoid unavailable color printing
    pinta_mapa(m, cor);
    mostra_mapa_cor(m);
    printf("COR: ");
    scanf("%d", &cor);

    //print moves until map is flooded
    tfronteira *f;
    f = aloca_fronteira(m);
    fronteira_mapa(m, f);
    if(f->tamanho){
      array_pos++;
      ptro = realloc(ptro, array_pos * sizeof(int));
      ptro[array_pos-1] = cor;
    }

    //get if is the last move
    pinta_mapa(m, cor);
    f = aloca_fronteira(m);
    fronteira_mapa(m, f);
    if(f->tamanho==0){
        printf("\033c");

        printf("type -1 to exit\n\n");
        int loop;

        printf("Previous Moves: ");
        for(loop = 0; loop < array_pos; loop++)
          printf("%d ", ptro[loop]);

        printf("\n");

        pinta_mapa(m, cor);
        mostra_mapa_cor(m);
        printf("COR: ");
        salva_plano();
        getchar();
        cor = -1;
    }
  }
  system("clear");
  free(ptro);

}

void config_generate_map_n_play(){
  int semente;
  tmapa m;
  printf("\033c");

  printf("enter the number of lines: ");
  scanf("%i", &m.nlinhas);

  printf("enter the number of columns: ");
  scanf("%i", &m.ncolunas);

  //this scanf lets a trailing line break
  printf("enter the number of colors: ");
  scanf("%i", &m.ncores);

  semente = 0;
  gera_mapa(&m, semente);
  mostra_mapa_cor(&m);
  salva_mapa(&m);
  getchar(); //just to wait showing the save path
  play_game(&m);
}

void config_generate_map(tmapa *m){
  int semente;

  printf("\033c");

  printf("enter the number of lines: ");
  scanf("%i", &m->nlinhas);

  printf("enter the number of columns: ");
  scanf("%i", &m->ncolunas);

  //this scanf lets a trailing line break
  printf("enter the number of colors: ");
  scanf("%i", &m->ncores);

  semente = 0;
  gera_mapa(m, semente);  
  mostra_mapa_cor(m);
  salva_mapa(m);
  getchar(); //just to wait showing the save path

}

/* when return 1, scandir will put this dirent to the list */
static int parse_ext(const struct dirent *dir)
{
  if(!dir)
    return 0;

  if(dir->d_type == DT_REG) { /* only deal with regular file */
      const char *ext = strrchr(dir->d_name,'.');
      if((!ext) || (ext == dir->d_name))
        return 0;
      else {
        if(strcmp(ext, ".fldmap") == 0)
          return 1;
      }
  }

  return 0;
}

void load_map_n_play(){
  tmapa m;
  // printf("\033c"); //hei this is aparently unnecessary

  const char * test = print_loadmap_menu();

  // carrega_mapa(&m);
  carrega_mapa_file(&m, test);

  play_game(&m);
}

   

void ga_play() {
  int i;
  tmapa m;
  tplano *plan;
  int intervalo;
  printf("\033c");

  //TODO: menu with option to load some of the saved maps
  printf(print_solvers_play_menu("Genetic Algorithm",&m)); //TODO: maybe is necessary to pass also the map pointer for further map alocation see dependency injection

  // config_generate_map(m);
  
  // printf("Insert the map!");
  // carrega_mapa(&m);
  
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