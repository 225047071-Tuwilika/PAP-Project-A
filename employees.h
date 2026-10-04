#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#define MAX_EMPLOYEES 100

// Employee structure
typedef struct
{
    int id;
    char name[100];
    char department[100];
    float basicSalary;
    float housingAllowance;
    float transportAllowance;
} Employee;

// Employee data
extern Employee employees[MAX_EMPLOYEES];
extern int employeeCount;

// Employee functions
void employeeMenu(void);
void addEmployee(void);
void displayEmployees(void);
void searchEmployee(void);
float calculateSalary(Employee employee);
int employeeIDExists(int id);

// Input functions from main.c
int readInt(void);
float readFloat(void);
void readNonEmptyString(char text[], int size);

#endif