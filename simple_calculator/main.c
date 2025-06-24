#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// Function declarations
void showMenu();
void performOperation(int choice);
void logHistory(char *operation);
void clearHistory();
void viewHistory();

// Main function
int main()
{
    int choice;
    do
    {
        showMenu();
        printf("\nEnter your choice (0 to exit): ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Exiting...\n");
            break;
        }
        if (choice == 8)
        {
            clearHistory();
        }
        else if (choice == 9)
        {
            viewHistory();
        }
        else if (choice != 0)
        {
            performOperation(choice);
        }
    } while (choice != 0);

    printf("Calculator closed.\n");
    return 0;
}

// Display menu
void showMenu()
{
    printf("\n====== CLI Calculator ======\n");
    printf("1. Addition (+)\n");
    printf("2. Subtraction (-)\n");
    printf("3. Multiplication (*)\n");
    printf("4. Division (/)\n");
    printf("5. Modulo (%%)\n");
    printf("6. Square Root (√)\n");
    printf("7. Power (x^y)\n");
    printf("8. Clear History\n");
    printf("9. View History\n");
    printf("0. Exit\n");
    printf("============================\n");
}

// Perform operation based on choice
void performOperation(int choice)
{
    double a, b, result;
    char operation[100];

    switch (choice)
    {
    case 1:
        printf("Enter two numbers: ");
        scanf("%lf %lf", &a, &b);
        result = a + b;
        printf("Result: %.2lf\n", result);
        sprintf(operation, "Addition: %.2lf + %.2lf = %.2lf", a, b, result);
        break;

    case 2:
        printf("Enter two numbers: ");
        scanf("%lf %lf", &a, &b);
        result = a - b;
        printf("Result: %.2lf\n", result);
        sprintf(operation, "Subtraction: %.2lf - %.2lf = %.2lf", a, b, result);
        break;

    case 3:
        printf("Enter two numbers: ");
        scanf("%lf %lf", &a, &b);
        result = a * b;
        printf("Result: %.2lf\n", result);
        sprintf(operation, "Multiplication: %.2lf * %.2lf = %.2lf", a, b, result);
        break;

    case 4:
        printf("Enter dividend and divisor: ");
        scanf("%lf %lf", &a, &b);
        if (b == 0)
        {
            printf("Error: Division by zero!\n");
            return;
        }
        result = a / b;
        printf("Result: %.2lf\n", result);
        sprintf(operation, "Division: %.2lf / %.2lf = %.2lf", a, b, result);
        break;

    case 5:
        printf("Enter two integers: ");
        int x, y;
        scanf("%d %d", &x, &y);
        if (y == 0)
        {
            printf("Error: Division by zero!\n");
            return;
        }
        int modResult = x % y;
        printf("Result: %d\n", modResult);
        sprintf(operation, "Modulo: %d %% %d = %d", x, y, modResult);
        break;

    case 6:
        printf("Enter number: ");
        scanf("%lf", &a);
        if (a < 0)
        {
            printf("Error: Cannot find square root of a negative number!\n");
            return;
        }
        result = sqrt(a);
        printf("Square root: %.2lf\n", result);
        sprintf(operation, "Square Root: √%.2lf = %.2lf", a, result);
        break;

    case 7:
        printf("Enter base and exponent: ");
        scanf("%lf %lf", &a, &b);
        result = pow(a, b);
        printf("Result: %.2lf\n", result);
        sprintf(operation, "Power: %.2lf ^ %.2lf = %.2lf", a, b, result);
        break;

    default:
        printf("Invalid choice!\n");
        return;
    }

    // Log to file
    logHistory(operation);
}

// Log operation to file
void logHistory(char *operation)
{
    FILE *log = fopen("calc_history.txt", "a");
    if (log == NULL)
    {
        printf("Error: Could not open log file!\n");
        return;
    }

    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strlen(timestamp) - 1] = '\0'; // Remove newline

    fprintf(log, "[%s] %s\n", timestamp, operation);
    fclose(log);
}

// View history if option 9 is selected
void viewHistory()
{
    FILE *log = fopen("calc_history.txt", "r");
    if (log == NULL)
    {
        printf("Error: Could not open history file.\n");
        return;
    }

    printf("\n------ Calculation History ------\n");
    char line[256];
    int empty = 1;
    while (fgets(line, sizeof(line), log))
    {
        printf("%s", line);
        empty = 0;
    }

    if (empty)
    {
        printf("No history found.\n");
    }

    fclose(log);
    printf("\n---------------------------------\n");
}

// Clear history if option 8 is selected
void clearHistory()
{
    FILE *log = fopen("calc_history.txt", "w");
    if (log == NULL)
    {
        printf("Error: Could not clear history!\n");
        return;
    }
    fclose(log);
    printf("History cleared successfully.\n");
}
