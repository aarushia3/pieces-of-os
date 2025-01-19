#include "rwlock.h"

int rw_lock_init(rw_lock_t * lck) {
    lck->reader_count = 0;
    pthread_mutex_init(&lck->reader_count_lock, NULL);
    pthread_mutex_init(&lck->resource_lock, NULL);
    return 0;
}

int rw_lock_destroy(rw_lock_t * lck) {
    pthread_mutex_destroy(&lck->reader_count_lock);
    pthread_mutex_destroy(&lck->resource_lock);
    return 0;
}

void rw_lock_rlock(rw_lock_t * lck) {
    pthread_mutex_lock(&lck->reader_count_lock);
    lck->reader_count++;
    if (lck->reader_count == 1) {
        pthread_mutex_lock(&lck->resource_lock);
    }
    pthread_mutex_unlock(&lck->reader_count_lock);
}

int rw_lock_try_rlock(rw_lock_t * lck) {
    pthread_mutex_lock(&lck->reader_count_lock);
    if (pthread_mutex_trylock(&lck->resource_lock) == 0 || lck->reader_count > 0) {
        lck->reader_count++;
        pthread_mutex_unlock(&lck->reader_count_lock);
        return 1;
    }
    pthread_mutex_unlock(&lck->reader_count_lock);
    return -EBUSY;
}

int rw_lock_runlock(rw_lock_t * lck) {
    pthread_mutex_lock(&lck->reader_count_lock);
    lck->reader_count--;
    if (lck->reader_count == 0) {
        pthread_mutex_unlock(&lck->resource_lock);
    }
    pthread_mutex_unlock(&lck->reader_count_lock);
    return 0;
}

void rw_lock_wlock(rw_lock_t * lck) {
    pthread_mutex_lock(&lck->resource_lock);
}

int rw_lock_try_wlock(rw_lock_t * lck) {
    return pthread_mutex_trylock(&lck->resource_lock);
}

int rw_lock_wunlock(rw_lock_t * lck) {
    pthread_mutex_unlock(&lck->resource_lock);
    return 0;
}
