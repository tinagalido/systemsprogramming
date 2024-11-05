#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct 
{
    char firstName[20];
    char lastName[20];
} Person;

int main(int argc, char* argv[])
{
    int status;
    int shmDescriptor;
    void* addressSpace;
    Person* pointerToSharedVariable;
    char* valueOfSharedVariable;
    sem_t* personSemaphore;
    struct stat shmMetadata;

    // Open existing sempahore
    personSemaphore = sem_open("/PersonSemaphore", 0);
    if (personSemaphore == SEM_FAILED)
    {
        printf("Failed to open semaphore.\n");
        exit(EXIT_FAILURE);
    }

    // Open shared variable for reading
    shmDescriptor = shm_open("/PersonSharedMemory", O_RDONLY, 0);
    if (status == -1)
    {
        printf("Failed to open shared variable.\n");
        exit(EXIT_FAILURE);
    }

    // Get metadata about shared variable
    status = fstat(shmDescriptor, &shmMetadata);
    if (status == -1)
    {
        printf("Failed to get metadata of shared variable.\n");
        exit(EXIT_FAILURE);
    }

    // Map shared variable into address space
    addressSpace = mmap(NULL, shmMetadata.st_size, 
        PROT_READ, MAP_SHARED, shmDescriptor, 0);
    if (addressSpace == MAP_FAILED)
    {
        printf("Failed to map shared variable.\n");
        exit(EXIT_FAILURE);
    }
    // Close shared variable
    status = close(shmDescriptor);
    if (status == -1)
    {
        printf("Failed to close shared variable.\n");
        exit(EXIT_FAILURE);
    }

    // Set value of address space to pointer to shared variable
    pointerToSharedVariable = (Person*)addressSpace;

    // Process semaphores
    status = sem_wait(personSemaphore);
    if (status != 0)
    {
        printf("Failed to lock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    // Critical section
    valueOfSharedVariable = pointerToSharedVariable->firstName;
    // snprintf(valueOfSharedVariable, 
    //     sizeof(valueOfSharedVariable), pointerToSharedVariable->firstName);
    // snprintf(valueOfSharedVariable,2, " ");
    // snprintf(valueOfSharedVariable, 
    //     sizeof(valueOfSharedVariable), pointerToSharedVariable->lastName);
    printf("%s", valueOfSharedVariable);

    status = sem_post(personSemaphore);
    if (status != 0)
    {
        printf("Failed to unlock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    status = sem_close(personSemaphore);
    if (status != 0)
    {
        printf("Failed to close semaphore.\n");
        exit(EXIT_FAILURE);
    }

    printf("%s", valueOfSharedVariable);
    

    exit(EXIT_SUCCESS);
    
}