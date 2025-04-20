#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "user.h"
#include "menus.h"
#include "inventory.h"

char admin[] = "Admin";
char manager[] = "Manager";
char employee[] = "Employee";

void editUser(int userPrivilege, char user[]){
    clearConsole();
    printf("============ Modify User Profiles 🪪  ============\n");

    // Prints a menu with options to modify user profiles
    printf("1) Display Users\n2) Add User\n3) Update User\n4) Delete User\n5) Back\n6) Exit Application\n\n");
    int action = getIntInput("Choose one of the given options: ");

    switch(action){
        case 1:
            clearConsole();
            printf("\n");
            userPage(userPrivilege, user);
            break;
        case 2:
            clearConsole();
            printf("\n");
            addUser(userPrivilege, user);
            break;
        case 3:
            clearConsole();
            printf("\n");
            updateUser(userPrivilege, user);
            break;
        case 4:
            clearConsole();
            printf("\n");
            deleteUser(userPrivilege, user);
            break;
        case 5:
            clearConsole();
            menu(userPrivilege, user);
            break;
        case 6:
            exitApp();
            break;
        default:
            clearConsole();
            printf("Invalid input.\n\n");
            editUser(userPrivilege, user);
    }
}

void showUserInformation(){
    printf("============ Users 🪪  ============\n");

    FILE *userFile = fopenUserFileRead();

    if(userFile == NULL){
        perror("❌ Error opening file");
        return;
    }

    struct User getUser;
    while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", getUser.id, getUser.name, getUser.surname, getUser.password, &getUser.role) == 5){
        printf("Name: %15s %s\tID: %12s\n", getUser.name, getUser.surname, getUser.id);
    }

    closeFileSafe(userFile, "UserPage: user database");
    printf("\n\n");
}

void userPage(int userPrivilege, char user[]){
    printf("============ Display Users 🪪  ============\n");

    FILE *userFile = fopenUserFileRead();

    if(userFile == NULL){
        perror("❌ Error opening file");
        printf("\n");
        editUser(userPrivilege, user);
        return;
    }

    struct User getUser;
    if(userPrivilege == ADMIN){
        while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", getUser.id, getUser.name, getUser.surname, getUser.password, &getUser.role) == 5){
            printf("Name: %15s %s\tID: %12s\tPassword: %15s\n", getUser.name, getUser.surname, getUser.id, getUser.password);
        }
    } else if(userPrivilege == MANAGER){
        while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", getUser.id, getUser.name, getUser.surname, getUser.password, &getUser.role) == 5){
            printf("Name: %15s %s\tID: %12s\n", getUser.name, getUser.surname, getUser.id);
        }
    } else {
        while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", getUser.id, getUser.name, getUser.surname, getUser.password, &getUser.role) == 5){
            printf("Name: %15s %s\n\n", getUser.name, getUser.surname);
        }
    }
    closeFileSafe(userFile, "UsersPage");

    while(1){
        if(promptYesOrNo("Would you like to go back to the previous page?") == 'y'){
            clearConsole();
            editUser(userPrivilege, user);
            break;
        } 
    }
}

void addUser(int userPrivilege, char user[]){
    showUserInformation();
    printf("============ Adding User 🪪  ============\n");
    printf("Enter CANCEL to cancel\n\n");

    FILE *userFile = fopenUserFileAppend();

    if(userFile == NULL){
        perror("❌ Error opening file\n");
        editUser(userPrivilege, user);
        return;
    }

    char buffer[STR_LEN];
    struct User new;

    // Promts the user for all required fields needed for the profile
    printf("Enter first name: ");
    getSanitizedInput(new.name, STR_LEN);
    if(cancelProcess(new.name)){
        closeFileSafe(userFile, "addUser: user database");
        clearConsole();
        editUser(userPrivilege, user);
        return;
    }

    printf("Enter last name: ");
    getSanitizedInput(new.surname, STR_LEN);
    if(cancelProcess(new.surname)){
        closeFileSafe(userFile, "addUser: user database");
        clearConsole();
        editUser(userPrivilege, user);
        return;
    }

    printf("Enter ID: ");
    getSanitizedInput(new.id, STR_LEN);
    if(cancelProcess(new.id)){
        closeFileSafe(userFile, "addUser: user database");
        clearConsole();
        editUser(userPrivilege, user);
        return;
    }

    while(1){
        printf("Enter password: ");
        getPassword(new.password, STR_LEN);
        if(cancelProcess(new.password)){
            closeFileSafe(userFile, "addUser: user database");
            clearConsole();
            editUser(userPrivilege, user);
            break;
            return;
        }

        printf("Confirm password: ");
        getPassword(buffer, STR_LEN);
        if(cancelProcess(buffer)){
            closeFileSafe(userFile, "addUser: user database");
            clearConsole();
            editUser(userPrivilege, user);
            break;
            return;
        }

        if(strcmp(new.password, buffer) == 0){
            break;
        } else {
            printf("❌ Passwords don't match. Try again.\n");
        }
    }

    new.role = getIntInput("Enter role (numeric): ");

    fprintf(userFile, "%s,%s,%s,%s,%d\n", new.id, new.name, new.surname, new.password, new.role);
    closeFileSafe(userFile, "addUser: user database");

    if(new.role == ADMIN){
        printf("✅ User added: %s %s (ID: %s) role: %s\n", new.name, new.surname, new.id, admin);
    } else if(new.role == MANAGER){
        printf("✅ User added: %s %s (ID: %s) role: %s\n", new.name, new.surname, new.id, manager);
    } else if(new.role == EMPLOYEE){
        printf("✅ User added: %s %s (ID: %s) role: %s\n", new.name, new.surname, new.id, employee);
    }

    // Utilized recurrsion to add more users or to go back to the previous menu
    char answer = promptYesOrNo("Would you like to add another user?");
    if(answer == 'y'){
        clearConsole();
        addUser(userPrivilege, user);
    } else {
        clearConsole();
        editUser(userPrivilege, user);
    }
}

void updateUser(int userPrivilege, char user[]){
    showUserInformation();
    printf("============ Update Users 🪪  ============\n");
    printf("Enter CANCEL to cancel\n\n");

    char targetID[STR_LEN];
    printf("Search ID: ");
    getSanitizedInput(targetID, STR_LEN);
    if(strcmp(targetID, "CANCEL") == 0){
        clearConsole();
        editUser(userPrivilege, user);
        return;
    }

    FILE *userFile = fopenUserFileRead();
    FILE *temp = fopenTempFileWrite();

    if(userFile == NULL || temp == NULL){
        perror("❌ Error opening files");
        editUser(userPrivilege, user);
        return;
    }

    int found = 0, role;
    char input1[STR_LEN], input2[STR_LEN];
    struct User update;

    // Scans user_database.txt line by line while assigning the given variables
    while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", update.id, update.name, update.surname, update.password, &update.role) == 5){
        // Finds the profile that the user is attempting to access for modifications
        if(strcmp(targetID, update.id) == 0){
            found = 1;
            // Confirmation to let the user know they are editing the correct profile
            // and asks the user what they would like to update specifically
            printf("\nCurrently editing %s's profile\n", update.name);
            printf("1) Change ID\n2) Change Name\n3) Change Password\n4) Change Role\n5) Cancel\n");

            int action = getIntInput("Enter an option: ");

            switch(action){
                case 1: // Change ID
                    // Updates only the profile's ID number
                    printf("Enter NEW ID: ");
                    getSanitizedInput(input1, STR_LEN);

                    printf("Confirm NEW ID: ");
                    getSanitizedInput(input2, STR_LEN);

                    if(strcmp(input1, input2) == 0){
                        strcpy(update.id, input1);
                        printf("✅ ID successfully updated.\n");
                    } else {
                        printf("❌ IDs do not match. Keeping old ID.\n");
                    }
                    break;
                case 2: // Change Name
                    // Edits the profile's first and last name
                    printf("Enter NEW First Name: ");
                    getSanitizedInput(update.name, STR_LEN);

                    printf("Enter NEW Last Name: ");
                    getSanitizedInput(update.surname, STR_LEN);

                    printf("✅ First and last name successfully updated.\n");
                    break;
                case 3: // Change Password
                    // Updates the profile's current password with a new password.
                    printf("Enter NEW password: ");
                    getPassword(input1, STR_LEN);
                    input1[strcspn(input1, "\n")] = '\0';

                    printf("Confirm NEW password: ");
                    getPassword(input2, STR_LEN);
                    input2[strcspn(input2, "\n")] = '\0';

                    // Compares the new password inputs for added protection as to not input an unwanted password
                    if(strcmp(input1, input2) == 0){
                        strcpy(update.password, input1);
                        printf("✅ Password successfully updated.\n");
                    } else {
                        printf("❌ Passwords do not match. Keeping old password.\n");
                    }
                    break;
                case 4: // Change Role
                    // Updates the profiles role/privilege
                    // ex. an employee is promoted to manager
                    update.role = getIntInput("Enter NEW role (numeric): ");
                    printf("✅ Role successfully updated.\n");
                    break;
                case 5:
                    printf("No changes made.\n");
                    break;
                default:
                    printf("Invalid option. No changes made.\n");
                    break;
            }
        }

        fprintf(temp, "%s,%s,%s,%s,%d\n", update.id, update.name, update.surname, update.password, update.role);
    }

    closeFileSafe(userFile, "updateUser: user database");
    closeFileSafe(temp, "updateUser: temp file");

    remove(FILE_USER_DATABASE);
    rename(FILE_TEMP, FILE_USER_DATABASE);

    if(promptYesOrNo("Would you like to update another profile?") == 'y'){
        clearConsole();
        updateUser(userPrivilege, user);
    } else {
        clearConsole();
        editUser(userPrivilege, user);
    }
}

void deleteUser(int userPrivilege, char user[]){
    showUserInformation();
    printf("============ Delete Users 🪪  ============\n");
    printf("Enter CANCEL to cancel\n\n");
    
    FILE *userFile = fopenUserFileRead();
    FILE *temp = fopenTempFileWrite();

    if(userFile == NULL || temp == NULL){
        perror("❌ Error opening files");
        editUser(userPrivilege, user);
        return;
    }
    
    struct User file;
    char targetID[STR_LEN], tempName[STR_LEN], input[STR_LEN];
    int found = 0;
    printf("Enter the ID number of the user to be deleted: ");
    if(fgets(targetID, STR_LEN, stdin) != NULL){
        targetID[strcspn(targetID, "\n")] = '\0';
        if(strcmp(targetID, "CANCEL") == 0){
            closeFileSafe(userFile, "deleteUser: user database");
            closeFileSafe(temp, "deleteUser: temp file");
            remove(FILE_TEMP);
            
            clearConsole();
            editUser(userPrivilege, user);
            return;
        }

        while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", file.id, file.name, file.surname, file.password, &file.role) == 5){
            if(strcmp(targetID, file.id) == 0){
                found = 1;
                strcpy(tempName, file.name);
                continue;
            } else {
                fprintf(temp, "%s,%s,%s,%s,%d\n", file.id, file.name, file.surname, file.password, file.role);
            }
        }
    }

    closeFileSafe(userFile, "deleteUser: user database");
    closeFileSafe(temp, "deleteUser: temp file");

    remove(FILE_USER_DATABASE);
    rename(FILE_TEMP, FILE_USER_DATABASE);

    if(found){
        printf("✅ %s\'s data has been successfully deleted.\n", tempName);
        if(promptYesOrNo("Would you like to delete another user?") == 'y'){
            clearConsole();
            deleteUser(userPrivilege, user);
        } else {
            clearConsole();
            editUser(userPrivilege, user);
        }
    } else {
        if(promptYesOrNo("❌ User not found. Would you like to try again? ") == 'y'){
            deleteUser(userPrivilege, user);
        } else {
            editUser(userPrivilege, user);
        }
    }
}