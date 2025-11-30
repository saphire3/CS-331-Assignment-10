#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t printers;
pthread_mutex_t mx;
int inside = 0;
int N, K;

void* job(void* a) {
    long id = (long)a;

    sem_wait(&printers);

    pthread_mutex_lock(&mx);
    inside++;
    printf("Thread %ld printing... inside=%d\n", id, inside);
    pthread_mutex_unlock(&mx);

    usleep(100000);

    pthread_mutex_lock(&mx);
    inside--;
    pthread_mutex_unlock(&mx);

    sem_post(&printers);
    return NULL;
}

int main(int argc, char** argv) {
    N = atoi(argv[1]);
    K = atoi(argv[2]);

    sem_init(&printers, 0, K);
    pthread_mutex_init(&mx, NULL);

    pthread_t th[N];
    for (long i = 0; i < N; i++) pthread_create(&th[i], NULL, job, (void*)i);
    for (long i = 0; i < N; i++) pthread_join(th[i], NULL);

    sem_destroy(&printers);
    pthread_mutex_destroy(&mx);
}

