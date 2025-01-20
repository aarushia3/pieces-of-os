#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // For rand() and srand()
#include <time.h>   // For time()

pthread_mutex_t forks[5];
// Fork 0: left for P0, right for P1
// Fork 1: left for P1, right for P2
// Fork 2: left for P2, right for P3
// Fork 3: left for P3, right for P4
// Fork 4: left for P4, right for P0

void think() {
    // Simulate thinking
    int think_time = rand() % 3 + 1; // Random time between 1 and 3 seconds
    sleep(think_time);
    return;
}

void eat() {
    // Simulate eating
    int eat_time = rand() % 3 + 1; // Random time between 1 and 3 seconds
    sleep(eat_time);
    return;
}

void pickup(int fork) {
    pthread_mutex_lock(forks + fork);
    return;
}

void putdown(int fork) {
    pthread_mutex_unlock(forks + fork);
    return;
}

void *philosopher(void *arg) {
    int phil_id = *((int *)arg);
    // for (int i = 0; i < 5; i ++) {
        int left_fork = phil_id;
        int right_fork = (phil_id + 5 - 1) % 5;   // 0 1 2 3 4 => 4 0 1 2 3

        printf("Philosopher %d is thinking.\n", phil_id);
        think();
        printf("Philosopher %d is hungry.\n", phil_id);

        // Ensure in-order locking to prevent deadlocks
        if (left_fork < right_fork) {
            pickup(left_fork);
            pickup(right_fork);
        } else {
            pickup(right_fork);
            pickup(left_fork);
        }

        printf("Philosopher %d is eating.\n", phil_id);
        eat();
        printf("Philosopher %d has finished eating.\n", phil_id);

        putdown(left_fork); // Put down left fork
        putdown(right_fork); // Put down right fork
    // }
    return NULL;
}

int main() {
    srand(time(NULL)); // Seed the random number generator

    // initialize locks
    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(forks + i, NULL);
    }

    // each thread is a philosopher
    pthread_t philosophers[5];
    int phil_ids[5];

    for (int i = 0; i < 5; i++) {
        phil_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &phil_ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < 5; i++) {
        pthread_mutex_destroy(forks + i);
    }

    printf("All philosophers have finished eating.\n");
    return 0;
}