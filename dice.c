/**
* @file dice.c
* @brief Implementation of functions defined and explained
*        in dice.h
* @author Jeremy Bernhardt
* @version Alpha/Dev
* @date 2011-05-31
*/

#include "dice.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


DiceObj* newDiceObj(char* diceString) {
  //Initilize
  DiceObj* obj = malloc(sizeof(DiceObj));
  bzero(obj, sizeof(DiceObj));

  //Set and return
  obj->diceString  = diceString;
  return obj;
}
void     freeDiceObj(DiceObj* obj) {
  //Free and Null object
  free(obj);
  obj = NULL;
}
void     rollDiceObj(DiceObj* obj) {
  //Seed the RNG
  srand(time(NULL));

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

    //If two interger elements
    if ((len == 2) 
     && (atoi(sep[0]) != 0)
     && (atoi(sep[1]) != 0))
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
    //If one interger element
    else if ((len == 1)
          && (atoi(sep[0]) != 0)) {
      int currentRoll = atoi(sep[0]);
      total += currentRoll;

      if (i != 0)
        sprintf(formatString, "%s + [%d]",
                formatString, currentRoll);
      else
        sprintf(formatString, "[%d]", currentRoll);
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
char*    rollDiceString(char *str) {
  DiceObj* roller = newDiceObj(str);
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

  returnVar[i] = malloc(sizeof(char)*strlen(prev));
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

DiceItem** loadDiceFromFile(char* path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) 
    return NULL;
  
  DiceItem** list = malloc(sizeof(DiceItem*)*MAX_ENTRIES);
  char       line  [MAX_STRING_LEN*3];
  int        i, 
             skip = 0;

  for (i = 0; i < MAX_ENTRIES; i++) list[i] = NULL;
  for (i = 0; fgets(line, MAX_STRING_LEN*3, file) != NULL; i++) {
    if (line[0] == ';') { //Comment detection
      skip++;
      continue; 
    }

    char *delin, *delin_2;
    delin      = strchr(line, ',');
    if (delin == NULL) break;
    delin[0]   = '\0';
    delin++;

    delin_2    = strchr(delin, ',');
    if (delin_2 == NULL) break;
    delin_2[0] = '\0';
    delin_2++;
    
    char* nl = strchr(delin_2, '\n');
    if (nl != NULL) nl[0] = '\0';

    DiceItem* item    = malloc(sizeof(DiceItem));
    item->title       = malloc(sizeof(char)*strlen(line));
    item->diceString  = malloc(sizeof(char)*strlen(delin));
    item->flavor      = malloc(sizeof(char)*strlen(delin_2));

    strcpy(item->title,      line);
    strcpy(item->diceString, delin);
    strcpy(item->flavor,     delin_2);
    
    list[i-skip] = item;
  }

  list[i-skip] = NULL;

  return list;
}
