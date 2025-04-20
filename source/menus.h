#ifndef MENU_H
#define MENU_H

#define ADMIN 1
#define MANAGER 2
#define EMPLOYEE 3

void login();
void getUserData(char currentID[], char currentPassword[]);

void menu(int userPrivilege, char user[]);
void adminMenu(int userPrivilege, char user[]);
void managerMenu(int userPrivilege, char user[]);
void employeeMenu(int userPrivilege, char user[]);

#endif