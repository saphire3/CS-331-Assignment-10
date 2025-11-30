#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 8

int buffer[BUFFER_SIZE];
int in_pos = 0, out_pos = 0;

sem_t empty_slots, full_slots;
pthread_mutex_t buffer_mutex;

int P, C, K;
int produced = 0, consumed = 0;
pthread_mutex_t count_mx;

void* producer(void* a) {
    while (1) {
        pthread_mutex_lock(&count_mx);
        if (produced >= P * K) {
            pthread_mutex_unlock(&count_mx);
            return NULL;
        }
        produced++;
        int item = produced;
        pthread_mutex_unlock(&count_mx);

        sem_wait(&empty_slots);
        pthread_mutex_lock(&buffer_mutex);
        buffer[in_pos] = item;
        in_pos = (in_pos + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full_slots);
    }
}

void* consumer(void* a) {
    while (1) {
        pthread_mutex_lock(&count_mx);
        if (consumed >= P * K) {
            pthread_mutex_unlock(&count_mx);
            return NULL;
        }
        consumed++;
        pthread_mutex_unlock(&count_mx);

        sem_wait(&full_slots);
        pthread_mutex_lock(&buffer_mutex);
        int x = buffer[out_pos];
        out_pos = (out_pos + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&empty_slots);
    }
}

int main(int argc, char** argv) {
    P = atoi(argv[1]);
    C = atoi(argv[2]);
    K = atoi(argv[3]);

    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);
    pthread_mutex_init(&buffer_mutex, NULL);
    pthread_mutex_init(&count_mx, NULL);

    pthread_t prod[P], cons[C];
    for (int i = 0; i < P; i++) pthread_create(&prod[i], NULL, producer, NULL);
    for (int i = 0; i < C; i++) pthread_create(&cons[i], NULL, consumer, NULL);

    for (int i = 0; i < P; i++) pthread_join(prod[i], NULL);
    for (int i = 0; i < C; i++) pthread_join(cons[i], NULL);

    printf("done\n");

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    pthread_mutex_destroy(&buffer_mutex);
    pthread_mutex_destroy(&count_mx);
}

