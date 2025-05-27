#include <termios.h>
#include "header.h"

//constants
#define MAX_USERNAME_SIZE 50
#define MAX_PASSWORD_SIZE 50
#define MAX_ID_SIZE 5

char *USERS = "./data/users.txt";

void mainMenu(struct User u);

// loginMenu() collects user credentials with hidden password input
void loginMenu(char a[MAX_USERNAME_SIZE], char pass[MAX_PASSWORD_SIZE])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }

    printf("\n\t\tEnter the password: ");
    scanf("%s", pass);

    //restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0){
        perror("tcsetattr");
        return exit(1);
    }
};

// getPassword() reads stored password for a user from file
const char *getPassword(struct User u) {
  FILE *fp;
  struct User userChecker;
  char id[MAX_ID_SIZE];

  if ((fp = fopen("./data/users.txt", "r")) == NULL) {
    printf("Error! opening file");
    exit(1);
  }

  while (fscanf(fp, "%s %s %s", id, userChecker.name, userChecker.password) != EOF) {
    if (strcmp(userChecker.name, u.name) == 0) {
      fclose(fp);
      u.id = atoi(id); 
      char *buff = userChecker.password;
      return buff;
    }
  }

  fclose(fp);
  return "no user found"; 
}

//isUsernameUnique() check if  the username already exists
int isUsernameUnique(char username[]){
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF) {
        if (strcmp(userChecker.name, username) == 0) {
        fclose(fp);
        return 0; 
    }
  }
  fclose(fp);
  return 1; 
}