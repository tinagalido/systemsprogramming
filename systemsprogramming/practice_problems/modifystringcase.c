/*
 *  Modifies the string to lower or upper case
 *
 *  Compile using: cc modifystringcase.c -o modifystringcase
 *  Run using: ./modifystringcase mode stringtomodify
 *  Also support: ./modifystringcase -h
 *  and support: ./modifystringcase --help
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void modifyStrings(char *items[], int (*visitor)(int)) {
    // Iterate through each string in the array
    for (int i = 0; items[i] != NULL; i++) {
        // Iterate through each character in the string
        for (int j = 0; items[i][j] != '\0'; j++) {
            // Apply the visitor function to each character
            items[i][j] = visitor(items[i][j]);
        }
    }
}

int main(int argc, char* argv[])
{
    int numArg, index;
    char **items; 

    if (argc < 3 ||
        strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0)
    {
        printf("Usage: %s mode stringtomodify\n", argv[0]);
        printf("Mode can be either tolower or toupper\n");
        exit(1);
    } else
    
    numArg = argc - 2;
    items = (char **)malloc(numArg * sizeof(char *));

    // Check for memory
    if (items == NULL)
    {
        printf("Memory failed");
        exit(EXIT_FAILURE);
    }

    //Populate items
    for (int index = 0; index < numArg; index++)
    {
        items[index] = strdup(argv[2 + index]);
        
    }

    // Configure if lower or upper or others
    if (strcmp(argv[1],"tolower") == 0)
    {
        modifyStrings(items, 
            tolower);
    } else
    if (strcmp(argv[1], "toupper") == 0)
    {
        modifyStrings(items, toupper);
    }
    else
    {
        printf("It should be toupper or tolower!\n");
        exit(1);
    }

    // Print the modified strings
    for (int a = 0; items[a] != NULL; a++) {
        printf("%s ", items[a]);
    }
    printf("\n");

    free(items);

    exit(EXIT_SUCCESS);

}