#include <stdio.h>
#include <pthread.h>

long long counter = 0;
int N, M;
pthread_mutex_t mx;

void* run(void* a) {
    for (int i = 0; i < M; i++) {
        pthread_mutex_lock(&mx);
        counter++;
        pthread_mutex_unlock(&mx);
    }
    return NULL;
}

int main(int argc, char** argv) {
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    pthread_mutex_init(&mx, NULL);
    pthread_t th[N];
    for (int i = 0; i < N; i++) pthread_create(&th[i], NULL, run, NULL);
    for (int i = 0; i < N; i++) pthread_join(th[i], NULL);
    printf("expected=%lld actual=%lld\n", (long long)N*M, counter);
    pthread_mutex_destroy(&mx);
}

