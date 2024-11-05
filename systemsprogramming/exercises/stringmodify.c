#include <stdio.h>
#include <stdlib.h>

void stringModify(char* input, int (*visitor) (int) )
{
    int index;
    for (index = 0; input[index] != '\0'; index++)
    {
       input[index] = visitor(input[index]);
    }
    
}