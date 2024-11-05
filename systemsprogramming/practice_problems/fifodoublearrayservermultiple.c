#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <pthread.h>

#define MAX_NUMBER 10

/* FIFO NAMES */
#define REQUEST_NAME "RequestFifo"
#define RESPONSE_NAME "ResponseFifo.%d"

#define RESPONSE_MAX_LENGTH 100
#define TRUE 1

/* Define protocol */
typedef struct 
{
    pid_t clientId;
    double randomDoubleList[MAX_NUMBER];
} Request;

typedef struct 
{
    double randomDoubleNumList[MAX_NUMBER];
    double average;
} Response;

/* Remove exit handler (request fifo)*/
void removeRequestFifo ()
{
    int status;

    status = unlink(REQUEST_NAME);
    if (status == -1)
    {
        printf("Error in removing request fifo.\n");
        
    }
}

void handleInterruptSignal(int)
{
    exit(EXIT_SUCCESS);
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* computeAverage(void* requestPointer)
{
    int status, index;
    int numWritten, responseFifoDescriptor;
    char responseFifoName[RESPONSE_MAX_LENGTH];
    Response response;
    double average;
    double sum = 0.0;

    Request request = *(Request*)requestPointer;

    for (index = 0; index < MAX_NUMBER; index++)
    {
        printf("Server received request %d:%lf\n", index, request.randomDoubleList[index]);
    }
    
    /* Process response */
    status = pthread_mutex_lock(&mutex);
    if (status != 0)
    {
        printf("Failed to lock mutex.\n");
        exit(EXIT_FAILURE);
    }

    for (index = 0; index < MAX_NUMBER; index++)
    {
        response.randomDoubleNumList[index] = request.randomDoubleList[index];
        sum = sum + request.randomDoubleList[index];
    }
    average = sum / MAX_NUMBER; 
    response.average = average;

    status = pthread_mutex_unlock(&mutex);
    if (status != 0)
    {
        printf("Failed to unlock mutex.\n");
        exit(EXIT_FAILURE);
    }

    /* Send response to correct client */
    // Name of response FIFO
    snprintf(responseFifoName, RESPONSE_MAX_LENGTH, RESPONSE_NAME, request.clientId);

    // Open the response FIFO
    responseFifoDescriptor = open(responseFifoName, O_WRONLY);
    if (responseFifoDescriptor == -1)
    {
        printf("Error in opening response fifo.\n");
        exit(EXIT_FAILURE);
    }

    // Write response 
    numWritten = write(responseFifoDescriptor, &response, sizeof(Response));
    if (numWritten < sizeof(Response))
    {
        printf("Error in writing response to response fifo.\n");
        exit(EXIT_FAILURE);
    }

    // Close response
    status = close(responseFifoDescriptor);
    if (status == -1)
    {
        printf("Error in closing response fifo.\n");
        exit(EXIT_FAILURE);
    }

    free(requestPointer);

    pthread_exit(NULL);
        
}

int main(int argc, char* argv[])
{
    int status, index; 
    int requestFifoDescriptor, dummyDescriptor;
    int numRead;
    Request request;
    pthread_t requestHandlerThread;
    Request* requestHandlerInput;

    /*Register exit handler*/
    status = atexit(removeRequestFifo);
    if (status != 0)
    {
        printf("Error in registering exit handler.\n");
        exit(EXIT_FAILURE);
    }

    /*Register signal handler*/
    if (signal(SIGINT, handleInterruptSignal) == SIG_ERR)
    {
        printf("Error in registering signal handler.\n");
        exit(EXIT_FAILURE);
    }

    /* Create request FIFO */
    status = mkfifo(
        REQUEST_NAME,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP 
    );
    if (status == -1)
    {
        printf("Error in making request fifo.\n");
        exit(EXIT_FAILURE);
    }

    /* Open the request FIFO for reading */
    requestFifoDescriptor = open(REQUEST_NAME, O_RDONLY);
    if (requestFifoDescriptor == -1)
    {
        printf("Error in opening the request fifo.\n");
        exit(EXIT_FAILURE);
    }

    /* Open the dummy descriptor to avoid getting EOF */
    dummyDescriptor = open(REQUEST_NAME, O_WRONLY);
    if (dummyDescriptor == -1)
    {
        printf("Error in opening the dummy descriptor.\n");
        exit(EXIT_FAILURE);
    }

    /* Process request and response*/
    while (TRUE)
    {
        /* Read next request */
        numRead = read(requestFifoDescriptor, &request, sizeof(Request));
        if (numRead == -1)
        {
            printf("Error in reading request fifo.\n");
            exit(EXIT_FAILURE);
        }

        /* Create the request handler input 
           so that every child thread gets its own request object
           instead of reusing a single request variable
           which would lead to a race condition
        */
        requestHandlerInput = (Request*) malloc(sizeof(Request));
        if (requestHandlerInput == NULL)
        {
            printf("Error in allocating memory for request handler input.\n");
            exit(EXIT_FAILURE);
        }

        memcpy(requestHandlerInput, &request, sizeof(Request));

        /* Create request handler thread */
        status = pthread_create(
            &requestHandlerThread, 
            NULL, 
            computeAverage, 
            (void*) requestHandlerInput
        );
        if (status != 0)
        {
            printf("Error in creating thread.\n");
            exit(EXIT_FAILURE);
        }

        /* Detach the request handler thread */
        status = pthread_detach(requestHandlerThread);
        if (status != 0)
        {
            printf("Error in detaching request handler thread.\n");
            exit(EXIT_FAILURE);
        }
    
    }
   
    exit(EXIT_SUCCESS);
}

