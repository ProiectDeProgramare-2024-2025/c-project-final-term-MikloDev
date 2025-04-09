#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 100
#define FILENAME "contacts.txt"

// Function prototypes
void trimNewline(char *str);
int isEmpty(const char *str);
void printColored(const char *text, const char *color);

typedef struct 
{
    char phone[15];     // Phone number
    char label[30];     // Name
    char company[50];   // Company name
    char email[50];     // Email
    int blocked;        // Blocked or not
} Contact;

Contact contacts[MAX_CONTACTS];
int contactCount = 0;

// Function to load contacts from a file
void loadContacts()
{
    FILE *file = fopen(FILENAME, "r"); // Open file in read mode

    if (file == NULL) return;

    contactCount = 0;

    char line[200];
    while (fgets(line, sizeof(line), file)) 
    {
        // Remove the newline character
        trimNewline(line);

        // Extract phone number and label
        if (sscanf(line, "%*d [%14[^]]] %29s", contacts[contactCount].phone, contacts[contactCount].label) == 2)
        {
            // Initialize empty
            contacts[contactCount].company[0] = '\0';
            contacts[contactCount].email[0] = '\0';

            // Look for " - " and extract fields
            char *ptr = strstr(line, "] ");
            if (ptr != NULL) 
            {
                ptr += 2; // Skip past "] "
                char *nextPtr = strstr(ptr, " - ");
                if (nextPtr != NULL) 
                {
                    // Extract company
                    sscanf(ptr, "%49[^-]", contacts[contactCount].company);
                    ptr = nextPtr + 3; // Skip past " - "
                    
                    // Extract email if it exists
                    char *nextNextPtr = strstr(ptr, " - ");
                    if (nextNextPtr != NULL) 
                    {
                        sscanf(ptr, "%49[^\n\r]", contacts[contactCount].email);
                    } 
                    else 
                    {
                        sscanf(ptr, "%49[^\n\r]", contacts[contactCount].email);
                    }
                } 
                else 
                {
                    // If no " - " found, check if there's a company
                    sscanf(ptr, "%49[^\n\r]", contacts[contactCount].company);
                }
            }

            contactCount++;
            if (contactCount >= MAX_CONTACTS) break;
        }
    }

    fclose(file);
}

// Function to save contacts to a file
void saveContacts() 
{
    FILE *file = fopen(FILENAME, "w");

    if (file == NULL) 
    {
        printf("\nError saving contacts!\n");
        return;
    }

    for (int i = 0; i < contactCount; i++) 
    {
        fprintf(file, "%d [%s] %s", i + 1, contacts[i].phone, contacts[i].label);

        // Only add company if it's not empty
        if (!isEmpty(contacts[i].company))
            fprintf(file, " - %s", contacts[i].company);

        // Only add email if it's not empty
        if (!isEmpty(contacts[i].email))
            fprintf(file, " - %s", contacts[i].email);

        // Add a newline character after the entire contact has been written
        fprintf(file, "\n");
    }

    fclose(file);
}

// Function for the menu header
void header() 
{
    printf("\n");
    printColored("--------------", "\033[1;34m" "\n");       // Blue
    printColored("1 - Add Contact", "\033[1;32m" "\n");      // Green
    printColored("2 - Display Contacts", "\033[1;32m" "\n"); // Green
    printColored("3 - Delete Contact", "\033[1;32m" "\n");   // Green
    printColored("0 - Exit", "\033[1;31m" "\n");             // Red
    printColored("--------------", "\033[1;34m" "\n");       // Blue
}

int isEmpty(const char *str) 
{
    return str == NULL || str[0] == '\0';
}

void trimNewline(char *str) 
{
    str[strcspn(str, "\n")] = '\0';
}

// Function to print colored text
void printColored(const char *text, const char *color) 
{
    printf("%s%s\033[0m", color, text); // Reset color after printing
}

// Function for adding new contacts
void addContact()
{
    if (contactCount >= MAX_CONTACTS) 
    {
        printf("\nContact list is full!\n");
        return;
    }

    Contact newContact;

    printf("Enter phone number: ");
    fgets(newContact.phone, sizeof(newContact.phone), stdin);
    trimNewline(newContact.phone);

    printf("Enter label (name): ");
    fgets(newContact.label, sizeof(newContact.label), stdin);
    trimNewline(newContact.label);

    printf("Enter company (optional): ");
    fgets(newContact.company, sizeof(newContact.company), stdin);
    trimNewline(newContact.company);

    printf("Enter email (optional): ");
    fgets(newContact.email, sizeof(newContact.email), stdin);
    trimNewline(newContact.email);

    newContact.blocked = 0;

    contacts[contactCount++] = newContact;

    saveContacts();
    printf("\nContact saved!\n");
}

// Function for displaying contacts
void displayContacts() 
{
    if (contactCount == 0) 
    {
        printf("\nNo contacts available.\n");
        return;
    }
    printf("\nCONTACT LIST:\n");

    for (int i = 0; i < contactCount; i++) 
    {
        printf("%d ", i + 1);

        printColored(contacts[i].phone, "\033[1;31m"); // Red

        printf(" - ");

        printColored(contacts[i].label, "\033[1;36m"); // Turquoise

        if (!isEmpty(contacts[i].company))
        {
            printf(" - ");
            printColored(contacts[i].company, "\033[1;32m"); // Green
        }

        if (!isEmpty(contacts[i].email))
        {
            printf(" - ");
            printColored(contacts[i].email, "\033[1;34m"); // Blue
        }

        printf("\n");
    }
}

//Function for deleting a contact
void deleteContact() 
{
    if (contactCount == 0) 
    {
        printf("\nNo contacts to delete. \n");
        return;
    }

    int index;

    printf("Enter contact number to delete: ");
    scanf("%d", &index);

    while (getchar() != '\n'); // Clear the input buffer

    if (index < 1 || index > contactCount)
    {
        printf("Invalid contact number.\n");
        return;
    }

    for (int i = index - 1; i < contactCount - 1; i++) 
    {
        contacts[i] = contacts[i + 1];
    }
    contactCount--;

    printf("\nContact deleted successfully!\n");

    saveContacts();
}

//Function for the menu
void menu(int op) 
{
    switch(op) 
    {
        case 1: addContact(); break;        //Press 1 to use the addConctact() function
        case 2: displayContacts(); break;   //Press 2 to use the displayContacts() function 
        case 3: deleteContact(); break;     //Press 3 to use the deleteContact() function 

        default: printf("\nPlease enter a valid option.\n");    //Press 0 to exit the program
    }
}

// The program uses the header() function, then asks the person to send back a number between 1 - 3
// The program then chooses which menu function should pop-up
// While the number is greater than 0 and lesser or equal than 3, else the program exits.
int main() 
{
    loadContacts();

    char input[10];
    int option;

    while(1)
    {
        printf("\nOptions:");
        header();
        printf("\nEnter option: ");


        if (fgets(input, sizeof(input), stdin) == NULL) 
        {
            printf("\nInput error. Try again.\n");
            continue;
        }

        if (sscanf(input, "%d", &option) != 1) // Check if valid integer input
        {
            printf("\nInvalid input. Please enter a number.\n");
            continue;
        }

        if (option == 0) // Check if input is 0 to exit
        {
            printf("\nExiting...\n");
            break;
        }

        menu(option);
    }

    return 0;
}