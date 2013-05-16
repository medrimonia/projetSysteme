#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

/* Ce test effectue en parallèle deux boucles incrémentant i de 100
 * le résultat doit être 200, cela permet de vérifier que l'on peut bien
 * avoir de la mémoire partagée.
 */

#define NB_ITERATIONS 1000 * 1000 * 200
#define NB_THREADS 1

static void * loop(void * thread_no)
{
    int k;
    int i=0;
    for (k = 0; k < NB_ITERATIONS / NB_THREADS; k++){
        i++;
    }
    return NULL;
}

int main(int argc, char *argv[])
{

    pthread_t th[NB_THREADS];
    int id[NB_THREADS];
    int err;

    int i;
    for (i = 0; i < NB_THREADS; i++){
        err = pthread_create(&th[i], NULL, loop, &id[i]);
        assert(!err);
    }

    for (i = 0; i < NB_THREADS; i++){
        err = pthread_join(th[i], NULL);
        assert(!err);
    }

    return 0;
}
