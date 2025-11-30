#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

long long balance = 0;
int depN, wdrN, loops;
int use_spin = 0;
int long_cs = 0;

pthread_mutex_t mx;
pthread_spinlock_t sp;

static inline void lock() {
    if (use_spin) pthread_spin_lock(&sp);
    else pthread_mutex_lock(&mx);
}

static inline void unlock() {
    if (use_spin) pthread_spin_unlock(&sp);
    else pthread_mutex_unlock(&mx);
}

void* deposit(void* a) {
    for (int i = 0; i < loops; i++) {
        lock();
        balance++;
        if (long_cs) usleep(100);
        unlock();
    }
    return NULL;
}

void* withdraw(void* a) {
    for (int i = 0; i < loops; i++) {
        lock();
        balance--;
        if (long_cs) usleep(100);
        unlock();
    }
    return NULL;
}

int main(int argc, char** argv) {
    use_spin = !strcmp(argv[1], "spin");
    long_cs = !strcmp(argv[2], "long");
    depN = atoi(argv[3]);
    wdrN = atoi(argv[4]);
    loops = atoi(argv[5]);

    if (use_spin) pthread_spin_init(&sp, 0);
    else pthread_mutex_init(&mx, NULL);

    pthread_t dep[depN], wdr[wdrN];
    for (int i = 0; i < depN; i++) pthread_create(&dep[i], NULL, deposit, NULL);
    for (int i = 0; i < wdrN; i++) pthread_create(&wdr[i], NULL, withdraw, NULL);

    for (int i = 0; i < depN; i++) pthread_join(dep[i], NULL);
    for (int i = 0; i < wdrN; i++) pthread_join(wdr[i], NULL);

    printf("%lld\n", balance);

    if (use_spin) pthread_spin_destroy(&sp);
    else pthread_mutex_destroy(&mx);
}

