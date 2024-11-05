/*
* Compile using : cc copy.c -o copy
* Run using: ./copy source destination
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    int sourceFd, destFd;
    int numRead, numWritten, status;
    char buffer[BUFFER_SIZE];

    if (argc != 3 ||
        strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0)
    {
        printf("Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    sourceFd = open(argv[1], O_RDONLY);
    if (sourceFd == -1)
    {
        printf("Failed to open source file %s\n", argv[1]);
        exit(1);
    }

    destFd = open(
        argv[2], 
        O_CREAT | O_WRONLY | O_TRUNC,
        S_IRUSR | S_IWUSR | S_IRGRP | 
        S_IWGRP | S_IROTH | S_IWOTH); // rw-rw-rw

    if (destFd == -1)
    {
        printf("Failed to open destination file %s\n", argv[2]);
        exit(1);
    }

    numRead = read(sourceFd, buffer, BUFFER_SIZE);
    while (numRead > 0)
    {
        numWritten = write(destFd, buffer, numRead); /* NOT BUFFER_SIZE*/
        //if (numWritten < numRead)
        if (numWritten < numRead)
        {
            printf("Partial write error.\n");
            exit(1);
        }
        numRead = read(sourceFd, buffer, BUFFER_SIZE);
    }
    
    if (numRead == -1)
    {
        printf("Failed to read from the source file\n");
        exit(1);
    }

    status = close(sourceFd);
    if (status == -1)
    {
        printf("Failed to close source file\n");
        exit(1);
    }
    
    if (close(destFd) == -1)
    {
        printf("Failed to close destination file\n");
        exit(1);
    }
}