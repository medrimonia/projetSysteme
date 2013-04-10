#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#define NB_THREADS 8

ucontext_t uc[NB_THREADS];

int current_thread;

void func(int numero){
  int i;
  for (i = 0; i < 10; i++){
    printf("%d: j'affiche le numéro %d\n", numero, i);
    swapcontext(&uc[numero], &uc[(numero + 1) % NB_THREADS]);
  }
}

int main() {
  int i;
  for (i = 0; i < NB_THREADS; i++){
    getcontext(&uc[i]);
    uc[i].uc_stack.ss_size = 64*1024;
    uc[i].uc_stack.ss_sp = malloc(uc[0].uc_stack.ss_size);
    uc[i].uc_link = NULL;
    makecontext(&uc[i], (void (*)(void)) func, 1, i);
  }

  setcontext(&uc[0]);

  return 0;
}
