#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define MAX_USERNAME_SIZE 50
#define MAX_PASSWORD_SIZE 50
#define MAX_COUNTRY_SIZE 100
#define MAX_TRANSACTION_TYPE_SIZE 10
#define MAX_RECORDS 100
#define MAX_ID_SIZE 5

// File paths - Declared as extern to avoid multiple definitions
extern const char *USERS_FILE;
extern const char *RECORDS_FILE;

// Structure for date (used in account records)
struct Date
{
    int month, day, year;
};

// Structure for an account record
struct Record
{
    int id;
    int userId;
    char name[MAX_USERNAME_SIZE];
    char country[MAX_COUNTRY_SIZE];
    int phone;
    char accountType[MAX_TRANSACTION_TYPE_SIZE];
    int accountNbr;
    double amount;
    struct Date deposit;
};

// Structure for a user
struct User
{
    int id;
    char name[MAX_USERNAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
};

// --- Authentication Functions ---
void initMenu(struct User *u);
void loginMenu(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE]);
void registerMenu(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE]);
void getPassword(struct User u, char *buffer);
void handleFailedLogin(struct User *u);
void handleFailedRegistration(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE]);
int isUsernameUnique(char username[]);
int getUserId(const char *username);


// --- System Functions ---
void mainMenu(struct User u);
void stayOrReturn(int notGood, void (*f)(struct User), struct User u);
void success(struct User u);
int getAccountFromFile(FILE *ptr, struct Record *r);
void saveAccountToFile(FILE *ptr, struct User u, struct Record r);
void updateUserAccountInFile(FILE *ptr, struct Record r);
int doesUserHaveAccounts(struct User u);
void createNewAcc(struct User u);
void checkAccountDetails(struct User u);
void checkAllAccounts(struct User u);
void updateAccountInformation(struct User u);
void makeTransaction(struct User u);
void removeAccount(struct User u);
void transferOwnership(struct User u);


#endif // HEADER_H
