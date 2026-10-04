#include <stdio.h>
#include "suppliers.h"

int main(void)
{
    int choice;

    do
    {
        printf("\n========================================\n");
        printf(" MUNICIPAL FINANCIAL MANAGEMENT SYSTEM\n");
        printf("========================================\n");
        printf("          SUPPLIER MANAGEMENT\n");
        printf("----------------------------------------\n");
        printf("1. Add Supplier\n");
        printf("2. Display Suppliers\n");
        printf("3. Search Supplier\n");
        printf("4. Exit\n");
        printf("----------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input. Please enter a number from 1 to 4.\n");

            while (getchar() != '\n')
            {
                /* Clear invalid input */
            }

            choice = 0;
            continue;
        }

        switch (choice)
        {
            case 1:
                addSupplier();
                break;

            case 2:
                displaySuppliers();
                break;

            case 3:
                searchSupplier();
                break;

            case 4:
                printf("\nExiting Supplier Management...\n");
                break;

            default:
                printf("\nInvalid choice. Please select 1 to 4.\n");
        }

    } while (choice != 4);

    return 0;
}