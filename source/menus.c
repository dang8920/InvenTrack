#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "user.h"
#include "menus.h"
#include "inventory.h"

void login(){
    char currentID[STR_LEN], currentPassword[STR_LEN];
    printf("Enter EXIT to exit application\n\n");

    // Promts the user to enter their ID and Password
    printf("Enter ID: ");
    getSanitizedInput(currentID, STR_LEN);
    if(strcmp(currentID, "EXIT") == 0){
        exitApp();
        return;
    }

    printf("Enter password: ");
    // Gets the password, but doesn't show the characters for privacy
    getPassword(currentPassword, STR_LEN);
    currentPassword[strcspn(currentPassword, "\n")] = '\0';
    if(strcmp(currentPassword, "EXIT") == 0){
        exitApp();
        return;
    }
    // Checks the user database for matching ID and Password
    // then opens the corresponding menu depending on the 
    // privilege of the user
    getUserData(currentID, currentPassword);
}

void getUserData(char currentID[], char currentPassword[]){
    FILE *userFile = fopenUserFileRead();

    if(userFile == NULL){
        printf("\n");
        login();
        return;
    }

    int found = 0;
    struct User file;

    // Scans user_database.txt line by line and checks that the ID and Password is found
    // and checks to make sure that both belong to one user
    while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", file.id, file.name, file.surname, file.password, &file.role) == 5){
        if(strcmp(file.id, currentID) == 0 && strcmp(file.password, currentPassword) == 0){
            found = 1;
            break;
        }
    }

    closeFileSafe(userFile, "user data update");

    if(found){
        menu(file.role, file.name);
    } else {
        // Lets the user retry to login if ID or Password is incorrect or not found
        printf("\nInvalid ID or Password. Try again\n");
        login();
    }

}

void menu(int userPrivilege, char user[]){
    switch(userPrivilege){
        case ADMIN:
            adminMenu(userPrivilege, user);
            break;
        case MANAGER:
            managerMenu(userPrivilege, user);
            break;
        case EMPLOYEE:
            employeeMenu(userPrivilege, user);
            break;
        default:
            printf("❌ An unexpected error occurred: Permission denied\n");
            exitApp();
            break;
    }
}

void adminMenu(int privilege, char user[]){
    clearConsole();
    printf("Hello %s 💻🔑\n", user);

    const char *options[] = {
        "1) Edit Inventories",
        "2) Edit Users",
        "3) Logout",
        "4) Exit Application"
    };

    int action = handleMenu("", options, 4);

    switch(action){
        case 1: clearConsole(); editInventory(privilege, user); break;
        case 2: clearConsole(); editUser(privilege, user); break;
        case 3: clearConsole(); login(); break;
        case 4: clearConsole(); exitApp(); break;
        default:
            printf("❌ Invalid input. Please enter one of the given numbers.\n");
            menu(privilege, user);
            break;
    }
}

void managerMenu(int privilege, char user[]){
    clearConsole();
    printf("Hello %s 💼\n", user);

    const char *options[] = {
        "1) Edit Inventories",
        "2) Edit Users",
        "3) Logout",
        "4) Exit Application"
    };

    int action = handleMenu("", options, 4);

    switch(action){
        case 1: clearConsole(); editInventory(privilege, user); break;
        case 2: clearConsole(); editUser(privilege, user); break;
        case 3: clearConsole(); login(); break;
        case 4: clearConsole(); exitApp(); break;
        default:
            printf("❌ Invalid input. Please enter one of the given numbers.\n");
            menu(privilege, user);
            break;
    }
}

void employeeMenu(int privilege, char user[]){
    clearConsole();
    printf("Hello %s 🪪\n", user);

    const char *options[] = {
        "1) Edit Inventories",
        "2) Logout",
        "3) Exit Application"
    };

    int action = handleMenu("", options, 3);

    switch(action){
        case 1: clearConsole(); editInventory(privilege, user); break;
        case 2: clearConsole(); login(); break;
        case 3: clearConsole(); exitApp(); break;
        default:
            printf("❌ Invalid input. Please enter one of the given numbers.\n");
            menu(privilege, user);
            break;
    }
}