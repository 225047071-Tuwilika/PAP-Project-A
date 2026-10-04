/*
 * assets.c - Asset Management module
 * Municipal Financial Management System, Project A
 * Author: Nambala Erastus (226076350)
 */

#include <stdio.h>
#include <string.h>
#include "assets.h"

#define MAX_ASSETS       100
#define ID_SIZE          7       /* up to 6 characters + '\0'  */
#define NAME_SIZE        20      /* up to 19 characters + '\0' */
#define TYPE_SIZE        12
#define DEPT_SIZE        18      /* up to 17 characters + '\0' */
#define COND_SIZE        6
#define LINE_SIZE        100     /* size of the temporary input buffer */
#define MAX_ASSET_VALUE  1000000000.0

/* Parallel arrays: position i in every array describes the same asset */
char   assetID[MAX_ASSETS][ID_SIZE];
char   assetName[MAX_ASSETS][NAME_SIZE];
char   assetType[MAX_ASSETS][TYPE_SIZE];
double assetValue[MAX_ASSETS];
char   assetDept[MAX_ASSETS][DEPT_SIZE];
char   assetCondition[MAX_ASSETS][COND_SIZE];
int    assetCount = 0;

/* ------------------------------------------------------------------ */
/* Input helpers                                                      */
/* ------------------------------------------------------------------ */

/* Returns 1 if the text is empty or contains only spaces/tabs/newlines */
int assetIsBlank(const char *text)
{
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] != ' ' && text[i] != '\t' && text[i] != '\n') {
            return 0;
        }
    }
    return 1;
}

/* Reads the rest of the current input line into junk */
void assetReadRest(char *junk)
{
    if (fgets(junk, LINE_SIZE, stdin) == NULL) {
        junk[0] = '\0';
    }
}

/* Reads one line of text into dest (which holds 'size' characters).
   Returns 1 if the text fitted, 0 if it was too long. */
int assetReadLine(const char *prompt, char *dest, int size)
{
    char line[LINE_SIZE];
    char junk[LINE_SIZE];
    int length;

    printf("%s", prompt);
    if (fgets(line, LINE_SIZE, stdin) == NULL) {
        dest[0] = '\0';
        return 1;
    }

    /* No newline in the buffer means the line was longer than the buffer:
       throw away the rest of it */
    if (line[strcspn(line, "\n")] == '\0') {
        do {
            assetReadRest(junk);
        } while (junk[0] != '\0' && junk[strcspn(junk, "\n")] == '\0');
        return 0;
    }

    line[strcspn(line, "\n")] = '\0';

    length = strlen(line);
    if (length >= size) {
        return 0;
    }

    strcpy(dest, line);
    return 1;
}

/* Keeps asking until the user enters non-empty text that fits in dest */
void assetReadRequired(const char *prompt, char *dest, int size)
{
    int valid = 0;

    while (valid == 0) {
        if (assetReadLine(prompt, dest, size) == 0) {
            printf("Error: input too long (maximum %d characters).\n", size - 1);
        } else if (assetIsBlank(dest)) {
            printf("Error: this field cannot be empty.\n");
        } else {
            valid = 1;
        }
    }
}

/* Reads a whole number. Returns 0 if the input is not a valid whole number. */
int assetReadInteger(const char *prompt)
{
    int number = 0;
    int result;
    char junk[LINE_SIZE];

    printf("%s", prompt);
    result = scanf("%d", &number);
    assetReadRest(junk);               /* remove the rest of the line, incl. newline */

    if (result == 1 && assetIsBlank(junk)) {
        return number;
    }
    return 0;
}

/* Keeps asking until the user enters a number from 0.01 up to the maximum */
double assetReadValue(const char *prompt)
{
    double value = 0;
    int result;
    char junk[LINE_SIZE];
    int valid = 0;

    while (valid == 0) {
        printf("%s", prompt);
        result = scanf("%lf", &value);
        assetReadRest(junk);

        if (result == 1 && assetIsBlank(junk) && value > 0 && value <= MAX_ASSET_VALUE) {
            valid = 1;
        } else {
            printf("Error: enter a number greater than 0 (for example 15000.50).\n");
        }
    }
    return value;
}

/* Shows the type menu and copies the chosen type into dest */
void assetChooseType(char *dest)
{
    int choice = 0;

    while (choice < 1 || choice > 6) {
        printf("\nAsset types:\n");
        printf("1. Vehicle\n");
        printf("2. Computer\n");
        printf("3. Building\n");
        printf("4. Equipment\n");
        printf("5. Furniture\n");
        printf("6. Other\n");
        choice = assetReadInteger("Choose type (1-6): ");
        if (choice < 1 || choice > 6) {
            printf("Error: invalid choice, enter a number from 1 to 6.\n");
        }
    }

    switch (choice) {
        case 1: strcpy(dest, "Vehicle");   break;
        case 2: strcpy(dest, "Computer");  break;
        case 3: strcpy(dest, "Building");  break;
        case 4: strcpy(dest, "Equipment"); break;
        case 5: strcpy(dest, "Furniture"); break;
        case 6: strcpy(dest, "Other");     break;
    }
}

/* Shows the condition menu and copies the chosen condition into dest */
void assetChooseCondition(char *dest)
{
    int choice = 0;

    while (choice < 1 || choice > 3) {
        printf("\nCondition:\n");
        printf("1. Good\n");
        printf("2. Fair\n");
        printf("3. Poor\n");
        choice = assetReadInteger("Choose condition (1-3): ");
        if (choice < 1 || choice > 3) {
            printf("Error: invalid choice, enter a number from 1 to 3.\n");
        }
    }

    switch (choice) {
        case 1: strcpy(dest, "Good"); break;
        case 2: strcpy(dest, "Fair"); break;
        case 3: strcpy(dest, "Poor"); break;
    }
}

/* ------------------------------------------------------------------ */
/* Search and display helpers                                         */
/* ------------------------------------------------------------------ */

/* Returns the position of the asset with this ID, or -1 if not found */
int assetFindByID(const char *id)
{
    for (int i = 0; i < assetCount; i++) {
        if (strcmp(assetID[i], id) == 0) {
            return i;
        }
    }
    return -1;
}

void assetPrintHeader(void)
{
    printf("%-7s %-20s %-10s %-18s %13s %-5s\n",
           "ID", "Name", "Type", "Department", "Value (N$)", "Cond.");
    printf("------------------------------------------------------------------------------\n");
}

void assetPrintRow(int i)
{
    printf("%-7s %-20s %-10s %-18s %13.2f %-5s\n",
           assetID[i], assetName[i], assetType[i],
           assetDept[i], assetValue[i], assetCondition[i]);
}

/* ------------------------------------------------------------------ */
/* Public functions                                                   */
/* ------------------------------------------------------------------ */

int getAssetCount(void)
{
    return assetCount;
}

double getTotalAssetValue(void)
{
    double total = 0;

    for (int i = 0; i < assetCount; i++) {
        total = total + assetValue[i];
    }
    return total;
}

void addAsset(void)
{
    char newID[ID_SIZE];
    int idValid = 0;
    int index = assetCount;            /* position of the new asset */

    if (assetCount >= MAX_ASSETS) {
        printf("Error: the asset register is full (%d assets).\n", MAX_ASSETS);
        return;
    }

    printf("\n--- ADD ASSET ---\n");

    while (idValid == 0) {
        assetReadRequired("Enter Asset ID (e.g. A001): ", newID, ID_SIZE);
        if (strcspn(newID, " \t") < strlen(newID)) {
            printf("Error: the Asset ID cannot contain spaces.\n");
        } else if (assetFindByID(newID) != -1) {
            printf("Error: Asset ID '%s' already exists.\n", newID);
        } else {
            idValid = 1;
        }
    }

    strcpy(assetID[index], newID);
    assetReadRequired("Enter Asset Name: ", assetName[index], NAME_SIZE);
    assetChooseType(assetType[index]);
    assetValue[index] = assetReadValue("Enter Purchase Value N$: ");
    assetReadRequired("Enter Department: ", assetDept[index], DEPT_SIZE);
    assetChooseCondition(assetCondition[index]);

    assetCount++;      /* the asset only counts once every field is valid */

    printf("\nAsset '%s' (%s) added successfully!\n", assetName[index], assetID[index]);
}

void displayAssets(void)
{
    if (assetCount == 0) {
        printf("\nNo assets registered yet.\n");
        return;
    }

    printf("\n========== ASSET REGISTER (%d) ==========\n", assetCount);
    assetPrintHeader();
    for (int i = 0; i < assetCount; i++) {
        assetPrintRow(i);
    }
}

void searchAsset(void)
{
    int choice;
    int index;
    int matches = 0;
    char id[ID_SIZE];
    char type[TYPE_SIZE];
    char dept[DEPT_SIZE];

    if (assetCount == 0) {
        printf("\nNo assets registered yet.\n");
        return;
    }

    printf("\n--- SEARCH ASSETS ---\n");
    printf("1. Search by Asset ID\n");
    printf("2. Search by Asset Type\n");
    printf("3. Search by Department\n");
    choice = assetReadInteger("Enter your choice: ");

    switch (choice) {
        case 1:
            assetReadRequired("Enter Asset ID to search: ", id, ID_SIZE);
            index = assetFindByID(id);
            if (index == -1) {
                printf("Asset '%s' not found.\n", id);
            } else {
                printf("\nAsset found:\n");
                assetPrintHeader();
                assetPrintRow(index);
            }
            break;

        case 2:
            assetChooseType(type);
            for (int i = 0; i < assetCount; i++) {
                if (strcmp(assetType[i], type) == 0) {
                    if (matches == 0) {
                        printf("\nAssets of type %s:\n", type);
                        assetPrintHeader();
                    }
                    assetPrintRow(i);
                    matches++;
                }
            }
            if (matches == 0) {
                printf("No assets of type %s found.\n", type);
            }
            break;

        case 3:
            assetReadRequired("Enter Department to search (exact name): ", dept, DEPT_SIZE);
            for (int i = 0; i < assetCount; i++) {
                if (strcmp(assetDept[i], dept) == 0) {
                    if (matches == 0) {
                        printf("\nAssets in %s:\n", dept);
                        assetPrintHeader();
                    }
                    assetPrintRow(i);
                    matches++;
                }
            }
            if (matches == 0) {
                printf("No assets found for department '%s'.\n", dept);
            }
            break;

        default:
            printf("Invalid choice! Enter 1, 2 or 3.\n");
    }
}

void assetReport(void)
{
    int good = 0;
    int fair = 0;
    int poor = 0;
    int mostValuable = 0;

    printf("\n========================================\n");
    printf(" ASSET REPORT\n");
    printf("========================================\n");

    if (assetCount == 0) {
        printf("No assets registered yet.\n");
        return;
    }

    for (int i = 0; i < assetCount; i++) {
        if (strcmp(assetCondition[i], "Good") == 0) {
            good++;
        } else if (strcmp(assetCondition[i], "Fair") == 0) {
            fair++;
        } else {
            poor++;
        }

        if (assetValue[i] > assetValue[mostValuable]) {
            mostValuable = i;
        }
    }

    printf("Total Assets: %d\n", assetCount);
    printf("Total Purchase Value: N$%.2f\n", getTotalAssetValue());
    printf("Most Valuable Asset: %s (N$%.2f)\n",
           assetName[mostValuable], assetValue[mostValuable]);
    printf("Condition: Good = %d, Fair = %d, Poor = %d\n\n", good, fair, poor);

    assetPrintHeader();
    for (int i = 0; i < assetCount; i++) {
        assetPrintRow(i);
    }
}

void assetMenu(void)
{
    int choice;

    do {
        printf("\n=== ASSET MANAGEMENT ===\n");
        printf("1. Add Asset\n");
        printf("2. Display Assets\n");
        printf("3. Search Asset\n");
        printf("4. Asset Report\n");
        printf("5. Back to Main Menu\n");
        choice = assetReadInteger("Enter your choice: ");

        switch (choice) {
            case 1: addAsset();      break;
            case 2: displayAssets(); break;
            case 3: searchAsset();   break;
            case 4: assetReport();   break;
            case 5: break;
            default: printf("Invalid choice! Enter a number from 1 to 5.\n");
        }
    } while (choice != 5);
}