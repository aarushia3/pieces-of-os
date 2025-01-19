#ifndef RW_LOCK_H
#define RW_LOCK_H

#include <pthread.h>
#include <errno.h>

/**
 * Readers-writer lock struct
 * Contains a reader count, a lock for the reader count, and a lock for the shared resource
 */
typedef struct rw_lock_s {
    int reader_count;
    pthread_mutex_t reader_count_lock;
    pthread_mutex_t resource_lock;
} rw_lock_t;

/**
 * Initialize a rw_lock_t struct.
 * Returns 0 on success.
 */
int rw_lock_init(rw_lock_t * lck);

/**
 * Destroy a rw_lock_t struct.
 * Returns 0 on success.
 */
int rw_lock_destroy(rw_lock_t * lck);

/**
 * Reader lock function.
 * Acquires the lock for the shared resource if it is the first reader.
 * All other readers increment the reader count.
 * Returns 0 on success.
 */
void rw_lock_rlock(rw_lock_t * lck);

/**
 * Try a reader lock. Bail out if lock is owned by a writer
 * Tries to acquire the resource lock. If it is already owned by a reader, increment the reader count.
 * If the lock is not acquired and the reader count is 0, a writer must be holding the lock. So, bail out.
 * Returns 1 on success, -EBUSY if the lock is owned by a writer.
 */
int rw_lock_try_rlock(rw_lock_t * lck);

/**
 * Reader unlock function.
 * Decrements the reader count and releases the lock for the shared resource if it is the last reader.
 * Returns 0 on success.
 */
int rw_lock_runlock(rw_lock_t * lck);

/**
 * Writer lock function.
 * Acquires the lock for the shared resource.
 * Returns 0 on success.
 */
void rw_lock_wlock(rw_lock_t * lck);

/**
 * Try to acquire the lock for the shared resource.
 * Returns 1 on success, -EBUSY if the lock is already owned by a reader or writer.
 */
int rw_lock_try_wlock(rw_lock_t * lck);

/**
 * Writer unlock function.
 * Releases the lock for the shared resource.
 * Returns 0 on success.
 */
int rw_lock_wunlock(rw_lock_t * lck);

#endif // RW_LOCK_H
