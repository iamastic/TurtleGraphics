#define MAXWORDS 50
#define MAXLENGTH 10
#define SCALEFACTOR 2
#define MAXCAPACITY 0.8

#define CURRENTWORD p->words[p->counter]

typedef enum bool {false, true} bool;
typedef enum checker {unused, used} checker;

/*courtesy of Prof. Neill Campbell*/
#define strsame(A,B) (strcmp(A, B)==0)

struct turtle {
  char** words;
  int capacity;
  int counter;
};
typedef struct turtle turtle;
