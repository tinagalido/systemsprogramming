/**
 * Goal/s: 
 * - Parent process writes a string of any length (up to a maximum) to a pipe
 * - Child process reads that string from the pipe and prints out the length of the string.
 * 
 * Compile using : cc pipestrlen.c -o pipestrlen
 * Run using: ./pipestrlen
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
  
int main(int argc, char* argv[])
{
    int pipeDescriptor[2];
    int numRead, numWritten;
    char message[100];
    char value;
    int status, childStatus;
    int childId;    // For process
    int index, messageLength;

    /* Test area */
    strcpy(message, "Life is beautiful!");

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
        while (numRead > 0)
        {
            if (value == '\0')
            {
                printf("\n");
                break;
            }
            printf("%c", value);
            numRead = read(pipeDescriptor[0], &value, sizeof(char));
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
        messageLength = strlen(message) + 1;
        numWritten = write(pipeDescriptor[1], message, messageLength);
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
        printf("The length should be %d\n", messageLength);
        exit(EXIT_SUCCESS);
    } 

    
}