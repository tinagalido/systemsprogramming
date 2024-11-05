/*
 * Goals:
 * Parent process creates two child processes.
 * Each child processes runs "date --utc", and then exits.
 * Parent process waits for the child processes to complete and then exits.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void printBye()
{
    printf("Process %d says bye!\n", getpid());
}

int main (int argc, char* argv[])
{

    pid_t parent, child;
    pid_t childPID[2];
    int status, childStatus;

    status = atexit(printBye);
    if (status != 0)
    {
        printf("Failed to register exit handler.\n");
    }

    for (int index = 0; index < 2; index++)
    {
        childPID[index] = fork();

        switch (childPID[index])
        {
        case -1:
            printf("Error in creating child.\n");
            exit(EXIT_FAILURE);
        case 0:
            printf("I am the child with process id %d and my parent ID is %d", 
                getpid(), 
                getppid()
                );
            status = execlp("/usr/bin/date", "date", "--utc", NULL);
            if (status == -1)
            {
                printf("Failed in executing execlp.\n");
                _exit(EXIT_FAILURE);
            }
            //_exit(EXIT_SUCCESS);
        default:
            break;
        }
    } // end of for loop

    for (int index = 0; index < 2; index++)
    {
        child = waitpid(childPID[index], &childStatus, 0);
        if (child == -1)
        {
            printf("Faied to wait for child process.\n");
        }
        printf("Child with process id %d executed with status %d.\n", 
            child,
            childStatus);

    }
    exit(EXIT_SUCCESS);
}