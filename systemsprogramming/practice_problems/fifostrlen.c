/**
 * Goal/s:
 * - Implement a program in two unrelated processes, a writer and a reader, 
 * communicate using a FIFO 
 * - Parent process writes a string of any length (up to a maximum) to a pipe
 * - Child process reads that string from the pipe and prints out the length of the string.
 * 
 * Compile using : cc fifostrlen.c -o fifostrlen
 * Run using: ./fifostrlen
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
  
int main(int argc, char* argv[])
{
    int numRead, numWritten;
    char message[100];
    int status, childStatus;
    int value;
    int fifoDescriptor;
    int childId;    // For process
    int index, messageLength;
    int messageSize;

    /* Test area */
    strcpy(message, "Systems Programming");
    printf("%s\n", message);
    // Creation
    status = mkfifo(
        "FIFOPractice", 
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
        fifoDescriptor = open("FIFOPractice", O_RDONLY);
        if (fifoDescriptor == -1)
        {
            printf("Failed to open the FIFO.\n");
            _exit(EXIT_FAILURE);
        }

        numRead = read(fifoDescriptor, &value, sizeof(char));
        messageSize = 0;
        // printf("Message length:");
        while (numRead > 0)
        {
            
            if (value == '\0')
            {
                // printf("\n");
                // messageSize = messageSize + 1;
                break;
            }
            // printf("%c", value);
            // messageSize = messageSize +1;
            numRead = read(fifoDescriptor, &value, sizeof(char));
        }
        // printf("%d", messageLength);
        if (numRead == -1)
        {
            printf("Error in reading fifo.\n");
            _exit(EXIT_FAILURE);
        }
        

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
        fifoDescriptor = open("FIFOPractice", O_WRONLY);
        if (fifoDescriptor == -1)
        {
            printf("Failed to open the FIFO.\n");
            exit(EXIT_FAILURE);
        }
        messageLength = strlen(message) + 1;
        numWritten = write(fifoDescriptor, message, messageLength);
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
    printf("The length should be %d\n", messageLength);
    exit(EXIT_SUCCESS);
    
}