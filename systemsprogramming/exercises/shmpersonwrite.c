#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

typedef struct 
{
    char firstName[20];
    char lastName[20];
} Person;

int main(int argc, char* argv[])
{
    int status;
    int shmDescriptor;
    Person* pointerToSharedVariable;
    sem_t* personSemaphore;
    void* addressSpace;
    //char* personValue;
    // struct Person person;

    // Create semaphore
    personSemaphore = sem_open("/PersonSemaphore",O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (personSemaphore == SEM_FAILED)
    {
        printf("Failed to create semaphore.\n");
        exit(EXIT_FAILURE);
    }

    //Create or open the shared memory
    shmDescriptor = shm_open("/PersonSharedMemory",O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shmDescriptor == -1)
    {
        printf("Failed to created shared memory.\n");
        exit(EXIT_FAILURE);
    }

    // Set the size of the shared memory to the size of Person
    status = ftruncate(shmDescriptor, sizeof(Person));
    if (status == -1)
    {
        printf("Failed to set the size of the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    // Map the shared memory into the address space
    addressSpace = mmap(NULL, sizeof(Person), 
        PROT_READ | PROT_WRITE, MAP_SHARED, shmDescriptor, 0);
    if (addressSpace == MAP_FAILED)
    {
        printf("Failed to map the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    // Close the shared memory
    status = close(shmDescriptor);
    if (status == -1)
    {
        printf("Failed to close the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    // Set the mapped address space to the shared variable
    pointerToSharedVariable = (Person*)addressSpace;

    // Process semaphores
    // Lock
    status = sem_wait(personSemaphore);
    if (status != 0)
    {
        printf("Failed to lock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    //Critical
    // Person persons[1] = {
    //     {"Juan","Cruz"}
    // };
    Person person1;
    // char fullName[50];
    // strcpy(person1.firstName, "Juan");
    // strcpy(person1.lastName,"Cruz");
    // strcpy(fullName, person1.firstName);
    // strcpy(fullName," ");
    // strcpy(fullName, person1.lastName);
    snprintf(pointerToSharedVariable->firstName,
        sizeof(pointerToSharedVariable->firstName), "Juan");
    snprintf(pointerToSharedVariable->lastName,
        sizeof(pointerToSharedVariable->lastName), "Cruz");

    *pointerToSharedVariable = person1;

    //Unlock
    status = sem_post(personSemaphore);
    if (status != 0)
    {
        printf("Failed to unlock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    //Close
    status = sem_close(personSemaphore);
    if (status != 0)
    {
        printf("Failed to close sempahore.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}