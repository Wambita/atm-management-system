#include "header.h"

// Removed constant definitions as they are now in header.h
// Also removed local definition of RECORDS_FILE as it's now global in main.c and declared in header.h

// getAccountFromFile function: Reads a single account record from the file into a Record struct
int getAccountFromFile(FILE *ptr, struct Record *r)
{
    // The fscanf format string must exactly match the data format in records.txt
    // %d (id) %d (userId) %s (userName) %d (accountNbr) %d/%d/%d (date) %s (country) %d (phone) %lf (amount) %s (accountType)
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  r->name, // Directly read into r->name
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

// saveAccountToFile: Saves a new account record to the file
void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    // Ensure the fprintf format matches the fscanf in getAccountFromFile for consistency
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n", // Added newline at the end
            r.id,           // Record ID
            u.id,           // User ID (from logged-in user)
            u.name,         // User Name (from logged-in user)
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

// updateUserAccountInFile: Rewrites a single updated account record to the file
void updateUserAccountInFile(FILE *ptr, struct Record r) {
  // Ensure the fprintf format matches getAccountFromFile
  fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
          r.id, r.userId, r.name, // Use record's own userId and name for consistency
          r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year,
          r.country, r.phone, r.amount, r.accountType);
}

// stayOrReturn: Gives user options to retry, return to main menu, or exit
void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
   int option;
   while(1) {
    if (notGood == 0) {
      printf("\n\t\tEnter 0 to try again, 1 to return to main menu and 2 to exit: ");
    } else {
      printf("\n\t\tEnter 1 to go to the main menu and 0 to exit: ");
    }

    if (scanf("%d", &option) != 1) {
        printf("\n\t\tInvalid input. Please enter a number.\n");
        while (getchar() != '\n');
        continue;
    }

    if (notGood == 0) {
        if (option == 0) {
            f(u);
            return;
        } else if (option == 1) {
            mainMenu(u);
            return;
        } else if (option == 2) {
            exit(0);
        } else {
            printf("\t\tInsert a valid operation!\n");
        }
    } else {
        if (option == 1) {
            mainMenu(u);
            return;
        } else if (option == 0) {
            exit(0);
        } else {
            printf("\t\tInsert a valid operation!\n");
        }
    }
  }
}

// doesUserHaveAccounts: Checks if the given user has any accounts
int doesUserHaveAccounts(struct User u) {
  struct Record r;
  FILE *pf = fopen(RECORDS_FILE, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open records file");
    return 0;
  }

  while (getAccountFromFile(pf, &r)) {
    if (strcmp(r.name, u.name) == 0) {
      fclose(pf);
      return 1;
    }
  }
  fclose(pf);
  return 0;
}

// success: Displays a success message and offers return to main menu or exit
void success(struct User u)
{
    int option;
    while(1) {
        printf("\n\t\tEnter 1 to go to the main menu and 0 to exit: ");
        if (scanf("%d", &option) != 1) {
            printf("\n\t\tInvalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        system("clear");
        if (option == 1)
        {
            mainMenu(u);
            return;
        }
        else if (option == 0)
        {
            exit(0);
        }
        else
        {
            printf("\n\t\tInsert a valid operation!\n");
        }
    }
}

// createNewAcc: Creates a new bank account for the logged-in user
void createNewAcc(struct User u)
{
    struct Record r;
    struct Record tempRecord;

    int lastRecordId = -1;
    int accountExists = 0;

    FILE *rf_for_id = fopen(RECORDS_FILE, "r");
    if (!rf_for_id) {
        perror("\n\t\tWarning: Could not open records file to get last ID. Assuming new ID starts from 0.");
    } else {
        while (getAccountFromFile(rf_for_id, &tempRecord)) {
            if (tempRecord.id > lastRecordId) {
                lastRecordId = tempRecord.id;
            }
        }
        fclose(rf_for_id);
    }

    while (1) {
        system("clear");
        printf("\t\t\t===== New record =====\n");

        printf("\n\t\tEnter today's date (mm/dd/yyyy): ");
        if (scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year) != 3) {
            printf("\n\t\tInvalid date format. Please use mm/dd/yyyy.\n");
            while (getchar() != '\n');
            stayOrReturn(0, createNewAcc, u);
            return;
        }
        while (getchar() != '\n'); // Consume the newline character

        printf("\n\t\tEnter the account number: ");
        if (scanf("%d", &r.accountNbr) != 1) {
            printf("\n\t\tInvalid account number. Please enter a number.\n");
            while (getchar() != '\n');
            stayOrReturn(0, createNewAcc, u);
            return;
        }
        while (getchar() != '\n'); // Consume the newline character

        accountExists = 0;

        FILE *pf_check = fopen(RECORDS_FILE, "r");
        if (!pf_check) {
            perror("\n\t\tError opening file for account existence check");
            exit(1);
        }

        while (getAccountFromFile(pf_check, &tempRecord)) {
            if (strcmp(tempRecord.name, u.name) == 0 && tempRecord.accountNbr == r.accountNbr) {
                accountExists = 1;
                break;
            }
        }
        fclose(pf_check);

        if (accountExists) {
            system("clear");
            printf("\n\t✖ This Account number %d already exists for %s.\n\n", r.accountNbr, u.name);
            stayOrReturn(0, createNewAcc, u);
            return;
        } else {
            r.id = lastRecordId + 1;
            r.userId = u.id;
            strcpy(r.name, u.name);

            printf("\n\t\tEnter the country: ");
            scanf("%s", r.country);
            while (getchar() != '\n'); // Consume the newline character

            printf("\n\t\tEnter the phone number: ");
            if (scanf("%d", &r.phone) != 1) {
                printf("\n\t\tInvalid phone number. Please enter a number.\n");
                while (getchar() != '\n');
                stayOrReturn(0, createNewAcc, u);
                return;
            }
            while (getchar() != '\n'); // Consume the newline character

            printf("\n\t\tEnter amount to deposit: $");
            if (scanf("%lf", &r.amount) != 1) {
                printf("\n\t\tInvalid amount. Please enter a number.\n");
                while (getchar() != '\n');
                stayOrReturn(0, createNewAcc, u);
                return;
            }
            while (getchar() != '\n'); // Consume the newline character

            printf("\n\t\tChoose the type of account:\n\n\t\t -> savings\n\t\t -> "
                   "current\n\t\t -> fixed01(for 1 year)\n\t\t -> fixed02(for 2 "
                   "years)\n\t\t -> "
                   "fixed03(for 3 "
                   "years)\n\n\t\tEnter your choice: ");
            scanf("%s", r.accountType);
            while (getchar() != '\n'); // Consume the newline character

            FILE *pf_save = fopen(RECORDS_FILE, "a");
            if (!pf_save) {
                perror("\n\t\tError opening file for saving new account");
                exit(1);
            }
            saveAccountToFile(pf_save, u, r);
            fclose(pf_save);

            printf("\n\t\t✔ Account created successfully!\n");
            success(u);
            return;
        }
    }
}

// checkAllAccounts: Displays a list of all accounts owned by the current user
void checkAllAccounts(struct User u)
{
  if (!doesUserHaveAccounts(u)) {
    system("clear");
    printf("\n\t\tNo accounts found for %s. Returning to main menu.\n", u.name);
    stayOrReturn(1, checkAllAccounts, u);
    return;
  }

    int accountsFoundForUser = 0;
    struct Record r;

    FILE *pf = fopen(RECORDS_FILE, "r");
    if (pf == NULL) {
    perror("\n\t\tFailed to open records file");
    return;
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);

    while (getAccountFromFile(pf, &r))
    {
        if (strcmp(r.name, u.name) == 0)
        {
          accountsFoundForUser = 1;
            printf("_____________________\n");
            printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \nCountry: %s \nPhone number: %d \nAmount deposited: $%.2f \nType Of Account: %s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);

    if (accountsFoundForUser == 0) {
        printf("\n\t\tNo accounts found for %s.\n", u.name);
        stayOrReturn(1, checkAllAccounts, u);
        return;
    }
    success(u);
}

// updateAccountInformation: Updates information (phone or country) of an existing account
void updateAccountInformation(struct User u) {
  if (!doesUserHaveAccounts(u)) {
    system("clear");
    printf("\n\t\tNo accounts found for %s. Returning to main menu.\n", u.name);
    stayOrReturn(1, updateAccountInformation, u);
    return;
  }

  int accountNbr, choice;
  int newPhoneNumber;
  char newCountry[MAX_COUNTRY_SIZE];
  int found = 0;

  system("clear");
  printf("\t\t====== Update accounts for %s =====\n\n", u.name);
  printf("\n\t\tEnter the account number you wish to update: ");
  if (scanf("%d", &accountNbr) != 1) {
    printf("\n\t\tInvalid account number. Please enter a number.\n");
    while (getchar() != '\n');
    stayOrReturn(0, updateAccountInformation, u);
    return;
  }
  while (getchar() != '\n'); // Consume newline after scanf

  struct Record records[MAX_RECORDS];
  int recordCount = 0;

  FILE *pf = fopen(RECORDS_FILE, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open records file");
    return;
  }

  while (getAccountFromFile(pf, &records[recordCount])) {
    if (records[recordCount].accountNbr == accountNbr && strcmp(records[recordCount].name, u.name) == 0){
      found = 1;
    }
    recordCount++;
    if (recordCount >= MAX_RECORDS) {
        printf("\n\t\tWarning: Maximum records loaded. Some accounts might be missed.\n");
        break;
    }
  }
  fclose(pf);

  if(!found) {
    printf("\n\t\t✖ No account found with account number %d for user %s.\n", accountNbr, u.name);
    stayOrReturn(0, updateAccountInformation, u);
    return;
  }

  printf("\n\t\tWhich field do you want to update?\n\n\t\t 1. Phone "
         "number\n\n\t\t 2. Country\n\n\t\tEnter choice (1/2): ");
  if (scanf("%d", &choice) != 1) {
    printf("\n\t\tInvalid choice. Please enter 1 or 2.\n");
    while (getchar() != '\n');
    stayOrReturn(0, updateAccountInformation, u);
    return;
  }
  while (getchar() != '\n'); // Consume newline after scanf

  switch (choice) {
  case 1:
    printf("\n\t\tEnter new phone number: ");
    if (scanf("%d", &newPhoneNumber) != 1) {
        printf("\n\t\tInvalid phone number. Please enter a number.\n");
        while (getchar() != '\n');
        stayOrReturn(0, updateAccountInformation, u);
        return;
    }
    while (getchar() != '\n'); // Consume newline after scanf
    for (int i = 0; i < recordCount; i++) {
      if (records[i].accountNbr == accountNbr &&
          strcmp(records[i].name, u.name) == 0) {
        records[i].phone = newPhoneNumber;
        printf("\n\t\tPhone number updated in memory.\n");
        break;
      }
    }
    break;
  case 2:
     printf("\n\t\tEnter new country: ");
    scanf("%s", newCountry);
    while (getchar() != '\n'); // Consume newline after scanf
    for (int i = 0; i < recordCount; i++) {
      if (records[i].accountNbr == accountNbr &&
          strcmp(records[i].name, u.name) == 0) {
        strcpy(records[i].country, newCountry);
        printf("\n\t\tCountry updated in memory.\n");
        break;
      }
    }
    break;
  default:
    printf("\n\t\tInvalid choice. Returning to main menu.\n");
    mainMenu(u);
    return;
  }

pf = fopen(RECORDS_FILE, "w");
if (pf == NULL) {
  perror("\n\t\tFailed to open the records file for writing");
  return;
}

for (int i = 0; i < recordCount; i++)
{
  updateUserAccountInFile(pf, records[i]);
}
fclose(pf);

printf("\n\t\t✔ Account information updated successfully.\n");
success(u);
}

// checkAccountDetails: Checks and displays details for a specific account, including interest
void checkAccountDetails(struct User u) {
  if(!doesUserHaveAccounts(u)) {
    system("clear");
    printf("\n\t\tNo accounts found for %s. Returning to main menu.\n", u.name);
    stayOrReturn(1, checkAccountDetails, u);
    return;
  }

  int accountNbr;
  struct Record r;
  int found =0;

  system("clear");
  printf("\t\t====== Check Account Details for %s =====\n\n", u.name);
  printf("\n\t\tEnter the account number you wish to check: ");
  if (scanf("%d", &accountNbr) != 1) {
    printf("\n\t\tInvalid account number. Please enter a number.\n");
    while (getchar() != '\n');
    stayOrReturn(0, checkAccountDetails, u);
    return;
  }
  while (getchar() != '\n'); // Consume newline after scanf

  FILE *pf = fopen(RECORDS_FILE, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open records file");
    return;
  }

  while (getAccountFromFile(pf, &r)) {
    if (r.accountNbr == accountNbr && strcmp(r.name, u.name) == 0) {
      found = 1;
      break;
    }
  }
  fclose(pf);

  if (!found) {
    printf("\n\t\t✖ No account found with account number %d.\n", accountNbr);
    stayOrReturn(0, checkAccountDetails, u);
    return;
  }

  printf("\n\t\tAccount number: %d\n\t\tDeposit date: %d/%d/%d\n\t\tCountry: "
         "%s \n\t\tPhone number: %d \n\t\tAmount deposited: $%.2f \n\t\tType "
         "of account: %s\n",
         r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year,
         r.country, r.phone, r.amount, r.accountType);

  double rate;
  if (strcmp(r.accountType, "savings") == 0) {
    rate = 0.07;
    double interest = r.amount * (1 + rate / 12) - r.amount;
    printf("\n\t\tYou will get $%.2f as interest on day %d of every month.\n",
           interest, r.deposit.day);
  }
  else if (strcmp(r.accountType, "fixed01") == 0) {
    rate = 0.04;
    double interest = r.amount * (1 + rate / 12) - r.amount;
    interest *= 12;
    printf("\n\t\tYou will get $%.2f as interest on %d/%d/%d.\n", interest,
           r.deposit.month, r.deposit.day, r.deposit.year + 1);
  }
  else if (strcmp(r.accountType, "fixed02") == 0) {
    rate = 0.05;
    double interest = r.amount * (1 + rate / 12) - r.amount;
    interest *= 24;
    printf("\n\t\tYou will get $%.2f as interest on %d/%d/%d.\n", interest,
           r.deposit.month, r.deposit.day, r.deposit.year + 2);
  }
  else if (strcmp(r.accountType, "fixed03") == 0) {
    rate = 0.08;
    double interest = r.amount * (1 + rate / 12) - r.amount;
    interest *= 36;
    printf("\n\t\tYou will get $%.2f as interest on %d/%d/%d.\n", interest,
           r.deposit.month, r.deposit.day, r.deposit.year + 3);
  }
  else if (strcmp(r.accountType, "current") == 0) {
    printf("\n\t\tYou will not get interests because the account is of type "
           "current.\n");
  }
  else {
    printf("\n\t\tInvalid account type for interest calculation.\n");
    stayOrReturn(0, checkAccountDetails, u);
    return;
  }
  success(u);
}

// makeTransaction: Performs deposit or withdrawal transactions
void makeTransaction(struct User u) {
  if (!doesUserHaveAccounts(u)) {
    system("clear");
    printf("\n\t\tNo accounts found for %s. Returning to main menu.\n", u.name);
    stayOrReturn(1, makeTransaction, u);
    return;
  }

  int accountNbr;
  double amount;
  char transactionType[MAX_TRANSACTION_TYPE_SIZE];

  int found = 0;

  system("clear");
  printf("\t\t====== Make Transaction for %s =====\n\n", u.name);

  printf("\n\t\tEnter the account number for the transaction: ");
  if (scanf("%d", &accountNbr) != 1) {
    printf("\n\t\tInvalid account number. Please enter a number.\n");
    while (getchar() != '\n');
    stayOrReturn(0, makeTransaction, u);
    return;
  }
  while (getchar() != '\n'); // Consume newline after scanf

  struct Record records[MAX_RECORDS];
  int recordCount = 0;

  FILE *pf = fopen(RECORDS_FILE, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open records file");
    return;
  }

  while (getAccountFromFile(pf, &records[recordCount])) {
    if (records[recordCount].accountNbr == accountNbr &&
        strcmp(records[recordCount].name, u.name) == 0) {
      found = 1;
    }
    recordCount++;
    if (recordCount >= MAX_RECORDS) {
        printf("\n\t\tWarning: Maximum records loaded. Some accounts might be missed.\n");
        break;
    }
  }
  fclose(pf);

  if (!found) {
    printf("\n\t\t✖ No account found with account number %d.\n", accountNbr);
    stayOrReturn(0, makeTransaction, u);
    return;
  }

  char *restrictedAccountTypes[] = {"fixed01", "fixed02", "fixed03"};
  for (int i = 0; i < recordCount; i++) {
    if (records[i].accountNbr == accountNbr &&
        strcmp(records[i].name, u.name) == 0) {
      for (int j = 0; j < 3; ++j) {
        if (strcmp(records[i].accountType, restrictedAccountTypes[j]) == 0) {
          printf("\n\t\t✖ Accounts of type %s are not allowed to make "
                 "transactions.\n",
                 records[i].accountType);
          stayOrReturn(0, makeTransaction, u);
          return;
        }
      }
      break;
    }
  }

  printf("\n\t\tEnter the transaction type (deposit/withdraw): ");
  scanf("%s", transactionType);
  while (getchar() != '\n'); // Consume newline after scanf

  printf("\n\t\tEnter the amount: $");
  if (scanf("%lf", &amount) != 1) {
    printf("\n\t\tInvalid amount. Please enter a number.\n");
    while (getchar() != '\n');
    stayOrReturn(0, makeTransaction, u);
    return;
  }
  while (getchar() != '\n'); // Consume newline after scanf

   for (int i = 0; i < recordCount; i++) {
    if (records[i].accountNbr == accountNbr &&
        strcmp(records[i].name, u.name) == 0) {
      if (strcmp(transactionType, "withdraw") == 0) {
        if (records[i].amount < amount) {
          printf("\n\t\t✖ Not enough balance for withdrawal. Current balance: $%.2f\n", records[i].amount);
          stayOrReturn(0, makeTransaction, u);
          return;
        }
        records[i].amount -= amount;
        printf("\n\t\tWithdrawal processed. New balance: $%.2f.\n", records[i].amount);
      } else if (strcmp(transactionType, "deposit") == 0) {
        records[i].amount += amount;
        printf("\n\t\tDeposit processed. New balance: $%.2f.\n", records[i].amount);
      } else {
        printf("\n\t\t✖ Invalid transaction type. Please enter 'deposit' or 'withdraw'.\n");
        stayOrReturn(0, makeTransaction, u);
        return;
      }
      break;
    }
  }

  pf = fopen(RECORDS_FILE, "w");
  if (pf == NULL) {
    perror("\n\t\tFailed to open records file for writing");
    return;
  }
  for (int i = 0; i < recordCount; i++) {
    updateUserAccountInFile(pf, records[i]);
  }
  fclose(pf);

  printf("\n\t\t✔ Transaction successful.\n");
  success(u);
}

// removeAccount: Removes an existing account
void removeAccount(struct User u){
  if (!doesUserHaveAccounts(u)) {
    system("clear");
    printf("\n\t\tNo accounts found for %s. Returning to main menu.\n", u.name);
    stayOrReturn(1, removeAccount, u);
    return;
  }

  int accountNbr;
  struct Record records[MAX_RECORDS];
  int recordCount = 0;
  int found = 0;

  system("clear");
  printf("\t\t====== Remove Account for %s =====\n\n", u.name);
  printf("\n\t\tEnter the account number you wish to remove: ");
  if (scanf("%d", &accountNbr) != 1) {
    printf("\n\t\tInvalid account number. Please enter a number.\n");
    while (getchar() != '\n');
    stayOrReturn(0, removeAccount, u);
    return;
  }
  while (getchar() != '\n'); // Consume newline after scanf

  FILE *pf = fopen(RECORDS_FILE, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open records file");
    exit(1);
  }

  while (getAccountFromFile(pf, &records[recordCount])) {
    if (records[recordCount].accountNbr == accountNbr &&
        strcmp(records[recordCount].name, u.name) == 0) {
      found = 1;
    }
    recordCount++;
    if (recordCount >= MAX_RECORDS) {
        printf("\n\t\tWarning: Maximum records loaded. Some accounts might be missed.\n");
        break;
    }
  }
  fclose(pf);

   if (!found) {
    printf("\n\t\t✖ No account found with account number %d.\n", accountNbr);
    stayOrReturn(0, removeAccount, u);
    return;
  }

  for (int i = 0; i < recordCount; i++) {
    if (records[i].accountNbr == accountNbr &&
        strcmp(records[i].name, u.name) == 0) {
      for (int j = i; j < recordCount - 1; j++) {
        records[j] = records[j + 1];
      }
      recordCount--;
      break;
    }
  }

  pf = fopen(RECORDS_FILE, "w");
  if (pf == NULL) {
    perror("\n\t\tFailed to open records file for writing");
    exit(1);
  }

  int newId = 0;
  for (int i = 0; i < recordCount; i++) {
    records[i].id = newId++;
    updateUserAccountInFile(pf, records[i]);
  }
  fclose(pf);

  printf("\n\t\t✔ Account removed successfully.\n");
  success(u);
}

// transferOwnership: Transfers ownership of an account to another user
void transferOwnership(struct User u){
  if (!doesUserHaveAccounts(u)) {
    system("clear");
    printf("\n\t\tNo accounts found for %s. Returning to main menu.\n", u.name);
    stayOrReturn(1, transferOwnership, u);
    return;
  }

  int accountNbr;
  struct Record records[MAX_RECORDS];
  int recordCount = 0;
  int found = 0;
  char newUserName[MAX_USERNAME_SIZE];
  int newUserId;


  system("clear");
  printf("\t\t====== Transfer Ownership for %s =====\n\n", u.name);
  printf("\n\t\tEnter the account number you want to transfer: ");
  if (scanf("%d", &accountNbr) != 1) {
    printf("\n\t\tInvalid account number. Please enter a number.\n");
    while (getchar() != '\n');
    stayOrReturn(0, transferOwnership, u);
    return;
  }
  while (getchar() != '\n'); 
  FILE *pf = fopen(RECORDS_FILE, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open records file");
    return;
  }

  while (getAccountFromFile(pf, &records[recordCount])) {
    if (records[recordCount].accountNbr == accountNbr &&
        strcmp(records[recordCount].name, u.name) == 0) {
      found = 1;
    }
    recordCount++;
    if (recordCount >= MAX_RECORDS) {
        printf("\n\t\tWarning: Maximum records reached. Some accounts might not be loaded.\n");
        break;
    }
  }
  fclose(pf);

  if (!found) {
    printf("\n\t\t✖ No account found with account number %d or it does not belong to you.\n", accountNbr);
    stayOrReturn(0, transferOwnership, u);
    return;
  }

  printf("\n\t\tEnter the username of the new owner: ");
  scanf("%s", newUserName);
  while (getchar() != '\n'); 

  newUserId = getUserId(newUserName);

  if (newUserId == -1) {
    printf("\n\t\t New owner '%s' not found in the system.\n", newUserName);
    stayOrReturn(0, transferOwnership, u);
    return;
  }

  for (int i = 0; i < recordCount; i++) {
    if (records[i].accountNbr == accountNbr &&
        strcmp(records[i].name, u.name) == 0) {
      records[i].userId = newUserId;
      strcpy(records[i].name, newUserName);
      strcpy(records[i].country, "N/A");
      records[i].phone = 0;
      break;
    }
  }

  pf = fopen(RECORDS_FILE, "w");
  if (pf == NULL) {
    perror("\n\t\tFailed to open records file for writing");
    return;
  }
   for (int i = 0; i < recordCount; i++) {
    updateUserAccountInFile(pf, records[i]);
  }
  fclose(pf);

  printf("\n\t\t✔ Ownership successfully transferred.\n");
  success(u);
}
