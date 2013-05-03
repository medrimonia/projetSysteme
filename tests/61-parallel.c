#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "thread.h"

/* Ce test effectue en parallèle deux boucles incrémentant i de 100
 * le résultat doit être 200, cela permet de vérifier que l'on peut bien
 * avoir de la mémoire partagée.
 */

int i;
mutex_p mutex;

static void * inc(void * thread_no)
{
    int thread_number = *((int *)thread_no);
    int k,j;
    for (k = 0; k < 10; k++){
        thread_mutex_lock(mutex);
        for (j = 0; j < 10; j++){
            i++;
            thread_yield();
        }
        printf("%d : %d\n",thread_number,i);
        thread_mutex_unlock(mutex);
        thread_yield();
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    i = 0;

    thread_t th, th2;
    int err;
    printf("initializing mutex\n");
    mutex = thread_mutex_init();

    int id = 1;
    int id2 = 2;

    err = thread_create(&th, inc, &id);
    assert(!err);
    err = thread_create(&th2, inc, &id2);
    assert(!err);

    err = thread_join(th, NULL);
    assert(!err);
    err = thread_join(th2, NULL);
    assert(i == 200);

    thread_mutex_destroy(mutex);

    return 0;
}
