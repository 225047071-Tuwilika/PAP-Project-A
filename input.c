#include <stdio.h>
#include <string.h>
#include "input.h"

// Read a whole number
int readInt(void)
{
    int value;

    while (scanf("%d", &value) != 1)
    {
        printf("Invalid input. Please enter a number: ");

        while (getchar() != '\n')
        {
        }
    }

    while (getchar() != '\n')
    {
    }

    return value;
}


// Read a decimal number
float readFloat(void)
{
    float value;

    while (scanf("%f", &value) != 1)
    {
        printf("Invalid input. Please enter a number: ");

        while (getchar() != '\n')
        {
        }
    }

    while (getchar() != '\n')
    {
    }

    return value;
}


// Read text safely
void readNonEmptyString(char text[], int size)
{
    do
    {
        fgets(text, size, stdin);

        text[strcspn(text, "\n")] = '\0';

        if (strlen(text) == 0)
        {
            printf("Input cannot be empty. Try again: ");
        }

    } while (strlen(text) == 0);
}