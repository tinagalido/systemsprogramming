/**
 * Goal/s:
 * - Implement a program in two unrelated processes, P1 and P2 communicated using a FIFO. 
 * - P1 writes a string of any length (up to a maximum) to a “request” FIFO.
 * - P2 reads the string from the FIFO, converts it to uppercase, and sends the result to P1 
 * using a “response” FIFO.
 * 
 * Compile using : cc fiforeqresp.c -o fiforeqresp
 * Run using: ./fiforeqresp
 * 
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
  
int main(int argc, char* argv[])
{
    
    int numRead, numWritten;
    // char message[100];
    int status, value, childStatus;
    int fifoDescriptor;
    int childId;    // For process
    int index, messageLength;
    char request[100];
    char response[100];

    /* Test area */
    strcpy(request, "a whole NEW world");
    printf("Request:%s\n", request);
    // Creation
    status = mkfifo(
        "FIFOReqRespPractice", 
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP
    ); /* S_IXUSR, S_IXGRP, S_IXOTHER should NEVER be here */
    if (status == -1)
    {
        printf("Failed to create the FIFO.\n");
        exit(EXIT_FAILURE);
    }
   
    childId = fork();
    
    switch (childId)
    {
    case -1:
        printf("Error in creating child for fifo.\n");
        exit(EXIT_FAILURE);
    case 0: /* Child process */
        fifoDescriptor = open("FIFOReqRespPractice", O_RDONLY);
        if (fifoDescriptor == -1)
        {
            printf("Failed to open the FIFO.\n");
            _exit(EXIT_FAILURE);
        }

        numRead = read(fifoDescriptor, &value, sizeof(char));
        index = 0;
        printf("Response:");
        while (numRead > 0)
        {
            if (value == '\0')
            {
                printf("\n");
                response[index] = '\0';
                break;
            }
            response[index] = toupper(value);
            printf("%c", response[index]);
            numRead = read(fifoDescriptor, &value, sizeof(char));
        }
        if (numRead == -1)
        {
            printf("Error in reading fifo.\n");
            _exit(EXIT_FAILURE);
        }
        // printf("The length should be:%d", messageLength);

        status = close(fifoDescriptor);
        if (status == -1)
        {
            printf("Failed to close the FIFO.\n");
            _exit(EXIT_FAILURE);
        }
        _exit(EXIT_SUCCESS);
    
    default: /* Parent */
        // Write 
        // Open
        fifoDescriptor = open("FIFOReqRespPractice", O_WRONLY);
        if (fifoDescriptor == -1)
        {
            printf("Failed to open the FIFO.\n");
            exit(EXIT_FAILURE);
        }
        messageLength = strlen(request) + 1;
        numWritten = write(fifoDescriptor, request, messageLength);
        if (numWritten != messageLength)
        {
            printf("Partial write error.\n");
            exit(EXIT_FAILURE);
        }
        status = close(fifoDescriptor);
        if (status == -1)
        {
            printf("Failed to close the FIFO.\n");
            exit(EXIT_FAILURE);
        }
        status = wait(&childStatus);
        if (status == -1)
        {
            printf("Error in waiting for child to complete.\n");
            exit(EXIT_FAILURE);
        }
        
    }
    exit(EXIT_SUCCESS);
    
}