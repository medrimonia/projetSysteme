#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/times.h>

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
    
    struct timeval tim;
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

    loop();

    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
    printf("%lf secs\n",t2 - t1);

    return 0;
}
