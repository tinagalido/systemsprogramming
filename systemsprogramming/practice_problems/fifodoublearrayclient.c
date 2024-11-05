

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>


#define MAX_NUMBER 10

/* FIFO NAMES */
#define REQUEST_NAME "RequestFifo"
#define RESPONSE_NAME "ResponseFifo.%d"

#define RESPONSE_MAX_LENGTH 100

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


double random_double()
{
    int random_int = rand();   // Generate from 0 to RAND_MAX

    
    // Scale random int to double from 0 to 1
    double random_dbl = (double)random_int / RAND_MAX;

    return random_dbl;
}

void removeResponseFifo()
{
    int status;
    char responseFifoName[RESPONSE_MAX_LENGTH];

    snprintf(responseFifoName,RESPONSE_MAX_LENGTH, RESPONSE_NAME, getpid());
    status = unlink(responseFifoName);
    if (status == -1)
    {
        printf("Error in unlinking response FIFO.\n");
    }
}

int main(int argc, char* argv[])
{
    int status, index;
    int requestFifoDescriptor, responseFifoDescriptor;
    int numRead,numWritten;
    char responseFifoName[RESPONSE_MAX_LENGTH];
    Request request;
    Response response;
    

    // if (argc != 2 ||
    //     strcmp(argv[1],"-h") == 0 ||
    //     strcmp(argv[1], "--help") == 0)
    // {
    //     printf("Usage: %s max_of_double_number\n", argv[0]);
    //     exit(EXIT_FAILURE);
    // }

    // maxDouble = atoi(argv[1]);
    // if (maxDouble <=0) 
    // {
    //     printf("Maximum number should be positive.\n");
    //     exit(EXIT_FAILURE);
    // }

    // Register the exit handler (removing of response fifo)
    status = atexit(removeResponseFifo);
    if (status != 0)
    {
        printf("Error in registering exit handler.\n");
        exit(EXIT_FAILURE);
    }

    // Create response fifo
    snprintf(responseFifoName, RESPONSE_MAX_LENGTH, RESPONSE_NAME, getpid());

    status = mkfifo(
        responseFifoName,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP
    );
    if (status == -1)
    {
        printf("Error in making FIFO.\n");
        exit(EXIT_FAILURE);
    }

    /* REQUEST */
    // Prepare request
    request.clientId = getpid();
    printf("RAND_MAX:%d\n", RAND_MAX);

    srand(time(NULL)); // Seed the random generator with the time
    
    for (index = 0; index < MAX_NUMBER; index++)
    {
        double random_value = random_double();
        //printf("Random:%lf\n", random_value);
        request.randomDoubleList[index] = random_value;
    }
    
    // Put request into request FIFO
    requestFifoDescriptor = open(REQUEST_NAME, O_WRONLY);
    if (requestFifoDescriptor == -1)
    {
        printf("Client cannot reach serversssss.\n");
        exit(EXIT_FAILURE);
    }

    // Write request
    numWritten = write(requestFifoDescriptor, &request, sizeof(Request));
    if (numWritten < sizeof(Request))
    {
        printf("Error in writing request.\n");
        exit(EXIT_FAILURE);
    }

    // Close request fd
    status = close(requestFifoDescriptor);
    if (status == -1)
    {
        printf("Error in closing request fifo.\n");
        exit(EXIT_FAILURE);
    }
    
    /* RESPONSE */
    // Read the response
    responseFifoDescriptor = open(responseFifoName, O_RDONLY);
    if (responseFifoDescriptor == -1)
    {
        printf("Error in opening response fifo.\n");
        exit(EXIT_FAILURE);
    }
    numRead = read(responseFifoDescriptor, &response, sizeof(Response));
    if (numRead == -1)
    {
        printf("Error in reading response fifo.\n");
        exit(EXIT_FAILURE);
    }
    // Print the response
    for (index = 0; index < MAX_NUMBER; index++)
    {
        printf("Random number %d: %lf\n",index, response.randomDoubleNumList[index]);
    }
    printf("Average of all %d double numbers is:%lf\n", MAX_NUMBER, response.average);
    
    // Close the response
    status = close(responseFifoDescriptor);
    if (status == -1)
    {
        printf("Error in closing response fifo.\n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
