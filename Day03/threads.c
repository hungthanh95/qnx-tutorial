#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

void* worker(void* arg) {
    char* name = (char*)arg;
    for (int i = 0; i < 5; i++) {
        printf("Worker %s is working... iteration %d\n", name, i + 1);
        sleep(1); // Simulate work
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    struct sched_param param;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    // Create two worker threads
    param.sched_priority = 20;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&thread1, &attr, worker, "HIGH");

    param.sched_priority = 10;
    pthread_attr_setschedparam(&attr, &param);
    pthread_create(&thread2, &attr, worker, "LOW");

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("All workers have finished.\n");
    return 0;
}