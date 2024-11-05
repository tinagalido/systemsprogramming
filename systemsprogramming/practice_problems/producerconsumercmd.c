/*
 * MAX_BUFFER_SIZE is the maximum number of items in the buffer
 * numberOfUnits is the current number of items in the buffer
 * REQUIREMENTS:
 * (1) numberOfUnits must be protected by mutual exclusion, AND
 * (2) numberOfUnits cannot be negative, i.e., no underflow, AND
 * (3) numberOfUnits cannot be greater than MAX_BUFFER_SIZE, i.e., no overflow
 * 
 * Compile by: cc producerconsumercmd.c -o producerconsumercmd -pthread
 * Run by: ./producerconsumercmd <numberOfProducerThreads> <sleepOfProducer>
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER_SIZE 5
#define TRUE 1
#define FALSE 0

volatile int numberOfUnits = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct ProducerValues {
    int noOfThreads;
    int sleepInSeconds;
};

void* producer(void* input)
{
    int status, index;
    struct ProducerValues* producerValuesObj = (struct ProducerValues*)input;
    int actualNoOfThreads = producerValuesObj->noOfThreads;
    int actualSleepInSeconds = producerValuesObj->sleepInSeconds;

    for (index = 0; index < actualNoOfThreads; index++)   //while (TRUE)
    {
        status = pthread_mutex_lock(&mutex);
        if (status != 0)
        {
            printf("producer failed to lock mutex.\n");
            exit(EXIT_FAILURE);
        }
        
        if (numberOfUnits < MAX_BUFFER_SIZE)
        {
            numberOfUnits++;        /* Critical section */
            printf("producer increased numberOfUnits to %d\n", numberOfUnits);
        }
        else
        {
            printf("producer is waiting.\n");
        }

        status = pthread_mutex_unlock(&mutex);
        if (status != 0)
        {
            printf("producer failed to unlock mutex.\n");
            exit(EXIT_FAILURE);
        }

        sleep(actualSleepInSeconds);
    }

    return NULL;
}

void* consumer(void* input)
{
    int status;

    while (TRUE)
    {
        status = pthread_mutex_lock(&mutex);
        if (status != 0)
        {
            printf("consumer failed to lock mutex.\n");
            exit(EXIT_FAILURE);
        }
        
        if (numberOfUnits > 0)
        {
            numberOfUnits--;    /* Consuming happens here */
            printf("consumer decreased numberOfUnits to %d\n", numberOfUnits);
        }
        else
        {
            printf("consumer is waiting.\n");
        }

        status = pthread_mutex_unlock(&mutex);
        if (status != 0)
        {
            printf("consumer failed to unlock mutex.\n");
            exit(EXIT_FAILURE);
        }

        sleep(2);
    }
}


int main(int argc, char* argv[])
{
    pthread_t thread1, thread2;
    void* result;
    int status;
    int numberOfThreads, sleepOfProducer;
    struct ProducerValues producerValues;

    /* Error checking */
    if (argc != 3 ||
        strcmp(argv[1],"-h") == 0 ||
        strcmp(argv[1],"--help") == 0
        )
    {
        printf("Usage: %s <numberOfProducerThreads> <sleepOfProducer>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    numberOfThreads = atoi(argv[1]);
    sleepOfProducer = atoi(argv[2]);
    if (numberOfThreads < 1 || sleepOfProducer < 1)
    {
        printf("The <numberOfProducerThreads> OR <sleepOfProducer> should be a positive integer that is more than 0.\n");
        exit(EXIT_FAILURE);
    }
    
    /* Set the values to struct object before creating thread */
    producerValues.noOfThreads = numberOfThreads;
    producerValues.sleepInSeconds = sleepOfProducer;
    /* Create the producer thread */
    status = pthread_create(&thread1, NULL, producer,(void*)&producerValues);
    if (status != 0)
    {
        printf("Failed to create the producer thread.\n");
        exit(EXIT_FAILURE);
    }

    /* Create the consumer thread */
    status = pthread_create(&thread2, NULL, consumer, NULL);
    if (status != 0)
    {
        printf("Failed to create the consumer thread.\n");
        exit(EXIT_FAILURE);
    }

    /* Wait for the producer thread to complete */
    status = pthread_join(thread1, &result);
    if (status != 0)
    {
        printf("Failed to join the producer thread.\n");
        exit(EXIT_FAILURE);
    }

    /* Wait for the consumer thread to complete */
    status = pthread_join(thread2, &result);
    if (status != 0)
    {
        printf("Failed to join the consumer thread.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}