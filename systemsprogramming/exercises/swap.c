#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person
{
    char firstName[20];
    char lastName[20];
} Person;

/*Swapping integers*/
void swapInt (int* px, int* py)
{
    int temp = *px;
    *px = *py;
    *py = temp;

}
void swapChar (char* px, char* py)
{
    int temp = *px;
    *px = *py;
    *py = temp;
}


void swapPerson (Person* px, Person* py)
{
    Person temp = *px;
    *px = *py;
    *py = temp;

}
void swap (void* first, void* second, size_t size)
{
    void* temp = malloc(size);
    if (temp == NULL)
    {
        printf("malloc failed");
        exit(1);
   
    }
    memcpy(temp, first, size);
    memcpy(first, second, size);
    memcpy(second, temp, size);
    
    free(temp);
}

int main(int argc, char* argv[])
{
    int x,y; 
    Person p1 = {"Alan", "Turing"};
    Person p2 = {"Charles", "Babbage"};
        //int y = 4;
    if (argc != 3 ||
        strcmp(argv[1],"-h") == 0 ||
        strcmp(argv[1],"--help") == 0 )
    {
        printf("Usage: %s n1 n2\n", argv[0]);
        exit(1);
    }

    x = atoi(argv[1]);
    y = atoi(argv[2]);
    printf("Before swap: x = %d, y = %d\n", x, y);

    // swapInt (&x,&y);
    swap(&x, &y, sizeof(int));

    printf("After swap: x = %d, y = %d\n", x, y);

    printf("Before swap: p1 =%s %s, p2 = %s %s\n",
        p1.firstName,
        p1.lastName,
        p2.firstName,
        p2.lastName
    );

    swap(&p1,&p2, sizeof(Person));

    printf("After swap: p1 =%s %s, p2 = %s %s\n",
        p1.firstName,
        p1.lastName,
        p2.firstName,
        p2.lastName
    );

    exit(0);

}