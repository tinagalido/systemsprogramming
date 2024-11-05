/**
 * Goals:
 * Main thread will create two child threads
 * Each child thread will increment a global variable
 * named count (initialize to 0) n times and exit
 * Main thread will wait for the child threads to complete
 * Then, it will print the final value of the global variable
 * and exit
 * 
 * Compile using: cc simplethreads.c -o simplethreads -pthread
 * Run using: ./simplethreads
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>     /* This is for the POSIX threads */
#include <string.h>

volatile int count = 0;  /* Global variable */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* incrementThread(void* input)   /* Starting point of each child threads */
{
    int index;
    int* actualInput = (int*)input;
    int numIncrements = *actualInput;
    int status;

    for (index = 0; index < numIncrements; index++)
    {
        status = pthread_mutex_lock(&mutex);
        if (status != 0)
        {
            printf("Failed to lock mutex.\n");
            exit(EXIT_FAILURE);
        }
        count++;            /* Critical section*/

        status = pthread_mutex_unlock(&mutex);
        if (status != 0)
        {
            printf("Failed to unlock mutex.\n");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

int main (int argc, char* argv[])
{
    pthread_t childThread1, childThread2;
    void* threadResult1 = NULL;
    void* threadResult2 = NULL;
    int status; 
    int numOfIncrementsPerThread = 0;

    if (argc != 2 ||
        strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0)
    {
        printf("Usage: %s numOfIncrementsPerThread\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numOfIncrementsPerThread = atoi(argv[1]);
    if (numOfIncrementsPerThread <= 0)
    {
        printf("Number of threads must be positive.\n");
        exit(EXIT_FAILURE);
    }
    status = pthread_create(
                &childThread1, 
                NULL, 
                incrementThread, 
                (void*)&numOfIncrementsPerThread);

    if (status != 0)
    {
        printf("Failed to create child thread.\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_create(
                &childThread2, 
                NULL, 
                incrementThread, 
                (void*)&numOfIncrementsPerThread);
                
    if (status != 0)
    {
        printf("Failed to create child thread.\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_join(childThread1, &threadResult1);
    if (status != 0)
    {
        printf("Failed to join child threads.\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_join(childThread2, &threadResult2);
    if (status != 0)
    {
        printf("Failed to join child threads.\n");
        exit(EXIT_FAILURE);
    }

    printf("Final value of count :%d\n", count);
    exit(EXIT_SUCCESS);

}

