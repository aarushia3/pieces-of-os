#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

void initialize_mutex();
void destroy_mutex();
void increment_counter();
void decrement_counter();
int get_counter_value();

#endif // MUTEX_H