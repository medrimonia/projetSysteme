#include "thread.h"

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <valgrind/valgrind.h>

#define STACK_SIZE 64 * 1024
#define STATUS_TERMINATED 1

void initialize_thread_handler();

struct thread{
  ucontext_t context;
  int stack_id;
  int status;
  void * retval;
};

GList *threads = NULL;

/* L'identifiant du thread est mis à jour au fur et à mesure
 */
int current_thread = 0;
int nb_threads = 0;
int nb_threads_waiting_join = 0;
int next_thread_create = 0;
ucontext_t exit_context;
int exit_context_stack_id;

struct thread * add_thread(){
  struct thread *to_add = malloc(sizeof(struct thread));
  to_add->status = 0;
  to_add->retval = NULL;
  to_add->context.uc_link = NULL;
  threads = g_list_append(threads, &to_add);
  nb_threads++;
  next_thread_create++;
  return to_add;
}

// switch to next thread
struct thread * next_thread(){
  int next = current_thread;
  struct thread *next_running, *running;
  running = g_list_nth_data(threads, next);
  next_running = g_list_nth_data(threads, ++next);
  while (running != next_running) {
    if(running == NULL) {
      next_running = (struct thread*) g_list_first(threads);
    }
    else if ((next_running = (struct thread*) g_list_next(threads)) == NULL) {
      next_running = (struct thread*) g_list_first(threads);
    }
    if(next_running->status != STATUS_TERMINATED) {
      current_thread = g_list_index(threads, next_running);
      return next_running;
    }
  }
  return NULL;
}

void wrapper(void *(*func)(void *), void * funcarg){
  void * retval = func(funcarg);
  thread_exit(retval);
}
thread_t thread_self(){
  if (next_thread_create == 0){
    initialize_thread_handler();
  }
  return g_list_nth_data(threads, current_thread);
}

void initialize_thread_handler(){
  struct thread * this_thread = add_thread();
  getcontext(&exit_context);
  exit_context.uc_stack.ss_size = STACK_SIZE;
  exit_context.uc_stack.ss_sp = malloc(exit_context.uc_stack.ss_size);
  exit_context_stack_id =
    VALGRIND_STACK_REGISTER(exit_context.uc_stack.ss_sp,
        exit_context.uc_stack.ss_sp
        + exit_context.uc_stack.ss_size);
  exit_context.uc_link = NULL;
  makecontext(&exit_context,(void (*)(void)) thread_exit, 1, 0);
}

void end_thread_handling(){
  VALGRIND_STACK_DEREGISTER(exit_context_stack_id);
  free(exit_context.uc_stack.ss_sp);
  g_list_free(threads);
  nb_threads = 0;
  current_thread = 0;
  threads = NULL;
  next_thread_create = 0;
}

int thread_create(thread_t * newthread,
    void *(*func)(void *),
    void * funcarg){
  if (next_thread_create == 0){
    initialize_thread_handler();
  }
  struct thread * new_thread = add_thread();
  ucontext_t * new_context = &new_thread->context;
  getcontext(new_context);
  new_context->uc_stack.ss_size = STACK_SIZE;
  new_context->uc_stack.ss_sp = malloc(new_context->uc_stack.ss_size);
  new_thread->stack_id =
    VALGRIND_STACK_REGISTER(new_context->uc_stack.ss_sp,
        new_context->uc_stack.ss_sp
        + new_context->uc_stack.ss_size);
  new_context->uc_link = NULL;
  makecontext(new_context, (void (*)(void)) wrapper, 2, func, funcarg);
  *newthread = (void *)new_thread;
  return 0;
}

int thread_yield(){
  if (next_thread_create == 0){
    return 0;
  }
  struct thread * my_thread = thread_self();
  struct thread * next = next_thread();
  if (next != my_thread){
    swapcontext(&my_thread->context, &next->context);
  }
  return 0;
}

int thread_join(thread_t thread, void ** retval){
  struct thread * to_wait = (struct thread *) thread;
  while(to_wait->status != STATUS_TERMINATED){
    thread_yield();
  }
  nb_threads_waiting_join--;
  if (retval != NULL)
    *retval = to_wait->retval;
  free(to_wait->context.uc_stack.ss_sp);
  if (nb_threads == 1 && nb_threads_waiting_join == 0)
    end_thread_handling();
  return 0;
}

void thread_exit(void *retval){
  struct thread * my_thread = thread_self();
  my_thread->status = STATUS_TERMINATED;
  my_thread->retval = retval;
  VALGRIND_STACK_DEREGISTER(my_thread->stack_id);
  if (nb_threads == 1)
    exit(EXIT_SUCCESS);
  struct thread * next = next_thread();
  ucontext_t next_context = next->context;
  nb_threads--;
  nb_threads_waiting_join++;
  setcontext(&next_context);
  exit(EXIT_FAILURE);
}
