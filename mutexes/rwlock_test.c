#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "rwlock.h"

rw_lock_t lock;

void *reader_thread(void *arg) {
    rw_lock_rlock(&lock);
    printf("Reader %ld is reading\n", (long)arg);
    rw_lock_runlock(&lock);
    printf("Reader %ld is done\n", (long)arg);
    return NULL;
}

void *writer_thread(void *arg) {
    rw_lock_wlock(&lock);
    printf("Writer %ld is writing\n", (long)arg);
    rw_lock_wunlock(&lock);
    printf("Writer %ld is done\n", (long)arg);
    return NULL;
}

void *slow_writer_thread(void *arg) {
    rw_lock_wlock(&lock);
    printf("Writer %ld is writing\n", (long)arg);
    sleep(2);
    rw_lock_wunlock(&lock);
    printf("Writer %ld is done\n", (long)arg);
    return NULL;
}

void *slow_reader_thread(void *arg) {
    rw_lock_rlock(&lock);
    printf("Reader %ld is reading\n", (long)arg);
    sleep(2);
    rw_lock_runlock(&lock);
    printf("Reader %ld is done\n", (long)arg);
    return NULL;
}

void *reader_thread_try(void *arg) {
    int result = rw_lock_try_rlock(&lock);
    if (result == 1) {
        printf("Reader %ld successfully acquired the lock\n", (long)arg);
        rw_lock_runlock(&lock);
    } else {
        printf("Reader %ld could not acquire the lock (EBUSY)\n", (long)arg);
    }
    return NULL;
}

void *writer_thread_try(void *arg) {
    int result = rw_lock_try_wlock(&lock);
    if (result == 0) {
        printf("Writer %ld acquired the lock\n", (long)arg);
        rw_lock_wunlock(&lock);
    } else {
        printf("Writer %ld could not acquire the lock (EBUSY)\n", (long)arg);
    }
    return NULL;
}

int main() {
    printf("------------Testcase 1: Basic Reader Lock.------------\n");
    pthread_t readers1[5];
    rw_lock_init(&lock);

    for (long i = 0; i < 5; i++) {
        pthread_create(&readers1[i], NULL, reader_thread, (void *)i);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(readers1[i], NULL);
    }

    rw_lock_destroy(&lock);

    printf("------------Testcase 2: One Writer and Multiple Readers.------------\n");
    pthread_t readers2[3], writers2[2];
    rw_lock_init(&lock);

    pthread_create(&writers2[0], NULL, writer_thread, (void *)0);

    for (long i = 0; i < 3; i++) {
        pthread_create(&readers2[i], NULL, reader_thread, (void *)i);
    }

    pthread_join(writers2[0], NULL); // Writer should complete before reading starts
    for (int i = 0; i < 3; i++) {
        pthread_join(readers2[i], NULL);
    }

    rw_lock_destroy(&lock);

    printf("------------Testcase 3: Multiple Writers.------------\n");
    pthread_t writers3[3];
    rw_lock_init(&lock);

    for (long i = 0; i < 3; i++) {
        pthread_create(&writers3[i], NULL, writer_thread, (void *)i);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(writers3[i], NULL);
    }

    rw_lock_destroy(&lock);

    printf("------------Testcase 4: Try Reader Lock.------------\n");
    pthread_t writer, readers4[2];
    rw_lock_init(&lock);

    pthread_create(&writer, NULL, slow_writer_thread, NULL);
    sleep(1);

    for (long i = 0; i < 2; i++) {
        pthread_create(&readers4[i], NULL, reader_thread_try, (void *)i);
    }

    pthread_join(writer, NULL);
    for (int i = 0; i < 2; i++) {
        pthread_join(readers4[i], NULL);
    }

    rw_lock_destroy(&lock);

    printf("------------Testcase 5: Try Writer Lock.------------\n");
    pthread_t reader5, writer5;
    rw_lock_init(&lock);

    pthread_create(&reader5, NULL, slow_reader_thread, (void *)0);
    sleep(1);

    pthread_create(&writer5, NULL, writer_thread_try, NULL);

    pthread_join(reader5, NULL);
    pthread_join(writer5, NULL);

    rw_lock_destroy(&lock);

    printf("Check if all tests have passed :).");
    
    return 0;
}
