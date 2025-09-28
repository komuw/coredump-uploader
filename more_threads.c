// crash_core.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>

#define N_THREADS 2 // 3 threads

void *worker(void *arg)
{
    long id = (long)arg;
    printf("Thread %ld started (pthread id %lu)\n", id, (unsigned long)pthread_self());

    // Keep the thread alive for a bit so the crash is observable in logs
    for (int i = 0; i < 10; ++i)
    {
        printf("Thread %ld heartbeat %d\n", id, i);
        sleep(1);
    }

    printf("Thread %ld exiting normally\n", id);
    return NULL;
}

void *crasher(void *arg)
{
    long id = (long)arg;
    printf("Crasher thread %ld started and will crash in 3s...\n", id);
    sleep(3);

    // Intentionally cause a segfault by writing to NULL.
    // Use volatile to avoid compiler optimizations.
    volatile int *p = (volatile int *)0;
    *p = 42; // SIGSEGV -> should generate a core dump if permitted
    // not reached
    return NULL;
}

int main(void)
{
    // Allow the process to create core files of unlimited size
    struct rlimit rl;
    rl.rlim_cur = RLIM_INFINITY;
    rl.rlim_max = RLIM_INFINITY;
    if (setrlimit(RLIMIT_CORE, &rl) != 0)
    {
        perror("setrlimit(RLIMIT_CORE)");
        // continue anyway; environment may still allow core
    }
    else
    {
        printf("Set RLIMIT_CORE to unlimited\n");
    }

    pthread_t th[N_THREADS];
    int rc;

    // start N_THREADS-1 normal workers
    for (long i = 0; i < N_THREADS - 1; ++i)
    {
        rc = pthread_create(&th[i], NULL, worker, (void *)i);
        if (rc)
        {
            fprintf(stderr, "Error creating worker thread %ld: %d\n", i, rc);
            exit(1);
        }
    }

    // start the crasher thread last
    rc = pthread_create(&th[N_THREADS - 1], NULL, crasher, (void *)(N_THREADS - 1));
    if (rc)
    {
        fprintf(stderr, "Error creating crasher thread: %d\n", rc);
        exit(1);
    }

    // join threads (in practice process will likely die when crasher faults)
    for (int i = 0; i < N_THREADS; ++i)
    {
        pthread_join(th[i], NULL);
    }

    printf("Main exiting\n");
    return 0;
}

/*
gcc -g more_threads.c -o more_threads
ulimit -c unlimited && ./more_threads
*/