Project title: MUNICIPAL FINANCIAL MANAGEMENT SYSTEM

Group number:4,9,11,17,23,7,9

GitHub: https://github.com/225047071-Tuwilika/PAP-Project-A.git



**Project description**

\- The MFMS is a menu-driven console application written in C that helps a municipality manage employees, departmental budgets, suppliers and assets, and produce summary reports. It is the foundation version that will be extended and refactored in Project B.



**System features**

**- Employee Management;** add, display and search employees (by ID, name or department); it also calculates gross salary, pension, income tax and get salary and prints a payslip.

\- **Budget Management;** enter the departmental budgets and expenditure, calculate the remaining budget, show the status (WITHIN BUDGET /NEAR LIMIT/ OVER BUDGET) and list departments that exceeded their budget.

\- **Supplier Management;** add, display and search suppliers (by ID, name or town) and compare two suppliers side by side.

\- Asset Management; asset register with type and condition menus; search by ID, name, type or department.

\- **Reports;** employee report( count, average, highest, lowest salary, pre-department headcount), budget report (totals and departments over budget), supplier report and asset report (total value, assets in poor condition).

Input validation; negative salaries/budgets/ values, empty names, invalid numbers, invalid menu choices, duplicate IDs, invalid e-mail and phone formats are all rejected with a clear message and user is asked again.



**compilation instruction**

Requirements: GCC (c99 support)



gcc -std=c99 -Wall -Wextra -pendantic -o mfms main.c common.c employees.c budget.c suppliers.c assets.c reports.

On Wndows( VS Code terminal) the output file is 'mfms.exe'.



**how to run the system**

bash

./mfms    # start with an empty system

./mfms --demo   # start with sample data already loaded



Navigate by typing the number of a menu option and pressing enter. choose option 6 on the main menu to exit.



\# Running the automated tests

bash

make test   # or: bash tests/run\_tests.sh



\# project structure



MFMS

|- main.c/ .h            main menu and project flow

|- common.c/ .h        input reading, validation and string helper functions

|- employees.c /.h        employee management and salary calculation

|- budget.c / .h           budget management

|- supplier.c / .h         supplier management

|- assets.c  /.h          assets register

|- reports.c /.h         employee, budget, supplier and asset reports



|- Makefile

|- tests/run\_test.sh

|- docs/                    technical report, test plan. contribution records

|\_ README.md



**individual responsibilities**



1. Employee management- Imanuel Dibo
2. Budget management - Pauline Kufuna KZ
3. Supplier management- Chaze Musunga
4. Asset management- Erastus Nambala
5. Reports-Peter Kamati
6. Functions, integration, validation- Petrus Hamunyela
7. Testing, documentation, Git Coordination- Tuwilika P Kanime

