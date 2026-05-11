// ADVANCED BANK MANAGEMENT SYSTEM
// Features Added:
// 1. Create Account
// 2. Deposit
// 3. Withdraw
// 4. Transfer Money
// 5. Search Account
// 6. Display All Accounts
// 7. Update Account
// 8. Delete Account
// 9. Export Accounts to Text File
// 10. Transaction History
// 11. Login System

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[15];
    char phone[15];
    char accountType[15];
    double balance;
};

// function prototypes
void login();
unsigned int enterChoice(void);

void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

void displayAll(FILE *fPtr);
void searchRecord(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void transferMoney(FILE *fPtr);

void transactionHistory();
void saveTransaction(unsigned int acc, char type[], double amount);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    login();

    // open binary file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File does not exist. Creating new file...\n");

        cfPtr = fopen("credit.dat", "wb+");

        struct clientData blankClient = {0, "", "", "", "", 0.0};

        for (int i = 0; i < MAX_ACCOUNTS; i++)
        {
            fwrite(&blankClient, sizeof(struct clientData), 1, cfPtr);
        }

        rewind(cfPtr);
    }

    while ((choice = enterChoice()) != 12)
    {
        switch (choice)
        {
        case 1:
            newRecord(cfPtr);
            break;

        case 2:
            depositMoney(cfPtr);
            break;

        case 3:
            withdrawMoney(cfPtr);
            break;

        case 4:
            transferMoney(cfPtr);
            break;

        case 5:
            searchRecord(cfPtr);
            break;

        case 6:
            displayAll(cfPtr);
            break;

        case 7:
            updateRecord(cfPtr);
            break;

        case 8:
            deleteRecord(cfPtr);
            break;

        case 9:
            textFile(cfPtr);
            break;

        case 10:
            transactionHistory();
            break;

        case 11:
            printf("Logged out successfully.\n");
            login();
            break;

        default:
            printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);

    return 0;
}

// LOGIN SYSTEM
void login()
{
    char username[20];
    char password[20];

    while (1)
    {
        printf("\n========== LOGIN ==========\n");

        printf("Username: ");
        scanf("%19s", username);

        printf("Password: ");
        scanf("%19s", password);

        if (strcmp(username, "admin") == 0 &&
            strcmp(password, "1234") == 0)
        {
            printf("Login Successful!\n");
            break;
        }
        else
        {
            printf("Invalid Username or Password\n");
        }
    }
}

// MENU
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\n=====================================\n");
    printf("      BANK MANAGEMENT SYSTEM\n");
    printf("=====================================\n");

    printf("1. Create New Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Transfer Money\n");
    printf("5. Search Account\n");
    printf("6. Display All Accounts\n");
    printf("7. Update Account\n");
    printf("8. Delete Account\n");
    printf("9. Export to Text File\n");
    printf("10. Transaction History\n");
    printf("11. Logout\n");
    printf("12. Exit\n");

    printf("=====================================\n");
    printf("Enter your choice: ");

    scanf("%u", &menuChoice);

    return menuChoice;
}

// CREATE NEW ACCOUNT
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", "", "", 0.0};

    unsigned int accountNum;

    printf("Enter Account Number (1 - 100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        printf("Enter Last Name: ");
        scanf("%14s", client.lastName);

        printf("Enter First Name: ");
        scanf("%14s", client.firstName);

        printf("Enter Phone Number: ");
        scanf("%14s", client.phone);

        printf("Enter Account Type: ");
        scanf("%14s", client.accountType);

        printf("Enter Balance: ");
        scanf("%lf", &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account Created Successfully.\n");

        saveTransaction(accountNum, "ACCOUNT CREATED", client.balance);
    }
}

// DISPLAY ALL ACCOUNTS
void displayAll(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-6s%-15s%-15s%-15s%-15s%-10s\n",
           "Acct",
           "Last Name",
           "First Name",
           "Phone",
           "Type",
           "Balance");

    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%-6u%-15s%-15s%-15s%-15s%-10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.phone,
                   client.accountType,
                   client.balance);
        }
    }
}

// SEARCH ACCOUNT
void searchRecord(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;

    printf("Enter account number to search: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Found:\n");

        printf("Account Number : %u\n", client.acctNum);
        printf("Last Name     : %s\n", client.lastName);
        printf("First Name    : %s\n", client.firstName);
        printf("Phone         : %s\n", client.phone);
        printf("Account Type  : %s\n", client.accountType);
        printf("Balance       : %.2f\n", client.balance);
    }
}

// DEPOSIT MONEY
void depositMoney(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter deposit amount: ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr,
          -sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Deposit Successful.\n");

    saveTransaction(account, "DEPOSIT", amount);
}

// WITHDRAW MONEY
void withdrawMoney(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Enter withdrawal amount: ");
    scanf("%lf", &amount);

    if (client.balance - amount < 500)
    {
        printf("Minimum balance must be 500.\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr,
          -sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Withdrawal Successful.\n");

    saveTransaction(account, "WITHDRAW", amount);
}

// TRANSFER MONEY
void transferMoney(FILE *fPtr)
{
    struct clientData sender, receiver;

    unsigned int fromAcc, toAcc;
    double amount;

    printf("Enter sender account: ");
    scanf("%u", &fromAcc);

    printf("Enter receiver account: ");
    scanf("%u", &toAcc);

    printf("Enter amount: ");
    scanf("%lf", &amount);

    // sender
    fseek(fPtr,
          (fromAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&sender,
          sizeof(struct clientData),
          1,
          fPtr);

    // receiver
    fseek(fPtr,
          (toAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&receiver,
          sizeof(struct clientData),
          1,
          fPtr);

    if (sender.acctNum == 0 || receiver.acctNum == 0)
    {
        printf("Invalid account.\n");
        return;
    }

    if (sender.balance - amount < 500)
    {
        printf("Insufficient balance.\n");
        return;
    }

    sender.balance -= amount;
    receiver.balance += amount;

    // update sender
    fseek(fPtr,
          (fromAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&sender,
           sizeof(struct clientData),
           1,
           fPtr);

    // update receiver
    fseek(fPtr,
          (toAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&receiver,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Transfer Successful.\n");

    saveTransaction(fromAcc, "TRANSFER SENT", amount);
    saveTransaction(toAcc, "TRANSFER RECEIVED", amount);
}

// UPDATE ACCOUNT
void updateRecord(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;

    printf("Enter account number to update: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("Enter New Phone Number: ");
        scanf("%14s", client.phone);

        printf("Enter New Account Type: ");
        scanf("%14s", client.accountType);

        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Record Updated Successfully.\n");
    }
}

// DELETE ACCOUNT
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account Deleted Successfully.\n");

        saveTransaction(accountNum, "ACCOUNT DELETED", 0);
    }
}

// EXPORT TO TEXT FILE
void textFile(FILE *readPtr)
{
    FILE *writePtr;

    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be opened.\n");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr,
                "%-6s%-15s%-15s%-15s%-15s%-10s\n",
                "Acct",
                "Last Name",
                "First Name",
                "Phone",
                "Type",
                "Balance");

        while (fread(&client,
                      sizeof(struct clientData),
                      1,
                      readPtr) == 1)
        {
            if (client.acctNum != 0)
            {
                fprintf(writePtr,
                        "%-6u%-15s%-15s%-15s%-15s%-10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.phone,
                        client.accountType,
                        client.balance);
            }
        }

        fclose(writePtr);

        printf("Accounts exported to accounts.txt\n");
    }
}

// SAVE TRANSACTION
void saveTransaction(unsigned int acc,
                     char type[],
                     double amount)
{
    FILE *tPtr;

    time_t now;
    time(&now);

    tPtr = fopen("transactions.txt", "a");

    fprintf(tPtr,
            "Account: %u | %s | Amount: %.2f | %s",
            acc,
            type,
            amount,
            ctime(&now));

    fclose(tPtr);
}

// DISPLAY TRANSACTION HISTORY
void transactionHistory()
{
    FILE *tPtr;

    char ch;

    tPtr = fopen("transactions.txt", "r");

    if (tPtr == NULL)
    {
        printf("No transaction history found.\n");
        return;
    }

    printf("\n========= TRANSACTION HISTORY =========\n");

    while ((ch = fgetc(tPtr)) != EOF)
    {
        putchar(ch);
    }

    fclose(tPtr);
}