/**
 * The goal is for a parent process to create two child process
 * The first child process will sleep for 3 seconds, the second child will sleep for 1 second
 * The child process will then sleep for 2 seconds and exit
 * The parent process will exit only after both children have exited
 * There are two exits, _exit() and exit()
 * The system call for exiting is _exit()
 * A helpful library function for exiting is exit()
 * which does some cleaning up ("exit handlers") and then calls _exit()
 * Child process should call _exit() directly
 * Parent process should call exit()
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void printBye()
{
    printf("PID %d says Bye!!\n", getpid());
}

int main(int  argc, char* argv[])
{
    pid_t parentPid, child;
    pid_t childPid[2];
    pid_t delay[2] = {3,1};
    int status, index;

    /* Register the exit handler */
    status = atexit(printBye);
    if (status == -1) 
    {
        printf("FAiled to register exit handler.\n");
        exit(EXIT_FAILURE);
    }

    // Process creation
    for (index = 0; index < 2; index++)
    {
        childPid[index] = fork();

        switch (childPid[index])
        {
        case -1: /* This is to create child process */
            printf("Failed to create child process.\n");
            exit(EXIT_FAILURE);
            //break;
        case 0:   /* This is the child process*/
            printf("I am the child process. My PID is %d and my parent's PID is %d.\n",
                getpid(), 
                getppid()
            );
            status = execlp("/bin/ls", "ls","-l", NULL);    
            //sleep(delay[index]);
            if (status == -1)
            {
                printf("Child process failed to exec.\n");
                exit(EXIT_FAILURE);
            }
            /* Control should never reach here*/
            // printf("Child process failed to exec.\n");
            // exit(EXIT_FAILURE);
            // }
            //_exit(EXIT_SUCCESS);    /* Child's exit */
        default:
            break;
    
        }
    }

    // Parent to do process creation
    for (index = 0; index < 2; index++)     /* Only runs in the parent */
    {
        child = waitpid(childPid[index], &status, 0);//wait(&status);
        if (child == -1) 
        {
            printf("Parent failed to wait for child to complete.\n");
            exit(EXIT_FAILURE);
        }

        printf("Child %d completed with status %d.\n", child, status);
    }
    // childPid = fork();

    // switch (childPid)
    // {
    // case -1: /* This is to create child process */
    //     printf("Failed to create child process.\n");
    //     exit(EXIT_FAILURE);
    //     //break;
    // case 0:   /* This is the child process*/
    //     printf("I am the child process. My PID is %d and my parent's PID is %d.\n",
    //         getpid(), 
    //         getppid()
    //     );
    //     sleep(2);
    //     _exit(EXIT_SUCCESS);    /* Child's exit */
    // default:
    //     printf("I am the parent process. My PID is %d, and my child's PID is %d.\n",
    //         getpid(),
    //         childPid
    //     );

    //     childPid = wait(&status);
    //     if (childPid == -1) 
    //     {
    //         printf("PArent failed to wait for child to complete.\n");
    //         exit(EXIT_FAILURE);
    //     }

    //     printf("Child %d completed with status %d.\n", childPid, status);

    exit(EXIT_SUCCESS);
    

   
    
}