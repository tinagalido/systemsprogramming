#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

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

int main(int argc, char* argv[])
{
    int status, index; 
    int requestFifoDescriptor, responseFifoDescriptor, dummyDescriptor;
    int numRead,numWritten;
    char responseFifoName[RESPONSE_MAX_LENGTH];
    Request request;
    Response response;
    double average;
    double sum = 0.0;

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
        for (index = 0; index < MAX_NUMBER; index++)
        {
            printf("Server received request %d:%lf\n", index, request.randomDoubleList[index]);
        }
        
        /* Process response */
        for (index = 0; index < MAX_NUMBER; index++)
        {
            response.randomDoubleNumList[index] = request.randomDoubleList[index];
            sum = sum + request.randomDoubleList[index];
            //printf("sum:%lf\n", sum);
        }
        average = sum / MAX_NUMBER; 
        //printf("average:%lf\n", average);
        response.average = average;
        

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
    }
    

    exit(EXIT_SUCCESS);
}

