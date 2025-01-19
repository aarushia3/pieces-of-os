#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "mutex.h"

#define THREAD_COUNT 6

void* increment_task(void* arg) {
    for (int i = 0; i < 1000; i++) {
        increment_counter();
    }
    return arg;
}

void* decrement_task(void* arg) {
    for (int i = 0; i < 1000; i++) {
        decrement_counter();
    }
    return arg;
}

int main() {
    pthread_t threads[THREAD_COUNT];

    initialize_mutex();

    for (int i = 0; i < THREAD_COUNT / 2; i++) {
        pthread_create(&threads[i], NULL, increment_task, NULL);
    }

    for (int i = THREAD_COUNT / 2; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, decrement_task, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final Counter Value: %d\n", get_counter_value());

    destroy_mutex();
    return 0;
}
