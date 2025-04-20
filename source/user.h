// user.h
#ifndef USER_H
#define USER_H

#define STR_LEN 50

// Replacements for common file usages
#define fopenUserFileRead() openFileChecked(FILE_USER_DATABASE, "r", "reading user data")
#define fopenUserFileAppend() openFileChecked(FILE_USER_DATABASE, "a+", "appending user data")
#define fopenInvFileRead() openFileChecked(FILE_INVENTORY, "r", "reading inventory")
#define fopenInvFileWrite() openFileChecked(FILE_INVENTORY, "w", "overwriting inventory")
#define fopenTempFileWrite() openFileChecked(FILE_TEMP, "w", "writing temporary data")

#define FILE_INVENTORY "program_files/inventory.txt"
#define FILE_USER_DATABASE "program_files/user_database.txt"
#define FILE_TEMP "program_files/temp.txt"

#define ADMIN 1
#define MANAGER 2
#define EMPLOYEE 3

struct User {
    char name[STR_LEN];
    char surname[STR_LEN];
    char id[STR_LEN];
    char password[STR_LEN];
    int role;
};

void editUser(int userPrivilege, char user[]);
void showUserInformation();
void userPage(int userPrivilege, char user[]);
void addUser(int userPrivilege, char user[]);
void updateUser(int userPrivilege, char user[]);
void deleteUser(int userPrivilege, char user[]);

#endif
