#ifndef DICE_C
#define DICE_C

/**
* @file dice.c
* @brief Implementation of functions defined and explained
*        in dice.h. Note: It is up to the developer to seed
*        the RNG.
* @author Jeremy Bernhardt
* @version Alpha/Dev
* @date 2011-05-31
*/

#include "dice.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

DiceObj*    newDiceObj(char* diceString, CharScores* scores) {
  //Initilize
  DiceObj* obj = malloc(sizeof(DiceObj));
  bzero(obj, sizeof(DiceObj));

  //Set values
  obj->diceString  = diceString;
  obj->stats       = scores;

  return obj;
}
CharScores* newCharScore(int* scores) {
  //Check for correct sizing
  CharScores* stats = malloc(sizeof(CharScores));

  stats->Str = scores[STR];
  stats->Dex = scores[DEX];
  stats->Con = scores[CON];
  stats->Int = scores[INT];
  stats->Wis = scores[WIS];
  stats->Cha = scores[CHA];

  return stats;
}

void     freeDiceObj(DiceObj* obj) {
  //Studies show that freeing a NULL pointer is the leading
  //cause of cancer. Eat your wheaties and munch your K&R
  if (obj == NULL) 
    return;

  //Check if the stats were initilized, free
  if (obj->stats != NULL)
    free(obj->stats);

  //Lastly clean up the main object, NULL the pointer
  free(obj);
  obj = NULL;

  return;
}
void     rollDiceObj(DiceObj* obj) {
  //Seperate dicestring based on operators
  char** entries     = seperateDiceString(obj->diceString, "+-");
  int    arrayLength = getSeperatedLength(entries);
  int    total       = 0;
  int    i;
  
  //Create a temporary var for the the formatString
  char* formatString = malloc(sizeof(char)*MAX_STRING_LEN);

  //Loop through the elements
  for (i = 0; i < arrayLength; i++) {
    //Split our element down farther
    char** sep = seperateDiceString(entries[i], "d");
    int    len = getSeperatedLength(sep);

    //Two interger elements, dice string
    if ((len == 2) && (atoi(sep[0]) != 0) && (atoi(sep[1]) != 0))
    {
      int size   = atoi(sep[1]);
      int amount = atoi(sep[0]);
      int j;

      //Formatting Jazz
      for (j = 0; j < amount; j++) {
        int currentRoll = (rand()%size)+1;
        total += currentRoll;

        //Opening Brace
        if      (j == 0 && i != 0) 
          sprintf(formatString, "%s + d%d[", formatString, size);
        else if (j == 0 && i == 0) 
          sprintf(formatString, "d%d[", size);
        
        //Middle of section
        if (j == 0)
          sprintf(formatString, "%s%d",
                  formatString, currentRoll);
        else if (j != 0)
          sprintf(formatString, "%s %d",
                  formatString, currentRoll);

        //Closing brace
        if (j == amount-1) 
          sprintf(formatString,"%s]",formatString);
      }
    }
    //If one interger element, Simple mod
    else if ((len == 1) && (atoi(sep[0]) != 0))  {
      int currentRoll = atoi(sep[0]);
      total += currentRoll;

      if (i != 0)
        sprintf(formatString, "%s + [%d]",
                formatString, currentRoll);
      else
        sprintf(formatString, "[%d]", currentRoll);
    }
    //Modifer check
    else if  ((len == 1) && (obj->stats != NULL)) {
      int     mod,    //Modifier in the string 
              val;    //Modifier value
      char* textList[7] = {"STR", "DEX", "CON", "INT", "WIS", "CHA", ""};

      for (mod = 0; mod < 6; mod++) 
        if (strcmp(sep[0], textList[mod]) == 0) break;

      //Not found, free alloc'd mem and proceed
      if (mod == 6) {
        freeDiceString(sep);
        continue;
      }
      
      //Retreive Mod value
      switch(mod) {
        case 0:
          val = obj->stats->Str;
          break;
        case 1:
          val = obj->stats->Dex;
          break;
        case 2:
          val = obj->stats->Con;
          break;
        case 3:
          val = obj->stats->Int;
          break;
        case 4:
          val = obj->stats->Wis;
          break;
        case 5:
          val = obj->stats->Cha;
          break;
      }
      
      val = calcMod(val);
      total += val;

      //Formatting jazz
      if (i != 0)
        sprintf(formatString, "%s + [%s:%d]", formatString, textList[mod], val);
      else
        sprintf(formatString, "[%s:%d]", textList[mod], val);
    }
    
    //Free allocated section
    freeDiceString(sep);
  }

  //Free allocated elements
  freeDiceString(entries);

  //Store the result
  obj->result = total;
  obj->resultString = formatString;
}
char*    rollDiceString(char *str, CharScores* scores) {
  DiceObj* roller = newDiceObj(str, scores);
  rollDiceObj(roller);

  char* returnStr = malloc(sizeof(char*)*MAX_STRING_LEN);
  sprintf(returnStr, "%s = %d", roller->resultString, roller->result);

  freeDiceObj(roller);
  return returnStr;
}

char**   seperateDiceString(char *orgString, char* delin) {
  char **returnVar = malloc(sizeof(char*)*MAX_ENTRIES);
  int    i;
  char  *prev, 
        *next;

  prev = orgString;
  next = strpbrk(prev, delin);
  
  if (next == NULL) {
    returnVar[0] = malloc(strlen(orgString)*sizeof(char));
    strcpy(returnVar[0], orgString);
    returnVar[1] = NULL;
    return returnVar;
  }

  for (i = 0; next != NULL; i++) {
    //Copy the seperated part out
    int strLen = (next-prev)+1;
    returnVar[i] = malloc(strLen);
    strncpy(returnVar[i], prev, strLen-1);
    returnVar[i][strLen] = '\0';

    //Update the pointers
    prev = next+sizeof(char);
    next = strpbrk(prev, delin);
  }

  returnVar[i] = malloc(sizeof(char)*strlen(prev)+1);
  strcpy(returnVar[i], prev);
  returnVar[i+1] = NULL;

  return returnVar;
}
void     freeDiceString(char** diceString) {
  int i; 
  for (i = 0; diceString[i] != NULL; i++)
    free(diceString[i]);
}
int      getSeperatedLength(char** diceArray) {
  int i = 0;
  while (diceArray[i] != NULL) i++;

  return i;
}
int      calcMod(int score) {
  return (int)((score-10)/2);
}

void     loadDiceFromFile(char* path, DiceItem*** diceList, CharScores** charScore) {
  //Standard as usual boys.
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    perror("ERROR: NO CONFIG");
    return;
  }
  
  //--This section will search through each line of the file, 
  //  Parsing as necessary
  DiceItem**  list  = malloc(sizeof(DiceItem*)*MAX_ENTRIES);
  CharScores* stats = NULL;
  int         skip  = 0,
              i;
  char        line  [MAX_STRING_LEN*3];

  //NULL the list
  for (i = 0; i < MAX_ENTRIES; i++) 
    list[i] = NULL;

  //Read each line to buffer, act on it
  for (i = 0; fgets(line, MAX_STRING_LEN*3, file) != NULL; i++) {
    //Comment detection
    if (line[0] == ';' || line[0] == '\n') { 
      skip++;
      continue; 
    }
    
    //Stat detection
    if (line[0] == '#') {
      stats = malloc(sizeof(CharScores));
      sscanf(line, "#%d,%d,%d,%d,%d,%d", &(stats->Str),
                                         &(stats->Dex),
                                         &(stats->Con),
                                         &(stats->Int),
                                         &(stats->Wis),
                                         &(stats->Cha));
      skip++;
      continue;
    }

    //String parsing
    char* title    = malloc(sizeof(char)*MAX_STRING_LEN);
    char* flavor   = malloc(sizeof(char)*MAX_STRING_LEN);
    char* dice     = malloc(sizeof(char)*MAX_STRING_LEN);

    sscanf(line, "%[^,],%[^,],%[^,\n]", title, dice, flavor);

    DiceItem* item   = malloc(sizeof(DiceItem));
    item->title      = title;
    item->flavor     = flavor;
    item->diceString = dice;

    list[i-skip]     = item;
  }

  list[i-skip] = NULL;

  //Set and return
  *diceList  = list;
  *charScore = stats;
  return;
}

#endif /* DICE_C */
