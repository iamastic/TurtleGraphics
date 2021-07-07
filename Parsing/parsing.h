#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include "../General/general.h"

/*reads file and initialises turtle struct*/
void importTutlFile (FILE* fp, turtle* p);
/*initiates program*/
void progMain (turtle* p);
void instrctlst (turtle* p);
void instructions (turtle* p);
void moveForward (turtle* p);
void turnLeft (turtle* p);
void turnRight (turtle* p);
void doLoop (turtle* p);
/*checks if letter or number*/
bool varNum (turtle* p);
bool isLetter (turtle* p);
bool isNumber (turtle* p);
/*checks if it is an operator*/
bool isOp (turtle* p);
/*declares a variable*/
void setVariable (turtle* p);
/*checks if it is polish notation*/
void polishNotation (turtle* p);
void test (void);
