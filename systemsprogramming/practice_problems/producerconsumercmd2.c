/*
 * MAX_BUFFER_SIZE is the maximum number of items in the buffer
 * numberOfUnits is the current number of items in the buffer
 * REQUIREMENTS:
 * (1) numberOfUnits must be protected by mutual exclusion
 * (2) numberOfUnits cannot be negative, i.e., no underflow
 * (3) numberOfUnits cannot be greater than MAX_BUFFER_SIZE, i.e., no overflow
 * 
 * Compile by: cc producerconsumercmd2.c -o producerconsumercmd2 -pthread
 * Run by: ./producerconsumercmd2 <numberOfProducerThreads> <sleepOfProducer>
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

/* The producer will use this to tell the consumer that there is something
   available to consume
*/
pthread_cond_t unitAvailable = PTHREAD_COND_INITIALIZER;

/* The consumer will use this to tell the producer that there is space
    available in the buffer
*/
pthread_cond_t spaceAvailable = PTHREAD_COND_INITIALIZER;

struct ProducerValues {
    int noOfThreads;
    int sleepInSeconds;
};

void* producer(void* input)
{
    int status,index;
    struct ProducerValues* producerValuesObj = (struct ProducerValues*)input;
    int actualNoOfThreads = producerValuesObj->noOfThreads;
    int actualSleepInSeconds = producerValuesObj->sleepInSeconds;

    for (index = 0; index < actualNoOfThreads; index++)     //while (TRUE)
    {
        status = pthread_mutex_lock(&mutex);
        if (status != 0)
        {
            printf("producer failed to lock mutex.\n");
            exit(EXIT_FAILURE);
        }
        
        /* Check if producer needs to wait */
        while (!(numberOfUnits < MAX_BUFFER_SIZE))
        {
            printf("producer is about to block\n");
            status = pthread_cond_wait(&spaceAvailable, &mutex);
            if (status != 0)
            {
                printf("producer failed to wait on spaceAvailable\n");
                exit(EXIT_FAILURE);
            }
        }

        numberOfUnits++;        /* Critical section */
        printf("producer increased numberOfUnits to %d\n", numberOfUnits);

        if (numberOfUnits == 1) /* Producer needs to wake up the consumer */
        {
            status = pthread_cond_signal(&unitAvailable);
            if (status != 0)
            {
                printf("producer failed to wake up consumer\n");
                exit(EXIT_FAILURE);
            }
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
        
        while (!(numberOfUnits > 0))
        {
            printf("consumer is about to block\n");
            status = pthread_cond_wait(&unitAvailable, &mutex);
            if (status != 0)
            {
                printf("consumer failed to wait on unitAvailable\n");
                exit(EXIT_FAILURE);
            }
        }


        numberOfUnits--;
        printf("consumer decreased numberOfUnits to %d\n", numberOfUnits);
        
        if (numberOfUnits == MAX_BUFFER_SIZE - 1) /* Consumer needs to wake up producer */
        {
            status = pthread_cond_signal(&spaceAvailable);
            if (status != 0)
            {
                printf("consumer failed to wake up producer.\n");
                exit(EXIT_FAILURE);
            }
        }

        status = pthread_mutex_unlock(&mutex);
        if (status != 0)
        {
            printf("consumer failed to unlock mutex.\n");
            exit(EXIT_FAILURE);
        }

        sleep(1);
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

    /* Set the values to struct object before creating */
    producerValues.noOfThreads = numberOfThreads;
    producerValues.sleepInSeconds = sleepOfProducer;

    /* Create the producer thread */
    status = pthread_create(&thread1, NULL, producer, (void*)&producerValues);
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