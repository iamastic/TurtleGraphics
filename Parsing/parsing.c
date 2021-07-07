#include "parsingSpecific.h"
#include "parsing.h"

int main (int argc, char** argv)
{
  FILE *fpointer;
  turtle tutl;

  if (argc != 2) {
    on_error("Missing a .txt file");
  }

  test();
  fpointer = nfopen(argv[1], "rt");
  importTutlFile(fpointer, &tutl);

  progMain(&tutl);

  fclose(fpointer);
  n2dfree(tutl.words, tutl.capacity);

  return 0;
}

void importTutlFile (FILE* fp, turtle* p)
{
  int i = 0;
  p->counter = 0;
  p->capacity = MAXWORDS;
  p->words = n2dcalloc(MAXWORDS, MAXLENGTH, sizeof(char));

  while (fscanf(fp, "%s", p->words[i++]) == 1) {
    if (i >= p->capacity * MAXCAPACITY) {
      p->words = n2drecalloc(p->words, p->capacity, p->capacity * SCALEFACTOR, MAXLENGTH, MAXLENGTH, sizeof(char));
      p->capacity = p->capacity * SCALEFACTOR;
    }
  }
}

void progMain (turtle* p)
{
  if (strsame(CURRENTWORD,"{")) {
    p->counter++;
    instrctlst(p);
  }
  else {
    on_error("Was expecting an Opening Bracket '{'");
  }
  p->counter++;

  if (!strsame(CURRENTWORD,"\0")) {
    on_error("Something after Final Bracket");
  }
}

void instrctlst (turtle* p)
{
  if (strsame(CURRENTWORD,"}")) {
    return;
  }
  if (strsame(CURRENTWORD,"\0")) {
    on_error("Missing a Closing Bracket or Instruction");
  }
  else {
    instructions(p);
    p->counter++;
    instrctlst(p);
  }
}

void instructions (turtle* p)
{
  if (strsame(CURRENTWORD,"FD")) {
    p->counter++;
    moveForward(p);
    return;
  }
  else if (strsame(CURRENTWORD,"LT")) {
    p->counter++;
    turnLeft(p);
    return;
  }
  else if (strsame(CURRENTWORD,"RT")) {
    p->counter++;
    turnRight(p);
    return;
  }
  else if (strsame(CURRENTWORD,"DO")) {
    p->counter++;
    doLoop(p);
    return;
  }
  else if (strsame(CURRENTWORD,"SET")) {
    p->counter++;
    setVariable(p);
    return;
  }
  else {
    on_error("Was Expecting an Instructiong e.g. 'FD'");
  }
}

void moveForward (turtle* p)
{
  if (varNum(p)) {
    return;
  }
  else {
    on_error("Was expecting a Variable [A-Z] or Number");
  }
}

void turnLeft (turtle* p)
{
  if (varNum(p)) {
    return;
  }
  else {
    on_error("Was expecting a Variable [A-Z] or Number");
  }
}

void turnRight (turtle* p)
{
  if (varNum(p)) {
    return;
  }
  else {
    on_error("Was expecting a Variable [A-Z] or Number");
  }
}

void doLoop (turtle* p)
{
  if (!isLetter(p)) {
    on_error("Was expecting a Variable [A-Z]");
  }
  p->counter++;

  if (!strsame(CURRENTWORD,"FROM")) {
    on_error("Was expecting 'FROM'");
  }
  p->counter++;

  if (!varNum(p)) {
    on_error("Was expecting a Variable [A-Z] or Number");
  }
  p->counter++;

  if (!strsame(CURRENTWORD,"TO")) {
    on_error("Was expecting 'TO'");
  }
  p->counter++;

  if (!varNum(p)) {
    on_error("Was expecting a Variable [A-Z] or Number");
  }
  p->counter++;

  if (!strsame(CURRENTWORD,"{")) {
    on_error("Was expecting a '{' after DO statement");
  }
  p->counter++;
  instrctlst(p);
}

bool varNum (turtle* p)
{
  if (isLetter(p)) {
    return true;
  }
  else if (isNumber(p)) {
    return true;
  }
  else {
    return false;
  }
}

bool isLetter (turtle* p)
{
  int length;
  char c;

  length = strlen(CURRENTWORD);
  /*can only be one letter*/
  if (length != 1) {
    return false;
  }
  c = CURRENTWORD[0];
  if (!isalpha(c)) {
    return false;
  }
  if (c < 'A' || c > 'Z') {
    return false;
  }
  return true;
}

bool isNumber (turtle* p)
{
  int length, cell, checkDigit, flag;
  checker decimalPoint;

  flag = 0;
  decimalPoint = unused;
  cell = 0;
  length = strlen(CURRENTWORD);
  checkDigit = isdigit(CURRENTWORD[cell]);
  /*the first could be a digit or negative sign*/
  if (checkDigit == 0) {
    if (CURRENTWORD[cell] != '-') {
      return false;
    }
  }
  /*start from the second cell*/
  for (cell = 1; cell < length; cell++) {
    checkDigit = isdigit(CURRENTWORD[cell]);
    if (checkDigit == 0) {
      if ((CURRENTWORD[cell] == '.') && (decimalPoint == unused)) {
        decimalPoint = used;
        flag--;
      }
      if (CURRENTWORD[cell] != '.') {
        flag++;
      }
      flag++;
    }
  }
  if (flag > 0) {
    return false;
  }
  else {
    return true;
  }
}

bool isOp (turtle* p)
{
  int length;

  length = strlen(CURRENTWORD);

  if (length != 1) {
    return false;
  }
  if (strsame(CURRENTWORD,"+")) {
    return true;
  }
  if (strsame(CURRENTWORD,"-")) {
    return true;
  }
  if (strsame(CURRENTWORD,"*")) {
    return true;
  }
  if (strsame(CURRENTWORD,"/")) {
    return true;
  }
  return false;
}

void setVariable (turtle* p)
{
  if (!isLetter(p)) {
    on_error("Was expecting a Variable [A-Z]");
  }
  p->counter++;

  if (!strsame(CURRENTWORD,":=")) {
    on_error("Was expecting ':='");
  }
  p->counter++;

  polishNotation(p);
}

void polishNotation (turtle* p)
{
  if (strsame(CURRENTWORD,";")) {
    return;
  }
  else if (varNum(p)) {
    p->counter++;
    polishNotation(p);
  }
  else if (isOp(p)) {
    p->counter++;
    polishNotation(p);
  }
  else {
    on_error("Incorrect Polish Notation structure");
  }
}

void test (void)
{
  turtle a;
  char x1 [MAXLENGTH] = "123";
  char x2 [MAXLENGTH] = "0.345";
  char x3 [MAXLENGTH] = "2000.33";
  char x4 [MAXLENGTH] = "-896";
  char x5 [MAXLENGTH] = "-123.22";

  char x6 [MAXLENGTH] = "1.2.3";
  char x7 [MAXLENGTH] = "-23-44";
  char x8 [MAXLENGTH] = "abc";
  char x9 [MAXLENGTH] = "XYZ";
  char x10 [MAXLENGTH] = " ";
  char x11 [MAXLENGTH] = "123abc";

  char y1 [MAXLENGTH] = "A";
  char y2 [MAXLENGTH] = "B";
  char y3 [MAXLENGTH] = "M";
  char y4 [MAXLENGTH] = "Z";
  char y5 [MAXLENGTH] = "Y";

  char y6 [MAXLENGTH] = "Aa";
  char y7 [MAXLENGTH] = "AB";
  char y8 [MAXLENGTH] = "123";
  char y9 [MAXLENGTH] = "A Y";
  char y10 [MAXLENGTH] = " ";
  char y11 [MAXLENGTH] = "H1";
  char y12 [MAXLENGTH] = "c";

  char z1 [MAXLENGTH] = "+";
  char z2 [MAXLENGTH] = "*";
  char z3 [MAXLENGTH] = "/";
  char z4 [MAXLENGTH] = "-";

  char z5 [MAXLENGTH] = ".";
  char z6 [MAXLENGTH] = " ";
  char z7 [MAXLENGTH] = "++";
  char z8 [MAXLENGTH] = "-+";
  char z9 [MAXLENGTH] = "99";
  char z10 [MAXLENGTH] = "Aa";

  /*testing isNumber*/
  a.words = n2dcalloc(MAXWORDS, MAXLENGTH, sizeof(char));
  a.counter = 0;
  a.capacity = MAXWORDS;

  strcpy(a.words[0], x1);
  strcpy(a.words[1], x2);
  strcpy(a.words[2], x3);
  strcpy(a.words[3], x4);
  strcpy(a.words[4], x5);

  strcpy(a.words[5], x6);
  strcpy(a.words[6], x7);
  strcpy(a.words[7], x8);
  strcpy(a.words[8], x9);
  strcpy(a.words[9], x10);
  strcpy(a.words[10], x11);

  assert(isNumber(&a));
  a.counter++;
  assert(isNumber(&a));
  a.counter++;
  assert(isNumber(&a));
  a.counter++;
  assert(isNumber(&a));
  a.counter++;
  assert(isNumber(&a));

  a.counter++;
  assert(!isNumber(&a));
  a.counter++;
  assert(!isNumber(&a));
  a.counter++;
  assert(!isNumber(&a));
  a.counter++;
  assert(!isNumber(&a));
  a.counter++;
  assert(!isNumber(&a));
  a.counter++;
  assert(!isNumber(&a));

  n2dfree(a.words, a.capacity);

  /*testing isLetter*/
  a.words = n2dcalloc(MAXWORDS, MAXLENGTH, sizeof(char));
  a.counter = 0;
  a.capacity = MAXWORDS;

  strcpy(a.words[0], y1);
  strcpy(a.words[1], y2);
  strcpy(a.words[2], y3);
  strcpy(a.words[3], y4);
  strcpy(a.words[4], y5);

  strcpy(a.words[5], y6);
  strcpy(a.words[6], y7);
  strcpy(a.words[7], y8);
  strcpy(a.words[8], y9);
  strcpy(a.words[9], y10);
  strcpy(a.words[10], y11);
  strcpy(a.words[11], y12);

  assert(isLetter(&a));
  a.counter++;
  assert(isLetter(&a));
  a.counter++;
  assert(isLetter(&a));
  a.counter++;
  assert(isLetter(&a));
  a.counter++;
  assert(isLetter(&a));

  a.counter++;
  assert(!isLetter(&a));
  a.counter++;
  assert(!isLetter(&a));
  a.counter++;
  assert(!isLetter(&a));
  a.counter++;
  assert(!isLetter(&a));
  a.counter++;
  assert(!isLetter(&a));
  a.counter++;
  assert(!isLetter(&a));
  a.counter++;
  assert(!isLetter(&a));

  n2dfree(a.words, a.capacity);

  /*testing isOp*/
  a.words = n2dcalloc(MAXWORDS, MAXLENGTH, sizeof(char));
  a.counter = 0;
  a.capacity = MAXWORDS;

  strcpy(a.words[0], z1);
  strcpy(a.words[1], z2);
  strcpy(a.words[2], z3);
  strcpy(a.words[3], z4);
  strcpy(a.words[4], z5);

  strcpy(a.words[5], z6);
  strcpy(a.words[6], z7);
  strcpy(a.words[7], z8);
  strcpy(a.words[8], z9);
  strcpy(a.words[9], z10);

  assert(isOp(&a));
  a.counter++;
  assert(isOp(&a));
  a.counter++;
  assert(isOp(&a));
  a.counter++;
  assert(isOp(&a));

  a.counter++;
  assert(!isOp(&a));
  a.counter++;
  assert(!isOp(&a));
  a.counter++;
  assert(!isOp(&a));
  a.counter++;
  assert(!isOp(&a));
  a.counter++;
  assert(!isOp(&a));
  a.counter++;
  assert(!isOp(&a));

  n2dfree(a.words, a.capacity);
}
