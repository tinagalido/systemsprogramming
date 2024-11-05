#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define MAX_LEN 2
typedef struct
{
    char firstName[20];
    char lastName[20];
} Person;

int main (int argc, char* argv[])
{
    int index;
    int status;
    int shmDescriptor;
    Person* pointerToSharedVariable;
    sem_t* personSemaphore;
    void* addressSpace;
    char* valueOfSharedVariableFirst;
    char* valueOfShareVariableLast;

    // Create semaphore
    personSemaphore = sem_open("/NewPersonSemaphore", 
        O_CREAT, 
        S_IRUSR | S_IWUSR, 1);
    if (personSemaphore == SEM_FAILED)
    {
        printf("Failed to open semaphore.\n");
        exit(EXIT_FAILURE);
    }

    // Create shared memory
    shmDescriptor = shm_open("/NewPersonSharedMemory", 
        O_CREAT | O_RDWR, 
        S_IRUSR | S_IWUSR);
    if (shmDescriptor == -1)
    {
        printf("Failed to open shared variable.\n");
        exit(EXIT_FAILURE);
    }

    // Set the size of shared memory
    status = ftruncate(shmDescriptor, sizeof(Person));
    if (status == -1)
    {
        printf("Failed to set the size of shared variable.\n");
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    addressSpace = mmap(NULL, sizeof(Person), 
        PROT_READ | PROT_WRITE, 
        MAP_SHARED, shmDescriptor, 0);
    if (addressSpace == MAP_FAILED)
    {
        printf("Failed to map shared memory.\n");
        exit(EXIT_FAILURE);
    }

    // Set the mapped address to the shared memory
    pointerToSharedVariable = (Person*)addressSpace;

    // Process semaphores
    // WRITE
    status = sem_wait(personSemaphore);
    if (status != 0)
    {
        printf("Failed to lock semaphore.\n");
        exit(EXIT_FAILURE);
    }
    // Critical section
    Person person1;
    // Person persons[MAX_LEN] = {
    //     // {"One","Isa"},
    //     // {"Two", "Duwa"}
          
    // };
    snprintf(person1.firstName, sizeof(person1.firstName), "Juan");
    snprintf(person1.lastName, sizeof(person1.lastName), " Cruz");
    // snprintf(persons[1].firstName, sizeof(persons[1].firstName), "Maria");
    // snprintf(persons[1].lastName, sizeof(persons[1].lastName), " Santos");
    // printf("1:%s %s\n", persons[0].firstName, persons[0].lastName);
    // printf("2:%s %s\n", persons[1].firstName, persons[1].lastName);
    // persons[2].firstName = NULL;
    // persons[2].lastName = NULL;
    *pointerToSharedVariable = person1;

    status = sem_post(personSemaphore);
    if (status != 0)
    {
        printf("Failed to unlock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    // READ
    status = sem_wait(personSemaphore);
    if (status != 0)
    {
        printf("Failed to lock semaphore.\n");
        exit(EXIT_FAILURE);
    }
    // Critical section
    
    valueOfSharedVariableFirst = pointerToSharedVariable->firstName;
    valueOfShareVariableLast = pointerToSharedVariable->lastName;
 
    status = sem_post(personSemaphore);
    if (status != 0)
    {
        printf("Failed to unlock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    // Close semaphore
    status = sem_close(personSemaphore);
    if (status != 0)
    {
        printf("Failed to close semaphore.\n");
        exit(EXIT_FAILURE);
    }

    // Close shared memory
    status = close(shmDescriptor);
    if (status == -1)
    {
        printf("Failed to close shared variable.\n");
        exit(EXIT_FAILURE);
    }


    printf("%s-%s\n", valueOfSharedVariableFirst,
    valueOfShareVariableLast);
   
   
    exit(EXIT_SUCCESS);
}
