#include <curses.h>
#include <menu.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lib/mapa.h"

void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color);

int termx, termy, mapwinx, mapwiny, sidebarx, sidebary;

WINDOW *mapwin, *sidebar;

void terminal_start();
void terminal_stop();
void get_window_dimensions();
void draw_window(WINDOW *, int, int, char);
void resizehandler(int);

// start c file here

void terminal_start() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  refresh();
}

void terminal_stop() { endwin(); }

void get_window_dimensions() {
  getmaxyx(stdscr, termy, termx);

  mapwinx = termx;
  mapwiny = sidebary = termy;

  mapwin = newwin(mapwiny, mapwinx, 0, 0);
  sidebar = newwin(sidebary, sidebarx, 0, mapwinx);
}

void draw_window(WINDOW *win, int height, int width, char c) {
  // int x, y;
  // for (y = 0; y < height; y++) {
  //   for (x = 0; x < width; x++) {
  //     mvwaddch(win, y, x, c);
  //   }
  // }
  init_pair(1, COLOR_RED, COLOR_BLACK);

  mvwprintw(win, height - 4, (width/2)-20, "Up and Down arrow keys to naviage (F2 to Exit)");
  // print_in_middle(win, 1, 0, width, "Flood-it", COLOR_PAIR(1));


  wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
  // box(win, 0, 0);


  wrefresh(win);
}

// i stole this idea of a resize handler which just calls start and stop from
// stone soup - http://crawl.develz.org/

void resizehandler(int sig) {
  terminal_stop();
  terminal_start();

  get_window_dimensions();

  draw_window(mapwin, mapwiny, mapwinx, ' ');
  // draw_window(sidebar, sidebary, sidebarx, 's');
}

int main(int argc, char *argv[]) {
  terminal_start();

  termx = 0;
  termy = 0;
  mapwinx = 0;
  mapwiny = 0;
  sidebarx = 25;
  sidebary = 0;

  signal(SIGWINCH, resizehandler);

  int c;
  get_window_dimensions();


  // game loop here
  while (true) {
    // draw_window(mapwin, mapwiny, mapwinx, ' ');
    // draw_window(sidebar, sidebary, sidebarx, 's');

    // handle input
    c = 0;
    c = getch();

    if (c == 'q' || c == 27 /*ESC*/) break;
  }

  terminal_stop();

  return 0;
}