#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

#define ADMIN 1
#define MANAGER 2
#define EMPLOYEE 3

#define CHANGE_NAME 1
#define CHANGE_QUANTITY 2
#define CHANGE_PRICE 3

#define FILE_INVENTORY "program_files/inventory.txt"
#define FILE_USER_DATABASE "program_files/user_database.txt"
#define FILE_TEMP "program_files/temp.txt"

#define STR_LEN 50

char admin[] = "Admin";
char manager[] = "Manager";
char employee[] = "Employee";

void login();
void exitApp();
void clearConsole(); // Clears console/terminal to reduce clutter
void delay(int numberOfSeconds); // Delays the app for any number of seconds
void flushInput(); // Flushes buffer input to avoid input bugs

void getUserData(char currentID[], char currentPassword[]);

void usersPage(int userPrivilege, char user[]);
    void displayUserOption(int userPrivilege, char user[]);
        void showUsers(); // Displays a table with the current users
    void addUser(int userPrivilege, char user[]); // Creates a user profile and saves to the database
    void updateUser(int userPrivilege, char user[]); // Updates information of current users
    void deleteUser(int userPrivilege, char user[]); // Deletes any user profile

void editInventory(int userPrivilege, char user[]);
    void displayInventoryInformation();
    void inventoryPage(int userPrivilege, char user[]);
    void addItem(int userPrivilege, char user[]);
    void updateItem(int userPrivilege, char user[]);
    void deleteItem(int userPrivilege, char user[]);

void getPassword(char *password, int maxLength); // Allows users to enter their password without having other people seeing
int getIntInput(const char *prompt);
char yesOrNo(const char *prompt);

void menu(int userPrivilege, char user[]);
void adminMenu(int userPrivilege, char user[]);
void managerMenu(int userPrivilege, char user[]);
void employeeMenu(int userPrivilege, char user[]);

struct User{
    char name[STR_LEN];
    char surname[STR_LEN];
    char id[STR_LEN];
    char password[STR_LEN];
    int role;
};

struct Inventory{
    int index;
    char name[STR_LEN];
    int quantity;
    float cost;
};

int main(){
    SetConsoleOutputCP(CP_UTF8);
    clearConsole();
    login();
    
    return 0;
}

void login(){
    char currentID[STR_LEN], currentPassword[STR_LEN];

    // Promts the user to enter their ID and Password
    printf("Enter ID: ");
    fgets(currentID, STR_LEN, stdin);
    currentID[strcspn(currentID, "\n")] = '\0';

    printf("Enter password: ");
    // Gets the password, but doesn't show the characters for privacy
    getPassword(currentPassword, STR_LEN);
    currentPassword[strcspn(currentPassword, "\n")] = '\0';

    // Checks the user database for matching ID and Password
    // then opens the corresponding menu depending on the 
    // privilege of the user
    getUserData(currentID, currentPassword);
}

void exitApp(){
    clearConsole();

    // Exits the application after a 3 second countdown
    printf("Exiting InvenTrack in...");
    printf("3...");delay(1);
    printf("2...");delay(1);
    printf("1...");delay(1);
}

void clearConsole(){
    printf("\e[1;1H\e[2J"); // Clears the console on any OS (Windows or Linux)
}

void delay(int numberOfSeconds)
{
	// Converting time into milli_seconds
	int milliSeconds = 1000 * numberOfSeconds;

	// Storing start time
	clock_t startTime = clock();

	// Looping till required time is not achieved
	while (clock() < startTime + milliSeconds);
}

void flushInput(){
    int ch;

    // Clears the input buffer to avoid input bugs
    while((ch = getchar()) != '\n' && ch != EOF);
}

void getUserData(char currentID[], char currentPassword[]){
    FILE *userFile;
    userFile = fopen(FILE_USER_DATABASE, "r");

    if(userFile == NULL){
        perror("❌ Error retrieving user data");

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

    fclose(userFile);

    if(found){
        menu(file.role, file.name);
    } else {
        // Lets the user retry to login if ID or Password is incorrect or not found
        printf("\nInvalid ID or Password. Try again\n");
        login();
    }

}

void usersPage(int userPrivilege, char user[]){
    clearConsole();
    printf("============ Modify User Profiles 🪪  ============\n");

    // Prints a menu with options to modify user profiles
    printf("1) Display Users\n2) Add User\n3) Update User\n4) Delete User\n5) Back\n6) Exit Application\n\n");
    int action = getIntInput("Choose one of the given options: ");

    switch(action){
        case 1:
            clearConsole();
            printf("\n");
            displayUserOption(userPrivilege, user);
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
            usersPage(userPrivilege, user);
    }
}

void displayUserOption(int userPrivilege, char user[]){
    printf("============ Display Users 🪪  ============\n");

    FILE *userFile;
    userFile = fopen(FILE_USER_DATABASE, "r+");

    if(userFile == NULL){
        perror("❌ Error opening file");
        printf("\n");
        usersPage(userPrivilege, user);
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
    fclose(userFile);

    while(1){
        if(yesOrNo("Would you like to go back to the previous page?") == 'y'){
            clearConsole();
            usersPage(userPrivilege, user);
            break;
        } 
    }
}

void showUsers(){
    printf("============ Users 🪪  ============\n");

    FILE *userFile;
    userFile = fopen(FILE_USER_DATABASE, "r+");

    if(userFile == NULL){
        perror("❌ Error opening file");
        return;
    }

    struct User getUser;
    while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", getUser.id, getUser.name, getUser.surname, getUser.password, &getUser.role) == 5){
        printf("Name: %15s %s\tID: %12s\n", getUser.name, getUser.surname, getUser.id);
    }

    fclose(userFile);
    printf("\n\n");
}

void addUser(int userPrivilege, char user[]){
    showUsers();
    printf("============ Adding User 🪪  ============\n");
    printf("Enter -1 to cancel\n\n");

    FILE *userFile;
    userFile = fopen(FILE_USER_DATABASE, "a+"); // No need to write or read to the file. We simply add the new user profile based on standard input

    if(userFile == NULL){
        perror("❌ Error opening file\n");
        usersPage(userPrivilege, user);
        return;
    }

    char buffer[STR_LEN];
    struct User new;

    // Promts the user for all required fields needed for the profile
    printf("Enter first name: ");
    fgets(new.name, STR_LEN, stdin);
    new.name[strcspn(new.name, "\n")] = '\0';

    printf("Enter last name: ");
    fgets(new.surname, STR_LEN, stdin);
    new.surname[strcspn(new.surname, "\n")] = '\0';

    printf("Enter ID: ");
    fgets(new.id, STR_LEN, stdin);
    new.id[strcspn(new.id, "\n")] = '\0';

    while(1){
        printf("Enter password: ");
        getPassword(new.password, STR_LEN);

        printf("Confirm password: ");
        getPassword(buffer, STR_LEN);

        if(strcmp(new.password, buffer) == 0){
            break;
        } else {
            printf("❌ Passwords don't match. Try again.\n");
        }
    }

    new.role = getIntInput("Enter role (numeric): ");

    fprintf(userFile, "%s,%s,%s,%s,%d\n", new.id, new.name, new.surname, new.password, new.role);
    fclose(userFile);

    if(new.role == ADMIN){
        printf("✅ User added: %s %s (ID: %s) role: %s\n", new.name, new.surname, new.id, admin);
    } else if(new.role == MANAGER){
        printf("✅ User added: %s %s (ID: %s) role: %s\n", new.name, new.surname, new.id, manager);
    } else if(new.role == EMPLOYEE){
        printf("✅ User added: %s %s (ID: %s) role: %s\n", new.name, new.surname, new.id, employee);
    }

    // Utilized recurrsion to add more users or to go back to the previous menu
    char answer = yesOrNo("Would you like to add another user?");
    if(answer == 'y'){
        clearConsole();
        addUser(userPrivilege, user);
    } else if(answer == 'n'){
        clearConsole();
        usersPage(userPrivilege, user);
    } else {
        printf("Invalid input. Returning to main menu.\n\n");
        menu(userPrivilege, user);
    }
}

void updateUser(int userPrivilege, char user[]){
    showUsers();
    printf("============ Update Users 🪪  ============\n");

    char targetID[STR_LEN];
    printf("Search ID: ");
    fgets(targetID, STR_LEN, stdin);
    targetID[strcspn(targetID, "\n")] = '\0';

    FILE *userFile = fopen(FILE_USER_DATABASE, "r+"); // Only reading this file is required
    FILE *temp = fopen(FILE_TEMP, "w+"); // Only writing to this file is required

    if(userFile == NULL || temp == NULL){
        perror("❌ Error opening files");
        usersPage(userPrivilege, user);
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
                    fgets(input1, STR_LEN, stdin);
                    input1[strcspn(input1, "\n")] = '\0';

                    printf("Confirm NEW ID: ");
                    fgets(input2, STR_LEN, stdin);
                    input2[strcspn(input2, "\n")] = '\0';

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
                    fgets(update.name, STR_LEN, stdin);
                    update.name[strcspn(update.name, "\n")] = '\0';

                    printf("Enter NEW Last Name: ");
                    fgets(update.surname, STR_LEN, stdin);
                    update.surname[strcspn(update.surname, "\n")] = '\0';

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

    fclose(userFile);
    fclose(temp);

    remove(FILE_USER_DATABASE);
    rename(FILE_TEMP, FILE_USER_DATABASE);

    if(yesOrNo("Would you like to update another profile?") == 'y'){
        clearConsole();
        updateUser(userPrivilege, user);
    } else {
        clearConsole();
        usersPage(userPrivilege, user);
    }
}

void deleteUser(int userPrivilege, char user[]){
    showUsers();
    printf("============ Delete Users 🪪  ============\n");
    
    FILE *userFile;
    FILE *temp;
    userFile = fopen(FILE_USER_DATABASE, "r+");
    temp = fopen(FILE_TEMP, "w+");

    if(userFile == NULL || temp == NULL){
        perror("❌ Error opening files");
        usersPage(userPrivilege, user);
        return;
    }
    
    struct User file;
    char targetID[STR_LEN], tempName[STR_LEN], input[STR_LEN];
    int found = 0;
    printf("Enter the ID number of the user to be deleted: ");
    if(fgets(targetID, STR_LEN, stdin) != NULL){
        targetID[strcspn(targetID, "\n")] = '\0';

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

    fclose(userFile);
    fclose(temp);

    remove(FILE_USER_DATABASE);
    rename(FILE_TEMP, FILE_USER_DATABASE);

    if(found){
        printf("✅ %s\'s data has been successfully deleted.\n", tempName);
        if(yesOrNo("Would you like to delete another user?") == 'y'){
            clearConsole();
            deleteUser(userPrivilege, user);
        } else {
            clearConsole();
            usersPage(userPrivilege, user);
        }
    } else {
        if(yesOrNo("❌ User not found. Would you like to try again? ") == 'y'){
            deleteUser(userPrivilege, user);
        } else {
            usersPage(userPrivilege, user);
        }
    }
}

void editInventory(int userPrivilege, char user[]){
    printf("============ Edit Inventory 📦 ============\n");
    int action = 0;
    if(userPrivilege == ADMIN){
        printf("1) Display Inventory\n2) Add New Items\n3) Update Exsisting Items\n4) Delete Exsisting Items\n5) Back\n6) Exit Application\n\n");
        action = getIntInput("Enter one of the given options: ");

        switch(action){
            case 1:
                clearConsole();
                printf("\n");
                inventoryPage(userPrivilege, user);
                break;
            case 2:
                clearConsole();
                printf("\n");
                addItem(userPrivilege, user);
                break;
            case 3:
                clearConsole();
                printf("\n");
                updateItem(userPrivilege, user);
                break;
            case 4:
                clearConsole();
                printf("\n");
                deleteItem(userPrivilege, user);
                break;
            case 5:
                clearConsole();
                printf("\n");
                menu(userPrivilege, user);
                break;
            case 6:
                printf("\n");
                exitApp();
                break;
            default:
                clearConsole();
                printf("❌ Invalid input. Enter one of the given numbers.\n");
                editInventory(userPrivilege, user);
                break;
        }
    } else if(userPrivilege == MANAGER){
        printf("1) Display Inventory\n2) Add New Items\n3) Update Exsisting Items\n4) Delete Exsisting Items\n5) Back\n6) Exit Application\n\n");
        action = getIntInput("Enter one of the given options: ");

        switch(action){
            case 1:
                clearConsole();
                printf("\n");
                inventoryPage(userPrivilege, user);
                break;
            case 2:
                clearConsole();
                printf("\n");
                addItem(userPrivilege, user);
                break;
            case 3:
                clearConsole();
                printf("\n");
                updateItem(userPrivilege, user);
                break;
            case 4:
                clearConsole();
                printf("\n");
                deleteItem(userPrivilege, user);
                break;
            case 5:
                clearConsole();
                printf("\n");
                menu(userPrivilege, user);
                break;
            case 6:
                printf("\n");
                exitApp();
                break;
            default:
                clearConsole();
                printf("❌ Invalid input. Enter one of the given numbers.\n");
                editInventory(userPrivilege, user);
                break;
        }
    } else {
        printf("1) Display Inventory\n2) Update Exsisting Items\n3) Back\n4) Exit Application\n\n");
        action = getIntInput("Enter one of the given options: ");

        switch(action){
            case 1:
                clearConsole();
                printf("\n");
                inventoryPage(userPrivilege, user);
                break;
            case 2:
                clearConsole();
                printf("\n");
                updateItem(userPrivilege, user);
                break;
            case 3:
                clearConsole();
                printf("\n");
                menu(userPrivilege, user);
                break;
            case 4:
                printf("\n");
                exitApp();
                break;
            default:
                clearConsole();
                printf("❌ Invalid input. Enter one of the given numbers.\n");
                editInventory(userPrivilege, user);
                break;
        } 
    }
}

void displayInventoryInformation(){
    FILE *inv;
    inv = fopen(FILE_INVENTORY, "r+");

    if(inv == NULL){
        perror("❌ Error displaying contents of the file");
        return;
    }

    struct Inventory item;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        printf("%5d: %30s\tcount: %d \tcost: $%.2f\n", item.index, item.name, item.quantity, item.cost);
    }

    fclose(inv);
}

void inventoryPage(int userPrivilege, char user[]){
    printf("============ Inventory 📦 ============\n");
    FILE *inv;
    inv = fopen(FILE_INVENTORY, "r+");

    if(inv == NULL){
        perror("❌ Error displaying inventoy items");
        editInventory(userPrivilege, user);
        return;
    }

    struct Inventory item;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        printf("%5d: %30s\tcount: %d \tcost: $%.2f\n", item.index, item.name, item.quantity, item.cost);
    }

    fclose(inv);

    char key;
    while(1){
        if(yesOrNo("Would you like to go back to the previous page?") == 'y'){
            clearConsole();
            editInventory(userPrivilege, user);
            break;
        }
    }
    
}

void addItem(int userPrivilege, char user[]){
    printf("============ Add Items ============\n");
    printf("Enter CANCEL to abort process\n");

    FILE *inv;
    FILE *temp;
    inv = fopen(FILE_INVENTORY, "r+");
    temp = fopen(FILE_TEMP, "w+");

    if(inv == NULL || temp == NULL){
        perror("❌ Error connecting to inventory");
        editInventory(userPrivilege, user);
        return;
    }

    struct Inventory item;
    char input[STR_LEN];
    item.index = 0;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        fprintf(temp, "%d,%s,%d,%.2f\n", item.index, item.name, item.quantity, item.cost);
    }

    // Read item name
    printf("Enter name of item: ");
    if(fgets(item.name, STR_LEN, stdin) != NULL){
        item.name[strcspn(item.name, "\n")] = '\0';
        // Cancels adding a new user
        if(strcmp(item.name, "CANCEL") == 0){
            fclose(inv);
            fclose(temp);

            remove(FILE_INVENTORY);
            rename(FILE_TEMP, FILE_INVENTORY);
            clearConsole();
            editInventory(userPrivilege, user);
            return;
        }
    }

    printf("Enter quantity: ");
    if(fgets(input, STR_LEN, stdin) != NULL){
        input[strcspn(input, "\n")] = '\0';
        // Cancels adding a new user
        if(strcmp(input, "CANCEL") == 0){
            fclose(inv);
            fclose(temp);
            
            remove(FILE_INVENTORY);
            rename(FILE_TEMP, FILE_INVENTORY);
            clearConsole();
            editInventory(userPrivilege, user);
            return;
        } else {
            sscanf(input, "%d", &item.quantity);
        }
    }

    printf("Enter the price of the item: $");
    if(fgets(input, STR_LEN, stdin) != NULL){
        input[strcspn(input, "\n")] = '\0';
        // Cancels adding a new user
        if(strcmp(input, "CANCEL") == 0){
            fclose(inv);
            fclose(temp);
            
            remove(FILE_INVENTORY);
            rename(FILE_TEMP, FILE_INVENTORY);
            clearConsole();
            editInventory(userPrivilege, user);
            return;
        } else {
            sscanf(input, "%f", &item.cost);
        }
    }

    fprintf(temp, "%d,%s,%d,%.2f\n", ++(item.index), item.name, item.quantity, item.cost);
    printf("✅ %s successfully add to inventory\n", item.name);

    fclose(inv);
    fclose(temp);

    remove(FILE_INVENTORY);
    rename(FILE_TEMP, FILE_INVENTORY);

    char proceed;
    if(yesOrNo("Would you like to add another item?") == 'y'){
        clearConsole();
        addItem(userPrivilege, user);
    } else {
        clearConsole();
        editInventory(userPrivilege, user);
    }
}

void updateItem(int userPrivilege, char user[]){
    printf("============ Update Items ============\n");
    displayInventoryInformation();

    char targetItem[STR_LEN];
    printf("Enter the name of the item to update: ");
    fgets(targetItem, sizeof(targetItem), stdin);
    targetItem[strcspn(targetItem, "\n")] = '\0';

    // Checks current user privileges and enables/disables 
    // item modification accordingly
    int command;
    if(userPrivilege == ADMIN){
        printf("\n%s selected,\n1) Change Name\n2) Change Quantity\n3) Change Price\n4) Cancel\n\n", targetItem);
        command = getIntInput("Enter one of the given options: ");
        if(command == 4){
            editInventory(userPrivilege, user); // Previous page
            return; // Prevents shutdown bugs
        }
    } else if(userPrivilege == MANAGER){
        printf("\n%s selected,\n1) Change Name\n2) Change Quantity\n3) Cancel\n\n", targetItem);
        command = getIntInput("Enter one of the given options: ");
        if(command == 3){
            editInventory(userPrivilege, user); // Previous page
            return; // Prevents shutdown bugs
        }
    } else {
        command = CHANGE_QUANTITY;
    }

    FILE *inv = fopen(FILE_INVENTORY, "r");
    FILE *temp = fopen(FILE_TEMP, "w");

    if(inv == NULL || temp == NULL){
        perror("❌ Error opening files");
        editInventory(userPrivilege, user);
        return;
    }

    struct Inventory item;
    char input[STR_LEN];
    char symbol;
    int found = 0;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        if(strcmp(item.name, targetItem) == 0){
            found = 1;
            switch(command){
                case CHANGE_NAME:
                    printf("Enter new name: ");
                    if(fgets(item.name, STR_LEN, stdin) != NULL){
                        item.name[strcspn(item.name, "\n")] = '\0';
                    }
                    break;
                case CHANGE_QUANTITY:
                    printf("Enter updated quantity: ");

                    if(fgets(input, STR_LEN, stdin) != NULL){
                        if(sscanf(input, "%d", &item.quantity) == 1){} 
                        else {
                                printf("❌ Invalid input. Disregarding changes.\n");
                            }
                        }
                    break;
                case CHANGE_PRICE:
                    printf("Enter updated price: $");
                    if(fgets(input, STR_LEN, stdin) != NULL){
                        sscanf(input, "%f", &item.cost);
                    }
                    break;
                default:
                    printf("❌ Invalid input. No changes were made.\n");
                    break;
            }
            fprintf(temp, "%d,%s,%d,%.2f\n", item.index, item.name, item.quantity, item.cost);
        } else {
            fprintf(temp, "%d,%s,%d,%.2f\n", item.index, item.name, item.quantity, item.cost);
        }
    }

    fclose(inv);
    fclose(temp);

    remove(FILE_INVENTORY);
    rename(FILE_TEMP, FILE_INVENTORY);

    if(!found){
        printf("❌ Item not found.\n\n");
        updateItem(userPrivilege, user);
    } else {
        printf("\n✅ Item updated successfully.\n");
        if(yesOrNo("Would you like to update another item?") == 'y'){
            clearConsole();
            updateItem(userPrivilege, user);
        } else {
            clearConsole();
            editInventory(userPrivilege, user);
        }
    }
}

void deleteItem(int userPrivilege, char user[]){
    printf("============ Delete Items ============\n");
    displayInventoryInformation();
    printf("Enter CANCEL to abort process\n");

    char targetItem[STR_LEN];
    printf("Enter the name of the item to delete: ");
    fgets(targetItem, sizeof(targetItem), stdin);
    targetItem[strcspn(targetItem, "\n")] = '\0';

    FILE *inv;
    FILE *temp;

    inv = fopen(FILE_INVENTORY, "r");
    temp = fopen(FILE_TEMP, "w");

    if(inv == NULL || temp == NULL){
        perror("❌ Error opening files.");
        editInventory(userPrivilege, user);
        return;
    }

    int found = 0, count = 0;

    char input[10], confirmation;
    struct Inventory item;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        if(strcmp(targetItem, "CANCEL") == 0){
            fclose(inv);
            fclose(temp);

            remove(FILE_INVENTORY);
            rename(FILE_TEMP, FILE_INVENTORY);
            clearConsole();
            editInventory(userPrivilege, user);
            return;
        }
        if(strcmp(targetItem, item.name) == 0){
            found = 1;
            if(yesOrNo("Are you sure you want to proceed with deletion?") == 'y'){
                printf("✅ %s successfully deleted.\n");
                continue;
            } else {
                count++;
                fprintf(temp, "%d,%s,%d,%.2f\n", count, item.name, item.quantity, item.cost);
            }
        } else {
            count++;
            fprintf(temp, "%d,%s,%d,%.2f\n", count, item.name, item.quantity, item.cost);
        }
    }

    fclose(inv);
    fclose(temp);

    remove(FILE_INVENTORY);
    rename(FILE_TEMP, FILE_INVENTORY);

    if(!found){
        while(1){
            if(yesOrNo("❌ Item not found.\nWould you like to try again?") == 'y'){
                clearConsole();
                deleteItem(userPrivilege, user);
                break;
            } else {
                clearConsole();
                editInventory(userPrivilege, user);
                break;
            }
        }
    } else {
        if(yesOrNo("Would you like to delete another item?") == 'y'){
            clearConsole();
            deleteItem(userPrivilege, user);
        } else {
            clearConsole();
            editInventory(userPrivilege, user);
        }
    }
}

void getPassword(char *password, int maxLength){
    int i = 0;
    char ch;

#ifdef _WIN32
    while(1){
        ch = getch();

        if(ch == 13){ // Enter
            break;
        } else if(ch == 8 && i > 0){ // Backspace
            i--;
            printf("\b \b");
        } else if(i < maxLength - 1){
            password[i++] = ch;
            printf("*");
        }
    }
#else
    struct termios oldt, newt;

    // Turn off terminal echo
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while((ch = getchar()) != '\n' && i < maxLength - 1){
        if(ch == 127 || ch == 8){ // Backspace key
            if(i > 0){
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    password[i] = '\0';
    printf("\n");
}

int getIntInput(const char *prompt){
    char input[STR_LEN];
    int value;

    while(1){
        printf("%s", prompt);
        if(fgets(input, STR_LEN, stdin) != NULL){
            if(sscanf(input, "%d", &value) == 1){
                return value;
            }
        }
        printf("❌ Invalid input. Please enter a valid number.\n");
    }
}

char yesOrNo(const char *prompt){
    char inputStr[STR_LEN], answer;
    while(1){
        printf("\n➡️  %s (y/n): ", prompt);
        if(fgets(inputStr, STR_LEN, stdin) != NULL){
            if(sscanf(inputStr, " %c", &answer) == 1){
                answer = tolower(answer);
                if(answer == 'y' || answer == 'n'){
                    return answer;
                }
            }
        }
        printf("❌ Invalid input. Please enter 'y' or 'n'.\n");
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
            printf("❌ An unexpected error occurd: Permission denied");
            exitApp();
            break;
            
    }
}

void adminMenu(int userPrivilege, char user[]){
    clearConsole();
    printf("Hello %s 💻🔑\n", user);
    int action = 0;

    printf("1) Edit Inventories \n2) Edit Users\n3) Logout\n4) Exit Application\n\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1: // Edit Inventories
            clearConsole();
            editInventory(userPrivilege, user);
            break;
        case 2: // Edit Users
            clearConsole();
            usersPage(userPrivilege, user);
            break;
        case 3: // Logout
            clearConsole();
            login();
            break;
        case 4: // Exit Application
            clearConsole();
            exitApp();
            break;
        default:
            printf("\n");
            printf("❌ Invalid input. Please enter one of the given numbers.\n");
            menu(userPrivilege, user);
            break;
    }
}

void managerMenu(int userPrivilege, char user[]){
    clearConsole();
    printf("Hello %s 💼\n", user);
    int action = 0;

    printf("1) Edit Inventories \n2) Edit Users\n3) Logout\n4) Exit Application\n\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1: // Edit Inventories
            clearConsole();
            editInventory(userPrivilege, user);
            break;
        case 2: // Edit Users
            clearConsole();
            usersPage(userPrivilege, user);
            break;
        case 3: // Logout
            clearConsole();
            login();
            break;
        case 4: // Exit Application
            clearConsole();
            exitApp();
            break;
        default:
            printf("\n");
            printf("❌ Invalid input. Please enter one of the given numbers.\n");
            menu(userPrivilege, user);
            break;
    }
}

void employeeMenu(int userPrivilege, char user[]){
    clearConsole();
    printf("Hello %s 🪪\n", user);
    int action = 0;

    printf("1) Edit Inventories\n2) Logout\n3) Exit Application\n\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1: // Edit Inventories
            clearConsole();
            editInventory(userPrivilege, user);
            break;
        case 2: // Logout
            clearConsole();
            login();
            break;
        case 3: // Exit Application
            clearConsole();
            exitApp();
            break;
        default:
            printf("\n");
            printf("❌ Invalid input. Please enter one of the given numbers.\n");
            menu(userPrivilege, user);
            break;
    }
}
