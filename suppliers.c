#include <stdio.h>
#include <string.h>
#include "suppliers.h"

#define MAX_SUPPLIERS 100

/* Arrays used to store supplier information */
char supplierID[MAX_SUPPLIERS][20];
char supplierName[MAX_SUPPLIERS][50];
char supplierEmail[MAX_SUPPLIERS][50];
char supplierPhone[MAX_SUPPLIERS][20];
char supplierTown[MAX_SUPPLIERS][30];

int supplierCount = 0;


/* Checks whether a telephone number contains numbers only */
int isValidPhone(char phone[])
{
    int i;

    if (strlen(phone) == 0)
    {
        return 0;
    }

    for (i = 0; phone[i] != '\0'; i++)
    {
        if (phone[i] < '0' || phone[i] > '9')
        {
            return 0;
        }
    }

    return 1;
}


/* Checks whether an email contains @ and a dot */
int isValidEmail(char email[])
{
    if (strchr(email, '@') == NULL ||
        strchr(email, '.') == NULL)
    {
        return 0;
    }

    return 1;
}


/* Adds a new supplier */
void addSupplier(void)
{
    char newID[20];
    int i;

    if (supplierCount >= MAX_SUPPLIERS)
    {
        printf("\nSupplier list is full.\n");
        return;
    }

    printf("\n========================================\n");
    printf("             ADD SUPPLIER\n");
    printf("========================================\n");

    printf("Enter Supplier ID: ");
    scanf("%19s", newID);

    /* Prevent duplicate supplier IDs */
    for (i = 0; i < supplierCount; i++)
    {
        if (strcmp(newID, supplierID[i]) == 0)
        {
            printf("\nError: Supplier ID already exists.\n");
            return;
        }
    }

    strcpy(supplierID[supplierCount], newID);

    printf("Enter Supplier Name: ");
    scanf(" %49[^\n]", supplierName[supplierCount]);

    do
    {
        printf("Enter Email: ");
        scanf("%49s", supplierEmail[supplierCount]);

        if (!isValidEmail(supplierEmail[supplierCount]))
        {
            printf("Invalid email. Email must contain @ and a dot.\n");
        }

    } while (!isValidEmail(supplierEmail[supplierCount]));

    do
    {
        printf("Enter Telephone Number: ");
        scanf("%19s", supplierPhone[supplierCount]);

        if (!isValidPhone(supplierPhone[supplierCount]))
        {
            printf("Invalid telephone number. Use numbers only.\n");
        }

    } while (!isValidPhone(supplierPhone[supplierCount]));

    printf("Enter Town/Location: ");
    scanf(" %29[^\n]", supplierTown[supplierCount]);

    supplierCount++;

    printf("\nSupplier added successfully!\n");
}


/* Displays all registered suppliers */
void displaySuppliers(void)
{
    int i;

    printf("\n========================================\n");
    printf("             SUPPLIER LIST\n");
    printf("========================================\n");

    if (supplierCount == 0)
    {
        printf("No suppliers registered.\n");
        return;
    }

    for (i = 0; i < supplierCount; i++)
    {
        printf("\nSupplier %d\n", i + 1);
        printf("----------------------------------------\n");
        printf("Supplier ID: %s\n", supplierID[i]);
        printf("Name: %s\n", supplierName[i]);
        printf("Email: %s\n", supplierEmail[i]);
        printf("Telephone: %s\n", supplierPhone[i]);
        printf("Town/Location: %s\n", supplierTown[i]);
    }

    printf("\nTotal Suppliers: %d\n", supplierCount);
}


/* Searches for suppliers by ID or name */
void searchSupplier(void)
{
    int choice;
    int i;
    int found = 0;
    char searchValue[50];

    if (supplierCount == 0)
    {
        printf("\nNo suppliers registered. Add a supplier first.\n");
        return;
    }

    printf("\n========================================\n");
    printf("            SEARCH SUPPLIER\n");
    printf("========================================\n");
    printf("1. Search by Supplier ID\n");
    printf("2. Search by Supplier Name\n");
    printf("----------------------------------------\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1)
    {
        printf("\nInvalid input. Please enter 1 or 2.\n");

        while (getchar() != '\n')
        {
            /* Clear invalid input */
        }

        return;
    }

    if (choice == 1)
    {
        printf("Enter Supplier ID: ");
        scanf("%49s", searchValue);

        for (i = 0; i < supplierCount; i++)
        {
            if (strcmp(searchValue, supplierID[i]) == 0)
            {
                printf("\nSupplier found!\n");
                printf("----------------------------------------\n");
                printf("Supplier ID: %s\n", supplierID[i]);
                printf("Name: %s\n", supplierName[i]);
                printf("Email: %s\n", supplierEmail[i]);
                printf("Telephone: %s\n", supplierPhone[i]);
                printf("Town/Location: %s\n", supplierTown[i]);

                found = 1;
                break;
            }
        }
    }
    else if (choice == 2)
    {
        printf("Enter Supplier Name: ");
        scanf(" %49[^\n]", searchValue);

        for (i = 0; i < supplierCount; i++)
        {
            if (strcmp(searchValue, supplierName[i]) == 0)
            {
                printf("\nSupplier found!\n");
                printf("----------------------------------------\n");
                printf("Supplier ID: %s\n", supplierID[i]);
                printf("Name: %s\n", supplierName[i]);
                printf("Email: %s\n", supplierEmail[i]);
                printf("Telephone: %s\n", supplierPhone[i]);
                printf("Town/Location: %s\n", supplierTown[i]);

                found = 1;
                break;
            }
        }
    }
    else
    {
        printf("\nInvalid search choice. Please select 1 or 2.\n");
        return;
    }

    if (found == 0)
    {
        printf("\nSupplier not found.\n");
    }
}