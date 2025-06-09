#include "header.h"

//constants
#define MAX_RECORDS 100
#define MAX_USERNAME_SIZE 50
#define MAX_PASSWORD_SIZE 50
#define MAX_ID_SIZE 5
#define MAX_COUNTRY_SIZE 100
#define MAX_TRANSACTION_TYPE_SIZE 10

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[MAX_USERNAME_SIZE], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                r.id,
	            u.id,
	         u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

//update user account function
void updateUserAccountInFile(FILE *ptr, struct Record r) {
  fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n", r.id, r.userId,
          r.name, r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year,
          r.country, r.phone, r.amount, r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
   int option;
  do {
    if (notGood == 0) {
      printf("\n\t\tEnter 0 to try again, 1 to return to main menu and 2 to "
             "exit! \n\n");
    } else {
      printf("\n\t\tEnter 1 to go to the main menu and 0 to exit! \n\n");
    }

    scanf("%d", &option);

    if (option == 0) {
      f(u);
      break;
    } else if (option == 1) {
      mainMenu(u);
      break;
    } else if (option == 2) {
      exit(0);
    } else {
      printf("Insert a valid operation!\n");
    }
  } while (option < 0 || option > 2);
}


// Looks up a user ID from the users file based on usernameint 
getUserId(const char *username) {
  FILE *fp = fopen("./data/users.txt", "r");
  if (!fp) {
    perror("\n\t\tError opening file");
    exit(1);
  }

  char id[MAX_ID_SIZE], name[MAX_USERNAME_SIZE], pass[MAX_PASSWORD_SIZE];
  while (fscanf(fp, "%s %s %s", id, name, pass) != EOF) {
    if (strcmp(name, username) == 0) {
      fclose(fp);
      return atoi(id);
    }
  }

  fclose(fp);
  return -1;
} 


//does  user have account function
int doesUserHaveAccounts(struct User u) {
  struct Record r;
  FILE *pf = fopen(RECORDS, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open file");
    return 0;
  }
  char userName[MAX_USERNAME_SIZE];
  while (getAccountFromFile(pf, userName, &r)) {
    if (strcmp(userName, u.name) == 0) {
      fclose(pf);
      return 1;
    }
  }
  fclose(pf);
  return 0;
}
//// Success screen with options to return to menu or exit
void success(struct User u)
{
    int option;
    while(1) {
    printf("\n\t\tEnter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("\n\t\tInsert a valid operation!\n");
    }
    }
}

//create new account
void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[MAX_USERNAME_SIZE];
    FILE *pf = fopen(RECORDS, "a+");
    FILE *rf = fopen(RECORDS, "r");

   if (!rf) {
    perror("\n\t\tError opening file");
    exit(1);
  }

  int lastRecordId = 0;
  while (fscanf(rf, "%d %*d %*s %*d %*s %*s %*d %*lf %*s", &r.id) != EOF) {
    lastRecordId = r.id;
  }

  fclose(rf);

do{
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);
     int accountExists = 0;

    rewind(pf);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("\n\t✖ This Account already exists for this user\n\n");
           int accountExists = 1;
           break;
        }
    }
     if (accountExists) {
      fclose(pf);
      stayOrReturn(0, createNewAcc, u);
    } else {
      r.id = lastRecordId + 1;
      u.id = getUserId(u.name);
      printf("\n\t\tEnter the country: ");
      scanf("%s", r.country);

      printf("\n\t\tEnter the phone number: ");
      scanf("%d", &r.phone);

      printf("\n\t\tEnter amount to deposit: $");
      scanf("%lf", &r.amount);

      printf("\n\t\tChoose the type of account:\n\n\t\t -> savings\n\t\t -> "
             "current\n\t\t -> fixed01(for 1 year)\n\t\t -> fixed02(for 2 "
             "years)\n\t\t -> "
             "fixed03(for 3 "
             "years)\n\n\t\tEnter your choice: ");
      scanf("%s", r.accountType);

      saveAccountToFile(pf, u, r);

      fclose(pf);
      printf("\n\t\t✔ Success!\n");
      success(u);
      break;
    }
  } while (1);

}

//check accounts
void checkAllAccounts(struct User u)
{
  if (!doesUserHaveAccounts(u)) {
    system("clear");
    printf("\n\t\tNo accounts found for %s. Returning to main menu.\n", u.name);
    stayOrReturn(1, checkAllAccounts, u);
    return;
  }

    char userName[MAX_USERNAME_SIZE];
    int accountsFound = 0;
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL) {
    perror("\n\t\tFailed to open file");
    return;
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);


    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
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
    success(u);
}
