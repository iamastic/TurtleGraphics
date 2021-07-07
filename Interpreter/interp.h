#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

/*start up an SDL window*/
void initializeSDLWindow (SDL_Simplewin* w);
/*run program till user presses quit*/
void sdlEvents (SDL_Simplewin* w);
/*reads file and initialises turtle struct*/
void importTutlFile (FILE* fp, turtle* p);
/*initiates program*/
void progMain (turtle* p, SDL_Simplewin* w);
void instrctlst (turtle* p, SDL_Simplewin* w);
void instructions (turtle* p, SDL_Simplewin* w);
void moveForward (turtle* p, SDL_Simplewin* w);
/*get number stored in variable*/
double getValue (turtle* p);
/*convert degrees to radians*/
double convertToRadians (turtle* p);
/*get next x coordinate based off of angle and distance*/
double getXCoordinate (turtle* p, double distance, double rad);
/*get next y coordinate based off of angle and distance*/
double getYCoordinate (turtle* p, double distance, double rad);
void turnLeft (turtle* p);
void turnRight (turtle* p);
void doLoop (turtle* p, SDL_Simplewin* w);
/*checks if letter or number*/
bool varNum (turtle* p);
bool isLetter (turtle* p);
bool isNumber (turtle* p);
/*declares a variable*/
void setVariable (turtle* p);
/*calculates the polish notation result*/
void polishNotation (turtle* p, stack* s, int alphaIndex);
/*checks if it is an operator and operates accordingly*/
bool isOp (turtle* p, stack* s);
void test (void);
