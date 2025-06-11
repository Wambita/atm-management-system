#include "header.h"

// Define the file paths here, as they are declared extern in header.h
const char *USERS_FILE = "./data/users.txt";
const char *RECORDS_FILE = "./data/records.txt";

void mainMenu(struct User u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check details of existing accounts\n");
    printf("\n\t\t[4]- Check list of owned accounts\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");

    while (1)
    {
        printf("\n\t\tEnter your choice: ");
        if (scanf("%d", &option) != 1) {
            printf("\n\t\tInvalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option)
        {
        case 1:
            createNewAcc(u);
            break;
        case 2:
            updateAccountInformation(u);
            break;
        case 3:
            checkAccountDetails(u);
            break;
        case 4:
            checkAllAccounts(u);
            break;
        case 5:
            makeTransaction(u);
            break;
        case 6:
            removeAccount(u);
            break;
        case 7:
            transferOwnership(u);
            break;
        case 8:
            exit(0);
            break;
        default:
            printf("\n\t\tInvalid operation! Please choose a number from 1-8.\n");
        }
        if (option >=1 && option <= 7) break;
    }
}

void initMenu(struct User *u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- Login\n");
    printf("\n\t\t[2]- Register\n");
    printf("\n\t\t[3]- Exit\n");

    while (1)
    {
        printf("\n\t\tEnter your choice: ");
        if (scanf("%d", &option) != 1) {
            printf("\n\t\tInvalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            char storedPassword[MAX_PASSWORD_SIZE];
            getPassword(*u, storedPassword);

            if (strcmp(u->password, storedPassword) == 0) {
                u->id = getUserId(u->name);
                printf("\n\t\t✔ Login successful! Welcome, %s.\n", u->name);
            } else {
                printf("\n\t\t✖ Wrong password or Username not found.\n");
                handleFailedLogin(u);
            }
            return;
        case 2:
            registerMenu(u->name, u->password);
            u->id = getUserId(u->name);
            return;
        case 3:
            exit(0);
        default:
            printf("\n\t\tInvalid operation! Please choose 1, 2, or 3.\n");
        }
    }
}

int main()
{
    struct User u;

    // Initialize data files if they don't exist
    FILE *users_f = fopen(USERS_FILE, "a+");
    if (users_f) fclose(users_f); else { perror("Could not open users file."); return 1; }
    FILE *records_f = fopen(RECORDS_FILE, "a+");
    if (records_f) fclose(records_f); else { perror("Could not open records file."); return 1; }

    initMenu(&u);
    mainMenu(u);
    return 0;
}
