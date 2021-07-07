#include "interpSpecific.h"
#include "interp.h"

int main (int argc, char** argv)
{
  FILE *fpointer;
  turtle tutl;
  SDL_Simplewin window;

  if (argc != 2) {
    on_error("Missing a .txt file");
  }

  test();
  fpointer = nfopen(argv[1], "rt");
  importTutlFile(fpointer, &tutl);

  initializeSDLWindow (&window);

  progMain(&tutl, &window);

  sdlEvents(&window);

  fclose(fpointer);
  n2dfree(tutl.words, tutl.capacity);

  SDL_DestroyRenderer(window.renderer);
  SDL_DestroyWindow(window.win);
  SDL_Quit();

  return 0;
}

void initializeSDLWindow (SDL_Simplewin* w)
{
  SDL_Init(SDL_INIT_VIDEO);
  w->finished = 0;
  w->win = SDL_CreateWindow ("Turtle", SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            WWIDTH, WHEIGHT,
                                            SDL_WINDOW_SHOWN);
  if (w->win == NULL) {
    ON_ERROR("Unable to Initialise SDL");
  }

  w->renderer = SDL_CreateRenderer (w->win,-1,
                                    SDL_RENDERER_ACCELERATED |
                                    SDL_RENDERER_TARGETTEXTURE);
  if (w->renderer == NULL) {
    ON_ERROR("Unable to Initialise SDL Renderer");
  }
  /*black*/
  SDL_SetRenderDrawColor (w->renderer, 0,0,0,255);
  SDL_RenderClear (w->renderer);
}

void sdlEvents (SDL_Simplewin* w)
{
  SDL_Event event;

  while (!w->finished) {
    SDL_Delay(20);
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        w->finished = 1;
      }
    }
  }
}

void importTutlFile (FILE* fp, turtle* p)
{
  int i = 0;
  p->counter = 0;
  p->angle = 0;
  p->x_coor = WWIDTH / HALF;
  p->y_coor = WHEIGHT / HALF;
  p->capacity = MAXWORDS;
  p->words = n2dcalloc(MAXWORDS, MAXLENGTH, sizeof(char));

  while (fscanf(fp, "%s", p->words[i++]) == 1) {
    if (i >= p->capacity * MAXCAPACITY) {
      p->words = n2drecalloc(p->words, p->capacity,
      p->capacity * SCALEFACTOR, MAXLENGTH, MAXLENGTH, sizeof(char));
      p->capacity = p->capacity * SCALEFACTOR;
    }
  }

  for (i = 0; i < ALPHAMAX; i++) {
    p->alphaFROM [i] = 0;
    p->alphaTO [i] = 0;
  }
}

void progMain (turtle* p, SDL_Simplewin* w)
{
  if (strsame(CURRENTWORD,"{")) {
    p->counter++;
    instrctlst(p, w);
  }
  else {
    on_error("Was expecting an Opening Bracket '{'");
  }
  p->counter++;

  if (!strsame(CURRENTWORD,"\0")) {
    on_error("Something after Final Bracket");
  }
}

void instrctlst (turtle* p, SDL_Simplewin* w)
{
  if (strsame(CURRENTWORD,"}")) {
    return;
  }
  if (strsame(CURRENTWORD,"\0")) {
    on_error("Missing a Closing Bracket or Instruction");
  }
  else {
    instructions(p, w);
    p->counter++;
    instrctlst(p, w);
  }
}

void instructions (turtle* p, SDL_Simplewin* w)
{
  if (strsame(CURRENTWORD,"FD")) {
    p->counter++;
    moveForward(p, w);
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
    doLoop(p, w);
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

void moveForward (turtle* p, SDL_Simplewin* w)
{
  double distance, x_new, y_new, rad;

  if (varNum(p)) {
    distance = getValue(p);
    rad = convertToRadians(p);
    x_new = getXCoordinate(p, distance, rad);
    y_new = getYCoordinate(p, distance, rad);
    /*white*/
    SDL_SetRenderDrawColor(w->renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(w->renderer, p->x_coor, p->y_coor, x_new, y_new);
    SDL_RenderPresent(w->renderer);

    p->x_coor = x_new;
    p->y_coor = y_new;

    return;
  }
  else {
    on_error("Was expecting a Variable [A-Z] or Number");
  }
}

double getValue (turtle* p)
{
  double value;
  int alphaIndex;
  char* tmpptr;

  if (!isalpha(CURRENTWORD [0])) {
    value = strtod(CURRENTWORD, &tmpptr);
  }
  else {
    alphaIndex = CURRENTWORD [0] - 'A';
    value = p->alphaFROM[alphaIndex];
  }
  return value;
}


double convertToRadians (turtle* p)
{
  double rad;
  rad = p->angle * PI / PIRADIANS;
  return rad;
}

double getXCoordinate (turtle* p, double distance, double rad)
{
  double x;
  x = p->x_coor + (distance * sin(rad));
  return x;
}

double getYCoordinate (turtle* p, double distance, double rad)
{
  double y;
  y = p->y_coor - (distance * cos(rad));
  return y;
}

void turnLeft (turtle* p)
{
  double value;

  if (varNum(p)) {
    value = getValue(p);
    p->angle = p->angle - value;
    return;
  }
  else {
    on_error("Was expecting a Variable [A-Z] or Number \n");
  }
}

void turnRight (turtle* p)
{
  double value;

  if (varNum(p)) {
    value = getValue(p);
    p->angle = p->angle + value;
    return;
  }
  else {
    on_error("Was expecting a Variable [A-Z] or Number \n");
  }
}

void doLoop (turtle* p, SDL_Simplewin* w)
{
  int alphaIndex, buffer, buffer2;
  double value, from, to, i;
  bool swapped = false;

  if (!isLetter(p)) {
    on_error("Was expecting a Variable [A-Z]");
  }

  alphaIndex = CURRENTWORD[0] - 'A';
  p->counter++;

  if (!strsame(CURRENTWORD,"FROM")) {
    on_error("Was expecting 'FROM'");
  }
  p->counter++;

  if (!varNum(p)) {
    on_error("Was expecting a Variable [A-Z] or Number");
  }

  value = getValue(p);
  p->alphaFROM[alphaIndex] = value;
  p->counter++;

  if (!strsame(CURRENTWORD,"TO")) {
    on_error("Was expecting 'TO'");
  }
  p->counter++;

  if (!varNum(p)) {
    on_error("Was expecting a Variable [A-Z] or Number");
  }

  value = getValue(p);
  p->alphaTO[alphaIndex] = value;
  p->counter++;

  if (!strsame(CURRENTWORD,"{")) {
    on_error("Was expecting a '{' after DO statement");
  }
  from = p->alphaFROM[alphaIndex];
  to = p->alphaTO[alphaIndex];
  /*in order to create a decrementing loop*/
  if (from > to) {
    SWAP(from, to);
    swapped = true;
  }
  /*preserve counter for when we return*/
  p->counter++;
  buffer = p->counter;
  buffer2 = 0;

  for (i = from; i <= to; i++) {
    instrctlst(p, w);
    if (swapped) {
      p->alphaFROM[alphaIndex]--;
    }
    else {
      p->alphaFROM[alphaIndex]++;
    }
    /*reassign counter to maintain loop*/
    buffer2 = p->counter;
    p->counter = buffer;
  }
  /*set counter to just after the loop*/
  p->counter = buffer2;
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

  if (length != 1) {
    return false;
  }
  c = CURRENTWORD [0];
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

  if (checkDigit == 0) {
    if (CURRENTWORD[cell] != '-') {
      return false;
    }
  }

  for (cell = 1; cell < length; cell++) {
    checkDigit = isdigit(CURRENTWORD[cell]);
    if (checkDigit == 0) {
      if ((CURRENTWORD[cell] == '.') && (decimalPoint == unused)) {
        decimalPoint = used;
      }
      else {
        flag = 1;
      }
    }
  }
  if (flag != 0) {
    return false;
  }
  else {
    return true;
  }
}

void setVariable (turtle* p)
{
  int alphaIndex;
  stack* rpn;

  if (!isLetter(p)) {
    on_error("Was expecting a Variable [A-Z]");
  }

  alphaIndex = CURRENTWORD[0] - 'A';
  p->counter++;

  if (!strsame(CURRENTWORD,":=")) {
    on_error("Was expecting ':='");
  }
  p->counter++;

  rpn = stack_init();

  polishNotation(p, rpn, alphaIndex);

  if (stack_free(rpn)) {
    return;
  }
}

void polishNotation (turtle* p, stack* s, int alphaIndex)
{
  double result, value;
  int stackSize;
  /*stack over/underflow*/
  if (strsame(CURRENTWORD,";")) {
    if ((stackSize = stack_size(s)) != 1) {
      on_error("Incorrect Polish Notation structure");
    }
    else {
      stack_pop(s, &result);
      p->alphaFROM[alphaIndex] = result;
    }
    return;
  }
  else if (varNum(p)) {
    value = getValue(p);
    stack_push(s, value);
    p->counter++;
    polishNotation(p, s, alphaIndex);
  }
  else if (isOp(p, s)) {
    p->counter++;
    polishNotation(p, s, alphaIndex);
  }
  else {
    on_error("Incorrect Polish Notation structure");
  }
}


bool isOp (turtle* p, stack* s)
{
  int length;
  double result, operand1, operand2;

  length = strlen(CURRENTWORD);

  if (length != 1) {
    return false;
  }
  if (!stack_pop (s, &operand1)) {
    on_error("Incorrect Polish Notation structure");
  }
  if (!stack_pop (s, &operand2)) {
    on_error("Incorrect Polish Notation structure");
  }
  if (strsame(CURRENTWORD,"+")) {
    result = operand2 + operand1;
    stack_push (s, result);
    return true;
  }
  if (strsame(CURRENTWORD,"-")) {
    result = operand2 - operand1;
    stack_push (s, result);
    return true;
  }
  if (strsame(CURRENTWORD,"*")) {
    result = operand2 * operand1;
    stack_push (s, result);
    return true;
  }
  if (strsame(CURRENTWORD,"/")) {
    result = operand2 / operand1;
    stack_push (s, result);
    return true;
  }
  return false;
}

void test (void)
{
  turtle a;
  double radians, difference, x_new, y_new;

  /*testing convertToRadians*/
  a.angle = 45;
  radians = convertToRadians(&a);
                                /*1/4 * PI */
  difference = fabs(radians - 0.7853981634);
  assert(difference < 0.0000001);

  a.angle = 90;
  radians = convertToRadians(&a);
                                /*1/2 * PI */
  difference = fabs(radians - 1.570796327);
  assert(difference < 0.0000001);

  a.angle = 100;
  radians = convertToRadians(&a);
                                /*5/9 * PI */
  difference = fabs(radians - 1.745329252);
  assert(difference < 0.0000001);

  a.angle = -35;
  radians = convertToRadians(&a);
                                /*7/36 * PI */
  difference = fabs(radians + 0.6108652382);
  assert(difference < 0.0000001);

  /*testing getXCoordinate*/
  a.x_coor = 200.0;
  x_new = getXCoordinate (&a, 30.0, 0.7853981634);
  difference = fabs(x_new - 221.2132034);
  assert(difference < 0.0000001);

  a.x_coor = 255.12;
  x_new = getXCoordinate (&a, 442.0, 1.570796327);
  difference = fabs(x_new - 697.12);
  assert(difference < 0.0000001);

  a.x_coor = 15;
  x_new = getXCoordinate (&a, -30, 0);
  difference = fabs(x_new - 15);
  assert(difference < 0.0000001);

  /*testing getYCoordinate*/
  a.y_coor = 100;
  y_new = getYCoordinate (&a, 47, 1.745329252);
  difference = fabs(y_new - 108.1614644);
  assert(difference < 0.0000001);

  a.y_coor = 999;
  y_new = getYCoordinate (&a, 123, 3.839724354);
  difference = fabs(y_new - 1093.223467);
  assert(difference < 0.000001);

  a.y_coor = 150;
  y_new = getYCoordinate (&a, -200, 1.308996939);
  difference = fabs(y_new - 201.763809);
  assert(difference < 0.000001);
}
