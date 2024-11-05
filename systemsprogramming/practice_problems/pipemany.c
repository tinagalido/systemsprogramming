/**
 * GOAL: 
 * (1) A parent process creates n child processes, 
 * (2) Each child must complete m tasks, namely,T1, T2, ..., Tm, where n and m are 
 * positive integers provided by the user through command line arguments
 * (3) The parent process wants to find out when all the child processes have completed T1,
 * when all the child processes have completed T2, and so on, and when all the children 
 * have completed all their tasks. 
 * (4) Implement the above using an array of m − 1 pipes
 * 
 * Compile using : cc pipemany.c -o pipemany
 * Run using: ./copy pipemany noOfChildren noOfTaskPerChild
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> /* for p_id */
#include <unistd.h> /* for pipe, sleep, fork  */
#include <sys/wait.h> /* for wait */

#define BUFFERSIZE 10

void doChildTask(int childIndex, char* taskName)
{
    printf("Child %d completed task %s\n", childIndex, taskName);
}

int main(int argc, char* argv[])
{
    int noOfArgs = argc - 1;
    int arrInt[noOfArgs]; 
    int pd[2];
    pid_t child; 
    int status, childStatus, index, taskIndex, pipeIndex;
    char buffer[BUFFERSIZE];
    int numRead;
    int noOfChildren, noOfTasksPerChild;
    char taskName[10];
    char taskName2[10];

    /* Error checking */
    if (argc < 2 ||
        strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0)
    {
        printf("Usage: %s noOfChildren noOfTaskPerChild \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (index = 0; index < noOfArgs; index++)
    {
        arrInt[index] = atoi(argv[index + 1]);
        if (arrInt[index] == 0) /* Not an integer*/
        {
            printf("Argument/s is not a positive integer.\n");
            exit(EXIT_FAILURE);
        }
        //printf("Argument[%d]:%d\n",index, arrInt[index]);
    }
    noOfChildren = arrInt[0];
    noOfTasksPerChild = arrInt[1];
    printf("No of children:%d; no of tasks per child:%d\n", noOfChildren, noOfTasksPerChild);
    if (noOfTasksPerChild < 2)
    {
        printf("Task should be at least 2 or more.\n"); /* This is for the m-1 criteria */
        exit(EXIT_FAILURE);
    }
    // strcpy(taskName, "Task");
    /* Pipe processing */
    for (pipeIndex = 0; pipeIndex < (noOfTasksPerChild - 1); pipeIndex++ )
    {
        status = pipe(pd);
        if (status == -1)
        {
            printf("Error in opening pipe.\n");
            exit(EXIT_FAILURE);
        }
        for (index = 0; index < noOfChildren; index++)
        {
            child = fork();
            switch (child)
            {
            case -1:
                printf("Error in creating child.\n");
                exit(EXIT_FAILURE);
            
            case 0: /* Child process */
                /* Close read pipe */
                status = close(pd[0]);
                if (status == -1)
                {
                    printf("Error in closing read pipe.\n");
                    _exit(EXIT_FAILURE);
                }

                /* Do the part of writing */
                int pipeBase = pipeIndex + 1;
                sprintf(taskName, "Task %d", pipeBase);
                doChildTask(index, taskName);

                /* Close write pipe */
                status = close(pd[1]);
                if (status == -1)
                {
                    printf("Error in closing write pipe.\n");
                    _exit(EXIT_FAILURE);
                }
              
                int pipeBase2 = pipeIndex + 2;
                sprintf(taskName2, "Task %d", pipeBase2);
                doChildTask(index,taskName2);

                sleep(2);
                _exit(EXIT_SUCCESS);
            
            default:
                break;
            }
        }

        // Parent
        status = close(pd[1]);
        if (status == -1)
        {
            printf("Error in closing write pipe.\n");
            exit(EXIT_FAILURE);
        }
        numRead = read(pd[0], buffer, BUFFERSIZE);
        if (numRead != 0)
        {
            printf("Parent process did not receive EOF as expected.\n");
            exit(EXIT_FAILURE);
        }
        printf("All children have completed their 2 tasks for pipe[%d].\n", pipeIndex);
        status = close(pd[0]);
        for (index = 0; index < noOfChildren; index++)
        {
            status = wait(&childStatus);
            if (status == -1)
            {
                printf("Error in waiting for child.\n");
                exit(EXIT_FAILURE);
            }
        }
    
    } // end of pipe index for loop
    printf("All %d children have completed all their %d tasks.\n", 
            noOfChildren, noOfTasksPerChild);
    exit(EXIT_SUCCESS);
}