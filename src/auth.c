#include "header.h"

// Removed constant definitions as they are now in header.h
// Also removed local definition of USERS_FILE as it's now global in main.c and declared in header.h

// loginMenu() collects user credentials (password is now visible)
void loginMenu(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE])
{
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    printf("\n\t\tEnter Username: ");
    scanf("%s", a);

    printf("\n\t\tEnter the password to login: ");
    scanf("%s", pass); // Password will be visible
}

// getPassword() reads stored password for a user from file and copies it to buffer
void getPassword(struct User u, char *buffer) {
  FILE *fp;
  struct User userChecker;
  char id_str[MAX_ID_SIZE]; // Temporary buffer to read ID as string

  if ((fp = fopen(USERS_FILE, "r")) == NULL) {
    perror("\n\t\tError opening users file");
    exit(1);
  }

  // Read ID as string, then username and password
  while (fscanf(fp, "%s %s %s", id_str, userChecker.name, userChecker.password) != EOF) {
    if (strcmp(userChecker.name, u.name) == 0) {
      strcpy(buffer, userChecker.password);
      fclose(fp);
      return;
    }
  }

  fclose(fp);
  strcpy(buffer, "no user found");
}

// isUsernameUnique() checks if the username already exists
int isUsernameUnique(char username[]){
    FILE *fp;
    struct User userChecker;
    char id_str[MAX_ID_SIZE];

    if ((fp = fopen(USERS_FILE, "r")) == NULL) {
        perror("\n\t\tError opening users file");
        exit(1);
    }

    while (fscanf(fp, "%s %s %s", id_str, userChecker.name, userChecker.password) != EOF) {
        if (strcmp(userChecker.name, username) == 0) {
        fclose(fp);
        return 0;
    }
  }
  fclose(fp);
  return 1;
}

// registerMenu() handles new user registration, enforces unique usernames, and saves data
void registerMenu(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE]){

  do {
    system("clear");
    printf("\n\n\t\tBank Management System\n\n\t\t  User Registration\n");
    printf("\n\n\t\tEnter Username: ");
    scanf("%s", a);

    if (!isUsernameUnique(a)) {
      system("clear");
      printf("\n\n\t\tBank Management System\n\n\t\t  User Registration\n");
      printf("\n\t\tUsername '%s' already exists. \n", a);
      handleFailedRegistration(a, pass);
    }
  } while (!isUsernameUnique(a));

  printf("\n\t\tEnter Password: ");
  scanf("%s", pass);

  FILE *fp;
  struct User userChecker;
  char id_str[MAX_ID_SIZE];
  int lastUserID = -1;

  if ((fp = fopen(USERS_FILE, "r")) == NULL) {
    perror("\n\t\tError opening users file for ID check");
    exit(1);
  }

  while (fscanf(fp, "%s %s %s", id_str, userChecker.name, userChecker.password) != EOF) {
    lastUserID = atoi(id_str);
  }
  fclose(fp);

  fp = fopen(USERS_FILE, "a");
  if (fp == NULL) {
    perror("\n\t\tError opening users file for appending");
    exit(1);
  }

  lastUserID++;
  fprintf(fp, "%d %s %s\n", lastUserID, a, pass);
  fclose(fp);

  printf("\n\t\t✔ Registration successful for user '%s'!\n", a);
}

// handleFailedLogin() gives users retry or exit options after failed login
void handleFailedLogin(struct User *u){
    int option;
    do {
        printf("\n\t\tEnter 0 to try again, 1 to exit: ");
        scanf("%d", &option);

        if (option == 0) {
            initMenu(u);
            return;
        } else if (option == 1) {
            exit(1);
        } else {
            printf("\t\tInsert a valid operation!\n");
        }
    } while (option < 0 || option > 1);
}

// handleFailedRegistration() gives retry or exit option on registration failure
void handleFailedRegistration(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE]) {
    int option;
    do {
        printf("\n\t\tEnter 0 to try again, 1 to exit: ");
        scanf("%d", &option);
        if (option == 0) {
            return;
        } else if (option == 1) {
            exit(1);
        } else {
            printf("\t\tInsert a valid operation!\n");
        }
    } while (option < 0 || option > 1);
}

// getUserId: Looks up a user ID from the users file based on username
int getUserId(const char *username) {
  FILE *fp = fopen(USERS_FILE, "r");
  if (!fp) {
    perror("\n\t\tError opening users file");
    exit(1);
  }

  char id_str[MAX_ID_SIZE];
  char name[MAX_USERNAME_SIZE];
  char pass[MAX_PASSWORD_SIZE];

  while (fscanf(fp, "%s %s %s", id_str, name, pass) != EOF) {
    if (strcmp(name, username) == 0) {
      fclose(fp);
      return atoi(id_str);
    }
  }

  fclose(fp);
  return -1;
}
