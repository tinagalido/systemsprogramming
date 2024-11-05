/**
 * Goal: Modify the thread_inc.c and update:
 * (1) The input arg is a pointer to a structure with two fields:
 *    a) Unique identifier
 *    b) Number of times to increment the global variable glob
 * (2) After incrementing, will print out:
 *     THREAD ID : <thread identifier>, new value of glob: <new value>
 * (3) After incrementing, should implement an empty loop
 * (4) Redirect the output to output_with_mutex2.txt
 *
 * Compile using: cc -O0 threadswdmutex2.c -o threadswdmutex2 -pthread
 * Run using: ./threadswdmutex2 <value>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE  1024

volatile int glob = 0; /* global variable */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct ThreadValues {
    pthread_t uniqueIdentifier;
    int numberOfIncrementsToGlobalVar;
};

void* threadFunc(void* arg)
{
    struct ThreadValues* threadObj = (struct ThreadValues*) arg;
    int loops = threadObj->numberOfIncrementsToGlobalVar;
    int uniqueID = pthread_self(); /* Set the thread id to that of the child */
    int status,j;
    int outputFile,fileRead,fileWrite;
    char fileName[] = "output_with_mutext2.txt";
    char buffer[BUFFER_SIZE];

    for (j = 0; j < loops; j++)
    {
        status = pthread_mutex_lock(&mutex);
        if (status != 0)
        {
            printf("Error in locking mutex.\n");
        }
        glob++;          /*Critical section*/
        
        //Add empty loop
        for (int k = 0; k < loops; k++)
        {
            // Do nothing
        }
        
        status = pthread_mutex_unlock(&mutex);
        if (status != 0)
        {
            printf("Error in unlocking mutex.\n");
        }
    }
    outputFile = open(
        fileName, 
        O_CREAT | O_WRONLY | O_APPEND, 
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH  /* rw-r-xr-x*/
    );
    
    if (outputFile == -1)
    {
        printf("Failed to open destination file: %s", fileName);
        exit(EXIT_FAILURE);

    }
    fileWrite = dup2(outputFile, STDOUT_FILENO);
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("THREAD ID:%d , new value of glob:%d\n",uniqueID, glob);
 
    if (fileWrite == -1)
    {
        printf("Failed to write into the file.\n");
        exit(EXIT_FAILURE);
    }
    status = close(fileWrite);
    if (status == -1)
    {
        printf("Failed to close destination file.\n");
        exit(EXIT_FAILURE);
    }
    return NULL;
}


int main (int argc, char* argv[])
{
    pthread_t thread1, thread2;
    int loops,status;
    pthread_t mainThreadID; 
    struct ThreadValues threadObj;
    
    if (argc != 2 ||
        strcmp(argv[1],"-h") == 0 ||
        strcmp(argv[1],"--help") == 0)
    {
        printf("Usage: %s numberOfTimeToIncrement\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    loops = atoi(argv[1]);
    if (loops <=0)
    {
        printf("Number should be a positive integer.\n");
        exit(EXIT_FAILURE);
    }
    //Setting variables to struct before creating
    threadObj.numberOfIncrementsToGlobalVar = loops;

    //Thread creation
    status = pthread_create(&thread1,NULL, threadFunc, (void*)&threadObj);
    if (status != 0)
    {
        printf("Error in creating thread1.\n");
        exit(EXIT_FAILURE);
    }
    status = pthread_create(&thread2, NULL, threadFunc, (void*)&threadObj);
    if (status != 0)
    {
        printf("Error in creating thread 2.\n");
        exit(EXIT_FAILURE);
    }

    // Thread joining
    status = pthread_join(thread1, NULL);
    if (status != 0)
    {
        printf("Error in joing thread1.\n");
    }
    status = pthread_join(thread2, NULL);
    if (status != 0)
    {
        printf("Error in joing thread2.\n");
    }

    exit(EXIT_SUCCESS);
}
