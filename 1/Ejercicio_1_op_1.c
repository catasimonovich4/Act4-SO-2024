#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define SEQUENCE_LENGTH 16

void* print_A(void* arg);
void* print_B(void* arg);
void* print_C(void* arg);
typedef struct {
    sem_t* sem_a;
    sem_t* sem_b;
    sem_t* sem_c;
    int* count;
} thread_args;

int main() {
    pthread_t thread_a, thread_b, thread_c;
    sem_t sem_a, sem_b, sem_c;
    int count = 0;
    sem_init(&sem_a, 0, 1);
    sem_init(&sem_b, 0, 0);
    sem_init(&sem_c, 0, 0);
    thread_args args = {&sem_a, &sem_b, &sem_c, &count};
    pthread_create(&thread_a, NULL, print_A, &args);
    pthread_create(&thread_b, NULL, print_B, &args);
    pthread_create(&thread_c, NULL, print_C, &args);
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);
    pthread_join(thread_c, NULL);
    sem_destroy(&sem_a);
    sem_destroy(&sem_b);
    sem_destroy(&sem_c);
    printf("\n");
    return 0;
}

void* print_A(void* arg) {
    thread_args* args = (thread_args*)arg;
    while (*(args->count) < SEQUENCE_LENGTH) {
        sem_wait(args->sem_a);
        if (*(args->count) < SEQUENCE_LENGTH) {
            printf("A");
            fflush(stdout);
            (*(args->count))++;
            if (*(args->count) == 7) {
                sem_post(args->sem_c);
            } else {
                sem_post(args->sem_b);
            }
        }
    }
    return NULL;
}

void* print_B(void* arg) {
    thread_args* args = (thread_args*)arg;
    while (*(args->count) < SEQUENCE_LENGTH) {
        sem_wait(args->sem_b);
        if (*(args->count) < SEQUENCE_LENGTH - 1) {  
            printf("B");
            fflush(stdout);
            (*(args->count))++;
            sem_post(args->sem_a);
        } else {
            sem_post(args->sem_c);  
        }
    }
    return NULL;
}

void* print_C(void* arg) {
    thread_args* args = (thread_args*)arg;
    while (*(args->count) < SEQUENCE_LENGTH) {
        sem_wait(args->sem_c);
        if (*(args->count) < SEQUENCE_LENGTH) {
            printf("C");
            fflush(stdout);
            (*(args->count))++;
        }
        if (*(args->count) < SEQUENCE_LENGTH) {
            sem_post(args->sem_a);
        }
    
    }
    return NULL;
}