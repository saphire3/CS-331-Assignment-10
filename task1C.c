#include <stdio.h>
#include <pthread.h>

long long counter = 0;
int N, M;
pthread_spinlock_t sp;

void* run(void* a) {
    for (int i = 0; i < M; i++) {
        pthread_spin_lock(&sp);
        counter++;
        pthread_spin_unlock(&sp);
    }
    return NULL;
}

int main(int argc, char** argv) {
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    pthread_spin_init(&sp, PTHREAD_PROCESS_PRIVATE);
    pthread_t th[N];
    for (int i = 0; i < N; i++) pthread_create(&th[i], NULL, run, NULL);
    for (int i = 0; i < N; i++) pthread_join(th[i], NULL);
    printf("expected=%lld actual=%lld\n", (long long)N*M, counter);
    pthread_spin_destroy(&sp);
}

