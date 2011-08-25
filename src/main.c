#include "main.h"
#include <time.h>

int       main() {
  srand(time(NULL));
  int      scores[] = {20,10,5,16,17,18};
  DiceObj* obj      = newDiceObj("1d6+CON+STR+DEX", newCharScore(scores));

  int i;
  for (i = 0; i < 5; i++) {
    rollDiceObj(obj);
    printf("%s = %d\n", obj->resultString, obj->result);
  }

  return 0;
}
/*
int       main(int argc, char*    argv[]) {
  //Ncurses Windows
  Window        *history_win,
                *input_win,
                *dice_win;

  //Dice Data
  HistoryData  **history;
  DiceItem     **dice;
  CharScores    *stats;

  int            i;

  //Input buffer
  char*          buffer;
  
  //Ncurses init
  curses_init();

  history_win = create_window(COLS*0.4, LINES, 0, 0, "[ History ]");
  input_win   = create_window(COLS*0.6, 3, LINES-3, COLS*0.4, NULL);
  dice_win    = create_window(COLS*0.6, LINES-3, 0, COLS*0.4, "[ Preset (j/k) ]");
  
  history     = malloc(sizeof(DiceObj*)*MAX_HISTORY);
  buffer      = malloc(sizeof(char)*50);
  dice        = malloc(sizeof(DiceItem*)*MAX_HISTORY);
  stats       = malloc(sizeof(CharScores));
  loadDice(argc, argv, &dice, &stats);

  for (i = 0; i < MAX_HISTORY; i++) history[i] = NULL;
  
  while (1) {
    //Draw borders
    box(history_win->win, '|', '-');
    box(input_win->win,   '|', '-');
    box(dice_win->win,    '|', '-');

    //--DICE WINDOW--

    //Fill dice list
    draw_title(dice_win);

    int offset = 1;
    for (i = 0; dice[i] != NULL; i++) {
      mvwprintw(dice_win->win, offset+i,    1, "%2d) %s", i+1, dice[i]->title);
      mvwprintw(dice_win->win,(offset+i)+1, 7, dice[i]->flavor);

      wattron(dice_win->win, A_BOLD);
      mvwprintw(dice_win->win,(offset+i)+2, 7, dice[i]->diceString);
      wattroff(dice_win->win, A_BOLD);

      offset += 3;
    }
    clean_hack(dice_win->win, (COLS*0.4)-2, i, LINES-2);

    //--HISTORY WINDOW--
    draw_title(history_win);
    //Fill History list
    offset = 1;
    for (i = 0; history[i] != NULL; i++) {
      mvwprintw(history_win->win, i+offset, 1,
                "%2d) %s",i+1, history[i]->title);
      mvwprintw(history_win->win, i+offset+1, 5,
                "%s = %d", history[i]->atk.resultString, 
                history[i]->atk.result);
      offset++;
    }
    if (history[0] != NULL)
      clean_hack(history_win->win, (COLS*0.6)-2, i*2, LINES-2);

    //--INPUT WINDOW--
    for (i = 1; i < (COLS*0.6) -1; i++)
      mvwaddch(input_win->win, 1, i, ' ');

    for (i = 0; history[i] != NULL; i++);
    if ((i != 0) && (history[--i] != NULL))
      mvwprintw(input_win->win, 1, 1, "%-4d| ", history[i]->atk.result);


    //--UPDATE ROUTINES--
    wrefresh(history_win->win);
    wrefresh(dice_win->win);
    wrefresh(input_win->win);

    mvwgetstr(input_win->win, 1, 7, buffer);

    //Process input
    if (parse_input(buffer, history, dice, stats) == 2)
      break;

    //Clear buffer
    memset(buffer, '\0', sizeof(buffer));
  }
  endwin();
  return 0;
}
*/

int       clean_hack(WINDOW *win, int size, int start, int end) {
  int i;
  char* clearString;
  
  //Create blank string
  clearString = malloc(sizeof(char)*(size+1));
  memset(clearString, ' ', size);
  clearString[size] = '\0';

  
  for (i = start; i < LINES-end; i++) 
    mvwprintw(win, i, 1, "%s", clearString);

  free(clearString);

  return 0;
}
int       parse_input(char* cmd, HistoryData** history_list, DiceItem** dice_list, CharScores* scores) {
  //Check if string is valid
  int len = strlen(cmd);

  //Check for quit command, only single character command
  if (len == 1 && cmd[0] == 'q') return 2;
  if (len == 0 || len == 1)      return 1;

  //Extract the first character for testing
  char command      = cmd[0];

  //Create space for new item
  HistoryData *data = malloc(sizeof(HistoryData));

  //Check for raw dice
  char* rawTest = rollDiceString(cmd, scores);  

  //Run tests (Ugly hack)
  //Partial dice set, Make whole
  if (command == 'd') {
    //Create and copy
    data->atk.diceString = malloc(sizeof(char)*strlen(cmd)+2);
    strcpy(data->atk.diceString, "1\0");
    strcat(data->atk.diceString, cmd);
    
    data->title = malloc(sizeof(char)*(strlen(cmd)+1));
    strcpy(data->title, cmd);
  }
  //Roll (n)umber from main list
  else if (command == 'n') {
    int i = atoi(cmd+sizeof(char));
    if (dice_list[i-1] != NULL) {
      //Copy entry to new entry
      data->atk.diceString = malloc(sizeof(dice_list[i-1]->diceString));
      data->title          = malloc(sizeof(dice_list[i-1]->title));

      strcpy(data->atk.diceString, dice_list[i-1]->diceString);
      strcpy(data->title, dice_list[i-1]->title);
    }
    else {
      //Invalid
      free(rawTest);
      return 1;
    }
  }
  //Roll a set that was previously rolled
  else if (command == 'h') {
    int i = atoi(cmd+sizeof(char));
    if (history_list[i] != NULL) {
      //Copy to new object
      data->atk.diceString = malloc(sizeof(history_list[i]->atk.diceString));
      data->title          = malloc(sizeof(history_list[i]->title));

      strcpy(data->atk.diceString,history_list[i]->atk.diceString);
      strcpy(data->title, history_list[i]->title);
    }
    else {
      free(rawTest);
      return 1;
    }
  }
  //Quit the program
  else if (command == 'q') {
    free(rawTest);
    return 2;
  }
  //Dice string is valid for rolling (Ugly hack)
  else if (strcmp(rawTest, " = 0") != 0) { 
    char* new = malloc(sizeof(char)*(strlen(cmd)+1));
    strcpy(new, cmd);

    data->atk.diceString = new;
    data->title = new;
    
    free(new);
  }
  //Undefinded, Clear and exit
  else {
    free(rawTest);
    return 1;
  }
    
  rollDiceObj(&data->atk);

  int i;
  for (i = 0; history_list[i] != NULL; i++)
    ;;

  if (i == 0)
    history_list[0] = data;
  else {
    history_list[i] = data;
    history_list[i+1] = NULL;
  }

  free(rawTest);

  return 0;
}
Window*   create_window(int width, int height, int x, int y, char* title) {
  Window* window = malloc(sizeof(Window));
  window->x       = x;
  window->y       = y;
  window->width   = width;
  window->height  = height;
  window->title   = title;
  window->win     = newwin(height, width, x, y);

  return window;
}
int       draw_title(Window *win) { 
  if (win->title != NULL) {
    int offset = (win->width/2) - (strlen(win->title)/2);
    mvwprintw(win->win, 0, offset, win->title); 
  }

  return 0;
}
void      loadDice(int nArgs, char** args, DiceItem*** diceList, CharScores** scores) {
  //Check if a config file was passed
  if (nArgs == 2) {
    loadDiceFromFile(args[1], diceList, scores);
    return;
  }
  
  //No config, show error
  DiceItem* notice = malloc(sizeof(DiceItem));
  notice->title    = "No dice loaded";
  notice->flavor   = "No config file passed. See the documentation for more info.";
  notice->diceString = "1d20";

  DiceItem** list = malloc(sizeof(DiceItem*)*2);
  list[0]         = notice;
  list[1]         = NULL;
  *diceList       = list; 
  

  return;
}
void      curses_init() {
  initscr();
  cbreak();
}
