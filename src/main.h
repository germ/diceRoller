#ifndef MAIN_H
#define MAIN_H

#include "dice.h"
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_HISTORY 40

typedef struct windowData {
  WINDOW *win;
  int     width,
          height,
          x,
          y;
  char*   title;
} Window;
typedef struct historyData {
  DiceObj  atk;
  char    *title;
} HistoryData;

//Helper function, Check for and load settings
void loadDice(int nArgs, char** args, DiceItem*** diceList, CharScores** scores);

//Kludge to clear crap left behind by scrolling
int  clean_hack(WINDOW *win, int size, int start, int end);

//Helper function, start curses
void curses_init();

//Helper function, returns finely crafted window
Window* create_window(int width, int height, int x, int y, char* title);

//Draw window title message
int  draw_title(Window *win);

//Do *something* with the crap the user just gave us
int parse_input(char* cmd, HistoryData** history_list, DiceItem** dice_list, CharScores* scores);

#endif /* MAIN_H */
