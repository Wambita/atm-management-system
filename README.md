
# ATM Management System

## Objective

This project demonstrates proficiency in C programming, focusing on core programming logic, file manipulation, and data structures. It's an ATM management system where users can perform various banking operations, with several key features implemented and opportunities for further enhancement.

## Features

The ATM Management System provides the following functionalities:

### Implemented Features

* **User Authentication:**
    * **Login:** Users can log into their existing accounts.
    * **Registration:** New users can register, with a uniqueness check to prevent duplicate usernames.
* **Account Management:**
    * **Create a New Account:** Users can open new bank accounts (savings, current, fixed deposits). Includes validation to prevent duplicate account numbers for a given user.
    * **Check List of Owned Accounts:** View all accounts associated with the logged-in user.
    * **Update Account Information:** Modify existing account details such as phone number or country.
    * **Check Details of Existing Accounts:** View detailed information for a single account, including calculated interest based on account type (savings, fixed deposits). Current accounts do not earn interest.
    * **Make Transactions:** Deposit or withdraw money from accounts. Fixed deposit accounts (`fixed01`, `fixed02`, `fixed03`) are restricted from making transactions.
    * **Remove Existing Account:** Delete an account owned by the user.
    * **Transfer Ownership:** Transfer an account to another existing user in the system.

### Bonus Features (Opportunities for Expansion)

* **Real-time Transfer Notification:** Implement a mechanism (e.g., using pipes and child processes) to instantly alert a user when an account ownership is transferred to them.
* **Improved Terminal Interface (TUI):** Enhance the visual presentation and user experience in the terminal.
* **Password Encryption:** Secure user passwords by encrypting them before storage.
* **Relational Database Integration:** Replace flat-file storage (`users.txt`, `records.txt`) with a more robust database solution like SQLite.

---

## File System Structure

The project follows a modular structure:

```

.
|
├── data
│   ├── records.txt         \# Stores information about user accounts
│   └── users.txt           \# Stores user login credentials (ID, username, password)
├── Makefile                \# Automates compilation and cleanup
└── src
├── auth.c              \# Handles user authentication (login, registration)
├── header.h            \# Contains shared structures, constants, and function prototypes
├── main.c              \# Main program entry point and menu navigation
└── system.c            \# Implements core banking system functionalities (account creation, transactions, etc.)

```

---

## Data File Formats

### `users.txt` (ID, Username, Password)

Each line represents a user:

```

[ID] [Username] [Password]

```

Example:

```

0 Alice 1234password
1 Michel password1234

```

### `records.txt` (ID, User ID, User Name, Account ID, Date of Creation, Country, Phone Number, Balance, Type of Account)

Each line represents a bank account:

```

[Record\_ID] [User\_ID] [User\_Name] [Account\_ID] [MM/DD/YYYY] [Country] [Phone\_Number] [Balance] [Account\_Type]

```

Example:

```

0 0 Alice 0 10/02/2020 german 986134231 11090830.00 current
1 1 Michel 2 10/10/2021 portugal 914134431 1920.42 savings

````

---

## Installation and Setup

To get this project running on your local machine, follow these steps:

### Prerequisites

Make sure you have the following installed on your system:

* **Git:** For cloning the repository.
* **C Compiler:** Such as **GCC** (GNU Compiler Collection).
* **Make Utility:** For automating the build process.

### Steps to Install

1.  **Clone the Repository:**
    Open your terminal and clone the project from GitHub using the provided link:
    ```bash
    git clone [https://github.com/Wambita/atm-management-system.git](https://github.com/Wambita/atm-management-system.git)
    ```
2.  **Navigate to the Project Directory:**
    ```bash
    cd atm-management-system
    ```
3.  **Compile the Project:**
    Execute the `make` command to compile the source code:
    ```bash
    make
    ```
    This command will read the `Makefile` and create an executable file named `atm` in the root directory.

---

## How to Run

After successful compilation, run the executable from the project's root directory:

```bash
./atm
````

## How to Use

Upon running the application, you'll be presented with a main menu. Here's a basic flow to get started:

1.  **Register a New User:**

      * Choose option `2` (**Register**).
      * Enter a **unique username** and a **password** when prompted.
      * The system will confirm successful registration.

2.  **Login:**

      * Choose option `1` (**Login**).
      * Enter your registered username and password.
      * Upon successful login, you will be directed to the main ATM menu.

3.  **Create a New Account:**

      * From the main ATM menu, choose option `1` (**Create a new account**).
      * Follow the prompts to enter details like the current date (mm/dd/yyyy), a unique account number, country, phone number, initial deposit amount, and the type of account (savings, current, fixed01, fixed02, or fixed03).
      * The system will validate the account number to ensure it's unique for your user.

4.  **Check List of Owned Accounts:**

      * From the main ATM menu, choose option `4` (**Check list of owned accounts**).
      * This will display a list of all accounts you currently own, along with their details.

5.  **Explore Other Features:**

      * **Update Account Information** (`2`): Modify contact details for a specific account.
      * **Check Details of Existing Accounts** (`3`): Get detailed info, including interest projections, for a single account by its number.
      * **Make Transaction** (`5`): Deposit or withdraw funds from your accounts (fixed accounts are excluded).
      * **Remove Existing Account** (`6`): Delete one of your accounts.
      * **Transfer Ownership** (`7`): Assign one of your accounts to another registered user.

6.  **Exit:**

      * To exit the application at any time, choose option `8` from the main ATM menu, or `0` from the success/retry prompts.

-----

## Contributions

Contributions are welcome\! If you find bugs or want to implement new features, feel free to fork the repository and submit pull requests.

## Author

  * [**Wambita**](https://github.com/Wambita) - Initial work and feature implementations

-----

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
