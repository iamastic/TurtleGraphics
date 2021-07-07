#include "stack.h"
#include "stackSpecific.h"

#define DOTFILE 5000

stack* stack_init(void)
{
   stack *s = (stack*) ncalloc(sizeof(stack), 1);
   /* Some implementations would allow you to pass
      a hint about the initial size of the stack */
   s->a = (stacktype*) ncalloc(sizeof(stacktype), FIXEDSIZE);
   s->size = 0;
   s->capacity= FIXEDSIZE;
   return s;
}

void stack_push(stack* s, stacktype d)
{
   if(s==NULL){
       return;
   }
   s->a[s->size] = d;
   s->size = s->size + 1;
   if(s->size >= s->capacity){
      s->a = (stacktype*) nremalloc(s->a, sizeof(stacktype)*s->capacity*SCALEFACTOR);
      s->capacity = s->capacity*SCALEFACTOR;
   }
}

bool stack_pop(stack* s, stacktype* d)
{
   if((s == NULL) || (s->size < 1)){
      return false;
   }
   s->size = s->size - 1;
   *d = s->a[s->size];
   return true;
}

int stack_size(stack* s)
{
  return s->size;
}

bool stack_free(stack* s)
{
   if(s==NULL){
      return true;
   }
   free(s->a);
   free(s);
   return true;
}
