#include <SDL.h>
#include "Stack/stack.h"
#define MAXWORDS 50
#define MAXLENGTH 10
#define ALPHAMAX 26
#define PI 3.14159265358979323846
#define WWIDTH 800
#define WHEIGHT 800
#define SCALEFACTOR 2
#define MAXCAPACITY 0.8
#define HALF 2
#define PIRADIANS 180
#define CURRENTWORD p->words[p->counter]

/*courtesy of Prof. Neill Campbell*/
#define SWAP(A,B) {double temp; temp=A;A=B;B=temp;}
/*courtesy of Prof. Neill Campbell*/
#define ON_ERROR(STR) fprintf(stderr, "\n%s: %s\n", STR, SDL_GetError()); SDL_Quit(); exit(1);
/*courtesy of Prof. Neill Campbell*/
#define strsame(A,B) (strcmp(A, B)==0)

typedef enum checker {unused, used} checker;

struct turtle {
  char** words;
  double alphaFROM [ALPHAMAX];
  double alphaTO [ALPHAMAX];
  int capacity;
  int counter;
  double angle; /*degrees*/
  double x_coor;
  double y_coor;
};
typedef struct turtle turtle;

/*adapted from Prof. Neill Campbell*/
struct SDL_Simplewin {
   SDL_bool finished;
   SDL_Window *win;
   SDL_Renderer *renderer;
};
typedef struct SDL_Simplewin SDL_Simplewin;
