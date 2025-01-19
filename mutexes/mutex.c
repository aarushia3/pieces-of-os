#include "mutex.h"

int counter = 0;        // shared resource
pthread_mutex_t lock;   // lock to protect above counter

void initialize_mutex() {
    pthread_mutex_init(&lock, NULL);
}

void destroy_mutex() {
    pthread_mutex_destroy(&lock);
}

void increment_counter() {
    // increment counter in a critical section
    pthread_mutex_lock(&lock);
    counter++;
    pthread_mutex_unlock(&lock);
}

void decrement_counter() {
    // decrement counter in a critical section
    pthread_mutex_lock(&lock);
    counter--;
    pthread_mutex_unlock(&lock);
}

int get_counter_value() {
    // return counter value in a critical section
    pthread_mutex_lock(&lock);
    int value = counter;
    pthread_mutex_unlock(&lock);
    return value;
}
