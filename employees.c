#include <stdio.h>
#include "employees.h"

// Employee data
Employee employees[MAX_EMPLOYEES];
int employeeCount = 0;

// Check if Employee ID already exists
int employeeIDExists(int id)
{
    int i;

    for (i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == id)
        {
            return 1;
        }
    }

    return 0;
}

// Employee Menu
void employeeMenu(void)
{
    int choice;

    do
    {
        printf("\n========== EMPLOYEE MANAGEMENT ==========\n");
        printf("1. Add Employee\n");
        printf("2. Display Employees\n");
        printf("3. Search Employee\n");
        printf("4. Calculate Salary\n");
        printf("0. Back\n");
        printf("Enter your choice: ");

        choice = readInt();

        switch (choice)
        {
            case 1:
                addEmployee();
                break;

            case 2:
                displayEmployees();
                break;

            case 3:
                searchEmployee();
                break;

            case 4:
            {
                int id;
                int i;

                printf("Enter Employee ID: ");
                id = readInt();

                for (i = 0; i < employeeCount; i++)
                {
                    if (employees[i].id == id)
                    {
                        printf("Total Salary: %.2f\n",
                               calculateSalary(employees[i]));
                        break;
                    }
                }

                if (i == employeeCount)
                {
                    printf("Employee not found.\n");
                }

                break;
            }

            case 0:
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 0);
}

// Add Employee
void addEmployee(void)
{
    if (employeeCount >= MAX_EMPLOYEES)
    {
        printf("Employee limit reached.\n");
        return;
    }

    do
    {
        printf("Enter Employee ID: ");
        employees[employeeCount].id = readInt();

        if (employees[employeeCount].id <= 0)
        {
            printf("Employee ID must be greater than 0.\n");
        }
        else if (employeeIDExists(employees[employeeCount].id))
        {
            printf("Employee ID already exists. Enter a different ID.\n");
        }

    } while (employees[employeeCount].id <= 0 ||
             employeeIDExists(employees[employeeCount].id));

    printf("Enter Employee Name: ");
    readNonEmptyString(employees[employeeCount].name,
                       sizeof(employees[employeeCount].name));

    printf("Enter Department: ");
    readNonEmptyString(employees[employeeCount].department,
                       sizeof(employees[employeeCount].department));

    do
    {
        printf("Enter Basic Salary: ");
        employees[employeeCount].basicSalary = readFloat();

        if (employees[employeeCount].basicSalary < 0)
        {
            printf("Salary cannot be negative.\n");
        }

    } while (employees[employeeCount].basicSalary < 0);

    do
    {
        printf("Enter Housing Allowance: ");
        employees[employeeCount].housingAllowance = readFloat();

        if (employees[employeeCount].housingAllowance < 0)
        {
            printf("Allowance cannot be negative.\n");
        }

    } while (employees[employeeCount].housingAllowance < 0);

    do
    {
        printf("Enter Transport Allowance: ");
        employees[employeeCount].transportAllowance = readFloat();

        if (employees[employeeCount].transportAllowance < 0)
        {
            printf("Allowance cannot be negative.\n");
        }

    } while (employees[employeeCount].transportAllowance < 0);

    employeeCount++;

    printf("Employee added successfully.\n");
}

// Display Employees
void displayEmployees(void)
{
    int i;

    if (employeeCount == 0)
    {
        printf("No employees available.\n");
        return;
    }

    printf("\n========== EMPLOYEES ==========\n");

    for (i = 0; i < employeeCount; i++)
    {
        printf("\nEmployee ID: %d\n", employees[i].id);
        printf("Name: %s\n", employees[i].name);
        printf("Department: %s\n", employees[i].department);
        printf("Basic Salary: %.2f\n", employees[i].basicSalary);
        printf("Housing Allowance: %.2f\n",
               employees[i].housingAllowance);
        printf("Transport Allowance: %.2f\n",
               employees[i].transportAllowance);
        printf("Total Salary: %.2f\n",
               calculateSalary(employees[i]));
    }
}

// Search Employee
void searchEmployee(void)
{
    int id;
    int i;

    printf("Enter Employee ID to search: ");
    id = readInt();

    for (i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == id)
        {
            printf("\nEmployee Found\n");
            printf("ID: %d\n", employees[i].id);
            printf("Name: %s\n", employees[i].name);
            printf("Department: %s\n", employees[i].department);
            printf("Total Salary: %.2f\n",
                   calculateSalary(employees[i]));
            return;
        }
    }

    printf("Employee not found.\n");
}

// Calculate Salary
float calculateSalary(Employee employee)
{
    return employee.basicSalary +
           employee.housingAllowance +
           employee.transportAllowance;
}