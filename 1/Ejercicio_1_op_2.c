#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SEQUENCE_LENGTH 16

//No cumple con la consigna (creo), pero esta hecho de una forma mas simple como hizo el profe en clase el de las moleculas de agua
sem_t sem_a, sem_b, sem_c; //Semaforos con variables globales
int count = 0; //Contador de letras de secuencia como variable global

void* print_A(void* arg) {
    while (count < SEQUENCE_LENGTH) {
        sem_wait(&sem_a);
        if (count < SEQUENCE_LENGTH) {
            printf("A");
            fflush(stdout);
            count++;
            sem_post(&sem_b);
        }
    }
    return NULL;
}

void* print_B(void* arg) {
    while (count < SEQUENCE_LENGTH) {
        sem_wait(&sem_b);
        if (count < SEQUENCE_LENGTH) {
            printf("B");
            fflush(stdout);
            count++;
            if (count % 7 == 0) {
                sem_post(&sem_c);
            } else {
                sem_post(&sem_a);
            }
        }
    }
    return NULL;
}

void* print_C(void* arg) {
    while (count < SEQUENCE_LENGTH) {
        sem_wait(&sem_c);
        if (count < SEQUENCE_LENGTH) {
            printf("C");
            fflush(stdout);
            count++;
            sem_post(&sem_a);
        }
    }
    return NULL;
}

int main() {
    pthread_t thread_a, thread_b, thread_c;

    sem_init(&sem_a, 0, 1);
    sem_init(&sem_b, 0, 0);
    sem_init(&sem_c, 0, 0);

    pthread_create(&thread_a, NULL, print_A, NULL);
    pthread_create(&thread_b, NULL, print_B, NULL);
    pthread_create(&thread_c, NULL, print_C, NULL);

    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);
    pthread_join(thread_c, NULL);

    sem_destroy(&sem_a);
    sem_destroy(&sem_b);
    sem_destroy(&sem_c);

    printf("\n");
    return 0;
}