/* Suppose message is a string and index is an int 
* Run index from 0 to MAX - 1
* For each value of index
* Set the contents of message to "index = n"
* When index is 0, message should be "index = 0"
* When index is 1, message should be "index = 1"
* E.g. 
* In Python, this can be done in two ways:
* message = "index = " + index
* OR
* message = "index = {}".format(index)
* In C:
* sprintf(message, "index = %d", index);
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 10
#define MESSAGE_LENGTH 10

int main()
{
    int index;
    char message[MESSAGE_LENGTH];

    for (index = 0; index < MAX; index++)
    {
        snprintf(message, MESSAGE_LENGTH,"index=%d", 1000000 * index);
        printf("The message is %s\n", message);
    }
    exit(EXIT_SUCCESS);
    
}