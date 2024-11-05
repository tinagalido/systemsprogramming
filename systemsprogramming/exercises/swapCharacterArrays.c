#include <stdlib.h>
#include <stdio.h>

void swapCharArrays (char* x, char* y, unsigned int numOfIntegers)
{

    char temp = *x;
    *x = *y;
    *y = temp;

    
}

int main()
{
    char arrCharOne[5] = "Four";
    char arrCharTwo[5] = "Nine";

    printf("Before swap:\n");
    printf("x = \"%c%c%c%c\"\n", arrCharOne[0], arrCharOne[1], arrCharOne[2], arrCharOne[3]);
    printf("y = \"%c%c%c%c\"\n", arrCharTwo[0], arrCharTwo[1], arrCharTwo[2], arrCharTwo[3]);
    
    for (int ctr = 0; ctr < 4; ctr++)
    {
        swapCharArrays(&arrCharOne[ctr], &arrCharTwo[ctr], 5);
    }
    
    printf("After swap:\n");
    printf("x = \"%c%c%c%c\"\n", arrCharOne[0], arrCharOne[1], arrCharOne[2], arrCharOne[3]);
    printf("y = \"%c%c%c%c\"\n", arrCharTwo[0], arrCharTwo[1], arrCharTwo[2], arrCharTwo[3]);
    
    return 0;

}