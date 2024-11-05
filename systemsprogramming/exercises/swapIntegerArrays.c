#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void swapIntegers (void* first, void* second, size_t size)
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

void printComma(int x, int arrLength)
{
    if (x < arrLength -1)
    {
        printf(",");
    }
}

int main()
{
    int arrOne[] = {1,3,5,7};
    int arrTwo[] = {2,4,6,8}; 
    
    int arrOneLength, arrTwoLength;
    
    arrOneLength = sizeof(arrOne)/sizeof(arrOne[0]); /*size of arrOne*/
    arrTwoLength = sizeof(arrTwo)/sizeof(arrTwo[0]); /*size of arrTwo*/

    printf("Before swap:\n");
    printf("x = [");
    for (int x = 0; x < arrOneLength; x++)
    {
        printf("%d", arrOne[x]);
        printComma(x, arrOneLength);
    }
    printf("]\n");
    printf("y = [");
    for (int y = 0; y < arrTwoLength; y++)
    {
        printf("%d", arrTwo[y]);
        printComma(y, arrTwoLength);
    }
    printf("]\n");

    for (int ctr = 0; ctr < sizeof(int); ctr++)
    {
        swapIntegers(&arrOne[ctr], &arrTwo[ctr], sizeof(int));
    }
    
    printf("After swap:\n");
    printf("x = [");
    for (int x = 0; x < arrOneLength; x++)
    {
        printf("%d", arrOne[x]);
        printComma(x, arrOneLength);
    }
    printf("]\n");
    printf("y = [");
    for (int y = 0; y < arrTwoLength; y++)
    {
        printf("%d", arrTwo[y]);
        printComma(y, arrTwoLength);
    }
    printf("]\n");
    return 0;

}