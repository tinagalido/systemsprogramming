#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int valuesToWrite[] = {1, 3, 5, 7, 9};
    int fifoDescriptor, status, size;
    int numWritten;

    fifoDescriptor = open("MyFIFO", O_WRONLY);
    if (fifoDescriptor == -1)
    {
        printf("Failed to open the FIFO.\n");
        exit(EXIT_FAILURE);
    }

    size = 5*sizeof(int);
    numWritten = write(fifoDescriptor, valuesToWrite, size);
    if (numWritten < size)
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

    exit(EXIT_SUCCESS);
}