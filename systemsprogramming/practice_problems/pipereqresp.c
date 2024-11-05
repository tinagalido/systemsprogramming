/**
 * Goal/s: 
 * - Parent process writes a string of any length (up to a maximum) to a “request” pipe
 * - Child process reads the string from the pipe, converts it to uppercase,
 *   and the result to the parent using a “response” pipe.
 * 
 * Compile using : cc pipereqresp.c -o pipereqresp
 * Run using: ./pipereqresp
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
  
int main(int argc, char* argv[])
{
    int pipeDescriptor[2];
    int numRead, numWritten;
    char request[100];
    char response[100];
    char value;
    int status, childStatus;
    int childId;    // For process
    int index, messageLength;

    /* Test area */
    strcpy(request, "hello");
    printf("Request: %s\n", request);
    status = pipe(pipeDescriptor);
    if (status == -1)
    {
        printf("Error in creating pipe.\n");
        exit(EXIT_FAILURE);
    }

    /* pipeDescriptor[0] is a read descriptor; pipeDescriptor[1] is a write descriptor */
    childId = fork();
    switch (childId)
    {
    case -1:
        printf("Error in creating process for pipes.\n");
        exit(EXIT_FAILURE);
        
    case 0: // Child
        /* Closes the write descriptor */
        status = close(pipeDescriptor[1]);
        if (status == -1)
        {
            printf("Error in closing the write descriptor.\n");
            _exit(EXIT_FAILURE);
        }

        /* Reads from the parent */
        numRead = read(pipeDescriptor[0], &value, sizeof(char));
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
            numRead = read(pipeDescriptor[0], &value, sizeof(char));
            index++;
        }
        
        /* Closes the read descriptor */
        status = close(pipeDescriptor[0]);
        if (status == -1)
        {
            printf("Error in closing the read descriptor.\n");
            _exit(EXIT_FAILURE);
        }
        _exit(EXIT_SUCCESS);
    
    default: // Parent
        /* Close the read descriptor */
        status = close(pipeDescriptor[0]);
        if (status == -1)
        {
            printf("Error in closing the read descriptor.\n");
            exit(EXIT_FAILURE);
        }

        /* Write in the pipe */
        messageLength = strlen(request) + 1;
        numWritten = write(pipeDescriptor[1], request, messageLength);
        if (numWritten != messageLength)
        {
            printf("Partial error in writing.\n");
            exit(EXIT_FAILURE);
        }
       
        
        /* Close the write descriptor */
        status = close(pipeDescriptor[1]);
        if (status == -1)
        {
            printf("Error in closing the write descriptor.\n");
            exit(EXIT_FAILURE);
        }
        
        status = wait(&childStatus);
        if (status == -1)
        {
            printf("Error in waiting for child to complete.\n");
            exit(EXIT_FAILURE);
        }

        
        exit(EXIT_SUCCESS);
    } 

    
}