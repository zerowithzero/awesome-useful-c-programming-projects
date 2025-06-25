/*
 * Invoice Generator
 * =================
 * 
 * This program is a simple invoice generator written in C. It allows users to:
 *   - Generate new invoices for buyers, either by loading items from a CSV file or by manual entry.
 *   - List all previously generated invoices stored in the "invoices" directory.
 *   - View the contents of a specific invoice by filename.
 * 
 * Features:
 * ---------
 * - Stores invoices as timestamped text files in the "invoices" folder.
 * - Calculates subtotal, GST (18%), and grand total for each invoice.
 * - Supports buyer details (name, phone, email).
 * - Loads items from a CSV file ("items.csv") or allows manual entry.
 * - Lists and views past invoices.
 * 
 * Key Structures:
 * ---------------
 * - Item: Represents an invoice item (name, quantity, price, total).
 * - Buyer: Represents buyer details (name, phone, email).
 * 
 * Main Functions:
 * ---------------
 * - getCurrentDateTime: Gets current date/time as a string for filenames.
 * - printDateTime: Prints and writes the current date/time to invoice.
 * - loadItemsFromCSV: Loads items from "items.csv" into an array.
 * - generateInvoice: Generates and saves an invoice file.
 * - listPastInvoices: Lists all invoice files in the "invoices" directory.
 * - viewInvoiceByName: Displays the contents of a selected invoice file.
 * 
 * Usage:
 * ------
 * 1. Run the program.
 * 2. Choose to generate a new invoice, list past invoices, or view a specific invoice.
 * 3. For new invoices, enter buyer details and choose item entry method.
 * 4. Invoices are saved in the "invoices" directory.
 * 
 * Note:
 * -----
 * - Requires "items.csv" for CSV item loading.
 * - Creates "invoices" directory if it does not exist.
 * - Designed for basic demonstration and educational purposes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

// Maximum number of items per invoice
#define MAX_ITEMS 100
// GST tax rate (18%)
#define TAX_RATE 0.18
// Folder to store invoices
#define INVOICE_FOLDER "invoices"

// Structure to represent an item in the invoice
typedef struct {
    char name[50];     // Item name
    int quantity;      // Quantity purchased
    float price;       // Price per item
    float total;       // Total price for this item (quantity * price)
} Item;

// Structure to represent buyer details
typedef struct {
    char name[100];    // Buyer's name
    char phone[20];    // Buyer's phone number
    char email[100];   // Buyer's email address
} Buyer;

// Gets the current date and time as a string for filenames
void getCurrentDateTime(char *datetimeStr, int maxLen) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(datetimeStr, maxLen, "%Y-%m-%d_%H-%M-%S", t);
}

// Prints and writes the current date/time to the invoice file
void printDateTime(FILE *fptr) {
    char dt[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(dt, sizeof(dt), "%d-%m-%Y %H:%M:%S", t);
    fprintf(fptr, "Date: %s\n", dt);
    printf("Date: %s\n", dt);
}

// Loads items from "items.csv" into the items array
int loadItemsFromCSV(Item items[]) {
    FILE *fp = fopen("items.csv", "r");
    if (!fp) {
        printf("Error opening items.csv\n");
        return 0;
    }

    char line[200];
    int count = 0;

    // Read each line and parse item details
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%d,%f", items[count].name, &items[count].quantity, &items[count].price);
        items[count].total = items[count].quantity * items[count].price;
        count++;
    }

    fclose(fp);
    return count; // Return number of items loaded
}

// Generates and saves an invoice file
void generateInvoice(Item items[], int count, Buyer buyer) {
    char filename[100];
    char timestamp[50];

    // Create a unique filename using the current date and time
    getCurrentDateTime(timestamp, sizeof(timestamp));
    sprintf(filename, "%s/invoice_%s.txt", INVOICE_FOLDER, timestamp);

    FILE *fptr = fopen(filename, "w");
    if (!fptr) {
        printf("Error creating invoice file.\n");
        return;
    }

    float subtotal = 0, tax, grandTotal;

    // Print invoice header
    printf("\n======== INVOICE ========\n");
    fprintf(fptr, "======== INVOICE ========\n");

    // Print and write date/time
    printDateTime(fptr);

    // Print and write buyer details
    fprintf(fptr, "Buyer Name : %s\n", buyer.name);
    fprintf(fptr, "Phone      : %s\n", buyer.phone);
    fprintf(fptr, "Email      : %s\n", buyer.email);

    printf("Buyer Name : %s\n", buyer.name);
    printf("Phone      : %s\n", buyer.phone);
    printf("Email      : %s\n", buyer.email);

    // Print and write item table header
    printf("\n%-20s %-10s %-10s %-10s\n", "Item", "Qty", "Price", "Total");
    fprintf(fptr, "\n%-20s %-10s %-10s %-10s\n", "Item", "Qty", "Price", "Total");

    // Print and write each item
    for (int i = 0; i < count; i++) {
        subtotal += items[i].total;
        printf("%-20s %-10d %-10.2f %-10.2f\n", items[i].name, items[i].quantity, items[i].price, items[i].total);
        fprintf(fptr, "%-20s %-10d %-10.2f %-10.2f\n", items[i].name, items[i].quantity, items[i].price, items[i].total);
    }

    // Calculate tax and grand total
    tax = subtotal * TAX_RATE;
    grandTotal = subtotal + tax;

    // Print and write totals
    printf("\nSubtotal: %.2f\n", subtotal);
    printf("GST (%.0f%%): %.2f\n", TAX_RATE * 100, tax);
    printf("Grand Total: %.2f\n", grandTotal);

    fprintf(fptr, "\nSubtotal: %.2f\n", subtotal);
    fprintf(fptr, "GST (%.0f%%): %.2f\n", TAX_RATE * 100, tax);
    fprintf(fptr, "Grand Total: %.2f\n", grandTotal);

    fclose(fptr);

    printf("\nInvoice saved to %s\n", filename);
}

// Lists all invoice files in the "invoices" directory
void listPastInvoices() {
    DIR *dir = opendir(INVOICE_FOLDER);
    struct dirent *entry;

    if (!dir) {
        printf("Could not open invoice directory.\n");
        return;
    }

    printf("\n📄 Available Invoices:\n");
    // Loop through directory entries and print invoice files
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, "invoice_")) {
            printf("- %s\n", entry->d_name);
        }
    }
    closedir(dir);
}

// Displays the contents of a selected invoice file
void viewInvoiceByName() {
    char fname[100];
    printf("\nEnter invoice filename (e.g., invoice_2025-06-25_18-00-00.txt): ");
    scanf("%s", fname);

    char path[150];
    sprintf(path, "%s/%s", INVOICE_FOLDER, fname);
    FILE *fp = fopen(path, "r");
    if (!fp) {
        printf("File not found!\n");
        return;
    }

    // Print file contents to console
    char ch;
    while ((ch = fgetc(fp)) != EOF)
        putchar(ch);
    fclose(fp);
}

// Main program loop
int main() {
    // Create the invoices directory if it doesn't exist (works on Linux/Mac; for Windows, use mkdir invoices)
    system("mkdir -p " INVOICE_FOLDER);

    int choice;
    do {
        // Display main menu
        printf("\n=========================\n");
        printf("Invoice Generator Menu:\n");
        printf("1. Generate New Invoice\n");
        printf("2. List Past Invoices\n");
        printf("3. View Specific Invoice\n");
        printf("0. Exit\n");
        printf("=========================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        if (choice == 1) {
            Buyer buyer;
            Item items[MAX_ITEMS];
            int itemCount = 0;

            // Get buyer details
            printf("Enter Buyer Name: ");
            fgets(buyer.name, sizeof(buyer.name), stdin);
            buyer.name[strcspn(buyer.name, "\n")] = 0;

            printf("Enter Phone: ");
            fgets(buyer.phone, sizeof(buyer.phone), stdin);
            buyer.phone[strcspn(buyer.phone, "\n")] = 0;

            printf("Enter Email: ");
            fgets(buyer.email, sizeof(buyer.email), stdin);
            buyer.email[strcspn(buyer.email, "\n")] = 0;

            int inputChoice;
            // Choose item entry method
            printf("\nItem Entry Method:\n");
            printf("1. Load items from CSV (items.csv)\n");
            printf("2. Enter items manually\n");
            printf("Enter your choice: ");
            scanf("%d", &inputChoice);
            getchar();

            if (inputChoice == 1) {
                // Load items from CSV file
                itemCount = loadItemsFromCSV(items);
                if (itemCount == 0) {
                    printf("No items loaded from CSV. Aborting invoice.\n");
                    continue;
                }
            } else if (inputChoice == 2) {
                // Manual item entry
                printf("Enter number of items: ");
                scanf("%d", &itemCount);
                getchar();

                for (int i = 0; i < itemCount; i++) {
                    printf("\nItem %d:\n", i + 1);
                    printf("Name: ");
                    fgets(items[i].name, sizeof(items[i].name), stdin);
                    items[i].name[strcspn(items[i].name, "\n")] = 0;

                    printf("Quantity: ");
                    scanf("%d", &items[i].quantity);
                    printf("Price per item: ");
                    scanf("%f", &items[i].price);
                    getchar();

                    items[i].total = items[i].quantity * items[i].price;
                }
            } else {
                printf("Invalid input method. Try again.\n");
                continue;
            }

            // Generate and save the invoice
            generateInvoice(items, itemCount, buyer);
        } else if (choice == 2) {
            // List all past invoices
            listPastInvoices();
        } else if (choice == 3) {
            // View a specific invoice by filename
            viewInvoiceByName();
        }

    } while (choice != 0);

    printf("Exiting Invoice Generator. Goodbye!\n");
    return 0;
}
