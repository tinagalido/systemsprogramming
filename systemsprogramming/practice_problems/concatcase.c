/*
 *  Concatenates the contents of the file into the console/display
 *
 *  Compile using: cc concatcase.c -o concatcase
 *  Run using: ./concatcase input1 input2 input3
 *  The number of input files can be any number of txt files
 *  Also support: ./concatcase -h
 *  and support: ./concatcase --help
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    
    int numRead, numWritten, status;
    int inputFileCount;
    char buffer[BUFFER_SIZE];

    // Error checking
    if (argc < 2 ||
        strcmp(argv[1],"-h") == 0 ||
        strcmp(argv[1],"--help") == 0)
    {
        printf("Usage: %s inputfile1 inputfile2 inputfile3 \n",argv[0]);
        printf("The number of inputfiles can be any number of txt files.\n");
        exit(EXIT_FAILURE);
    }

    inputFileCount = argc - 1;
    int sourceFd[inputFileCount];
    for (int index = 0; index < inputFileCount; index++)
    {
        sourceFd[index] = open(argv[index + 1],O_RDONLY);
        if (sourceFd[index] == -1)
        {
            printf("Error in reading the file %s.\n", argv[index]);
            exit(EXIT_FAILURE);
        }

        numRead = read(sourceFd[index], buffer, BUFFER_SIZE);
        while (numRead > 0)
        {
            numWritten = write(STDOUT_FILENO,buffer, numRead); // Print in the screen
            if (numWritten < numRead)
            {
                printf("Error in reading file/s\n");
                exit(EXIT_FAILURE);
            }
            numRead = read(sourceFd[index], buffer, BUFFER_SIZE);
            printf("\n");
        }

        // Checking for files read
        if (numRead == -1)
        {
            printf("Error in reading the file/s\n");
            exit(EXIT_FAILURE);
        }

        status = close(sourceFd[index]);
        if (status == -1)
        {
            printf("Error in closing the file/s.\n");
            exit(EXIT_FAILURE);
        }
    }

}