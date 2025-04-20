#ifndef INVENTORY_H
#define INVENTORY_H

#define STR_LEN 50

#define FILE_INVENTORY "program_files/inventory.txt"
#define FILE_USER_DATABASE "program_files/user_database.txt"
#define FILE_TEMP "program_files/temp.txt"

#define ADMIN 1
#define MANAGER 2
#define EMPLOYEE 3

#define CHANGE_NAME 1
#define CHANGE_QUANTITY 2
#define CHANGE_PRICE 3

struct Inventory{
    int index;
    char name[STR_LEN];
    int quantity;
    float cost;
};

void editInventory(int userPrivilege, char user[]);
void showInventoryInformation();
void inventoryPage(int userPrivilege, char user[]);
void addItem(int userPrivilege, char user[]);
void updateItem(int userPrivilege, char user[]);
void deleteItem(int userPrivilege, char user[]);

#endif