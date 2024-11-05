/**
 * Goal: 
 * (1) A parent with process id p creates three child processes with process id x,y,z
 * (2) After creating each child processes, the parent prints 
 * "Parent process p created child process x"
 * (3) When child process x gets created, it prints "Child process x going to sleep for 10 seconds"
 * (4) It then sleeps using the sleep system call
 * (5) After sleeping, it then prints "Exiting child process x" and exit
 * (6) Child processes y and z do the same thing but sleeps at 5 and 2 seconds, respectively
 * (7) Check for behavior for the ff:
 *  (a) Parent does not wait
 *  (b) Parent wait using the wait system call
 *  (c) Parent wait using waitpid
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void printExit()
{
    printf("PID %d exited.\n", getpid());
}

int main(int argc, char* argv[])
{
    pid_t child;
    pid_t childPID[3];
    unsigned int delay[3] = {10,5,2};
    int index, status, childStatus;

    /*Register the exit handler*/
    status = atexit(printExit);
    if (status != 0)
    {
        printf("Error in registering exit handler.\n");
    }
    for (index = 0; index < 3; index++)
    {
        childPID[index] = fork();
        switch (childPID[index])
        {
            case -1:
                printf("Failed to create child.\n");
                exit(EXIT_FAILURE);
            case 0: /*This is the child process*/
                printf("Child process %d is going to sleep for %d seconds.\n",
                    getpid(),
                    delay[index]
                );
                sleep(delay[index]);
                printf("Exiting child process %d.\n", getpid());
                _exit(EXIT_SUCCESS);
            default: /*This is the parent processsss*/
                printf("Parent process %d created child process %d.\n", 
                    getpid(), 
                    childPID[index]
                );
                break;
                
        }
    }// end of for loop

    // Case (a): Parent exits as soon as 3 child processes were created
    // Case (b): Child processes (y,z) already starts before parent creates it (diff behavior)
    // Case (c): 
    for (index = 0; index < 3; index++)
    {
        //child = wait(&childStatus);  /*case (b)*/
        child = waitpid(childPID[index], &childStatus, 0);

        if (child == -1)
        {
            printf("Failed to wait for child process to complete.\n");
            exit(EXIT_FAILURE);
        }
        printf("Child %d completed with status %d.\n",
            child,
            childStatus);
    }
    exit (EXIT_SUCCESS);
}