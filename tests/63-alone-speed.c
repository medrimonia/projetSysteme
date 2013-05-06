#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "thread.h"

/* Ce test effectue en parallèle deux boucles incrémentant i de 100
 * le résultat doit être 200, cela permet de vérifier que l'on peut bien
 * avoir de la mémoire partagée.
 */

#define NB_ITERATIONS 1000 * 1000 * 100
#define NB_THREADS 4

void loop()
{
    int k;
    int i=0;
    for (k = 0; k < NB_ITERATIONS * NB_THREADS; k++){
        i++;
    }
}

int main(int argc, char *argv[])
{
    
    loop();

    return 0;
}
