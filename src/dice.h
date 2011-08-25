/**
* @file dice.h
* @brief Function descriptions pertaining to the handling of n-sided die
* @author Jeremy Bernhardt
* @version Alpha/Dev
* @date 2011-05-30
*/

#define MAX_STRING_LEN 1024   ///< Max string buffer size
#define USELESS_CHAR   '/'    ///< Character that will never be parsed
#define MAX_ENTRIES    100    ///< Maximum number of entries in a seperated dice string

#define STR 0
#define DEX 1
#define CON 2
#define INT 3
#define WIS 4
#define CHA 5

/**
* @brief Structure containing a simple list of player scores for use
*        During rolling
*/
typedef struct charScores {
  int Str,
      Dex,
      Con,
      Int,
      Wis,
      Cha;
} CharScores;

/**
* @brief Structure containing the necessary info for rolling and displaying dice
*/
typedef struct diceRoll {
  char*       resultString;    ///< The formatted output (for printing)
  char*       diceString;      ///< The original string
  int         result;          ///< Roll result
  CharScores* stats;           ///< Character scores, can be a NULL pointer
} DiceObj;

/**
* @brief Structure for containing minimal info about a dice set
*/
typedef struct diceData {
  char* title;
  char* flavor;
  char* diceString;
} DiceItem;

//Creation/Init
/**
* @brief Create a new structure containing player scores
* @param scores A pointer to a 6 interger array
* @return An initilized structure on sucess, NULL on error
*/
CharScores* newCharScore(int* scores);
/**
* @brief Create a new DiceObj 
*
* @param scores An array containg player scores.
* @param diceString 
*             A String containg the dice to be rolled. Input in the form of
*             operator (+, -) seperated entries of either
*               - (Number of Dice)d(Sides per dice)
*               - (Constant)
*
*             Example strings include
*               - 3d6+2
*               - 1d20
*               - 3
*               - 3d6+5d500+3
*               - Etc.
* @return A pointer to a newly initilized DiceObj
*/
DiceObj*    newDiceObj(char* diceString, CharScores* scores);
/**
* @brief Load and seed a DiceItem list.
*
* @param path      Path to the config file
* @param diceList  Address of the pointer where the data will be stored
* @param charScore Address of a CharScore struct or NULL
*/
void        loadDiceFromFile(char* path, DiceItem*** diceList, CharScores** charScore);

//Usage functions
/**
* @brief Preform a dice roll
* @param obj A Initilied DiceObj
*/
void        rollDiceObj(DiceObj* obj);
/**
* @brief One time no-data roll
* @param str Dice string to roll
* @param scores Character scores to use or NULL
* @return Pointer to a string containing a formatted result
*/
char*       rollDiceString(char *str, CharScores* scores);

//Destroy
/**
* @brief Free the passed DiceObj and Null the pointer
* @param obj the object to be freed
*/
void        freeDiceObj(DiceObj* obj);
/**
* @brief Free all the initilized elements of a seperated
* DiceString 
* @param diceString The diceString to free
*/
void        freeDiceString(char** diceString);

//Internal
/**
* @brief Seperate a string into an array of strings based the
*        The characters containted in delin
* @param string The source string
* @param delin The characters to parse at
* @return Pointer to an array of pointers
*/
char**      seperateDiceString(char *string, char* delin);
/**
* @brief Calculate the length of a seperated dice string
* @param diceArray The array whose length will be computed
* @return An interger representing the length of the array
*/
int         getSeperatedLength(char** diceArray);
/**
* @brief Calculated the modifier from a full score
* @param score The full score
* @return The modifier
*/
int calcMod(int score);
