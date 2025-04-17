#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#ifdef _WIN32
#include <conio.h>
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

#define STR_LEN 50

char admin[] = "Admin";
char manager[] = "Manager";
char employee[] = "Employee";

void login();
void exitApp();
void clearConsole();
void delay(int number_of_seconds);
void flushInput();

void getUserData(char currentID[], char currentPassword[]);
void editUsers(int privilege, char user[]);
    void displayUser(int privilege, char user[]);
    void addUser(int privilege, char user[]);
    void updateUser(int privilege, char user[]);
    void deleteUser(int privilege, char user[]);
void editInventories(int privilege, char user[]);
    void displayBasicInventory();
    void displayAdvanceInventory();
    void displayAdvancedInventoryPage(int privilege, char user[]);
    void addItem(int privilege, char user[]);
    void updateItem(int privilege, char user[]);
    void deleteItem(int privilege, char user[]);

void getPassword(char *password, int maxLength); // Allows users to enter their password without having other people seeing
int getIntInput(const char *prompt);
char yesOrNo(const char *prompt);

void menu(int privilege, char user[]);
void adminMenu(int privilege, char user[]);
void managerMenu(int privilege, char user[]);
void employeeMenu(int privilege, char user[]);

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

void delay(int number_of_seconds)
{
	// Converting time into milli_seconds
	int milli_seconds = 1000 * number_of_seconds;

	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + milli_seconds);
}

void flushInput(){
    int ch;

    // Clears the input buffer to avoid input bugs
    while((ch = getchar()) != '\n' && ch != EOF);
}

void getUserData(char currentID[], char currentPassword[]){
    FILE *userFile;
    userFile = fopen("user_database.txt", "r");

    if(userFile == NULL){
        printf("Error retrieving user data\n");

        clearConsole();
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
        printf("Invalid ID or Password\nTry again\n\n");
        login();
    }

}

void editUsers(int privilege, char user[]){
    clearConsole();

    // Prints a menu with options to modify user profiles
    printf("1) Display Users\n2) Add User\n3) Update User\n4) Delete User\n5) Back\n6) Exit Application\n\n");
    int action = getIntInput("Choose one of the given options: ");

    switch(action){
        case 1:
            clearConsole();
            printf("\n");
            displayUser(privilege, user);
            break;
        case 2:
            clearConsole();
            printf("\n");
            addUser(privilege, user);
            break;
        case 3:
            clearConsole();
            printf("\n");
            updateUser(privilege, user);
            break;
        case 4:
            clearConsole();
            printf("\n");
            deleteUser(privilege, user);
            break;
        case 5:
            clearConsole();
            menu(privilege, user);
            break;
        case 6:
            exitApp();
            break;
        default:
            clearConsole();
            printf("Invalid input.\n\n");
            editUsers(privilege, user);
    }
}

void displayUser(int privilege, char user[]){

    FILE *userFile;
    userFile = fopen("user_database.txt", "r+");

    struct User getUser;
    if(privilege == ADMIN){
        while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", getUser.id, getUser.name, getUser.surname, getUser.password, &getUser.role) == 5){
            printf("Name: %s %s\n", getUser.name, getUser.surname);
            printf("ID: %s\n", getUser.id);
            printf("Password: %s\n\n", getUser.password);
        }
    } else if(privilege == MANAGER){
        while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", getUser.id, getUser.name, getUser.surname, getUser.password, &getUser.role) == 5){
            printf("Name: %s %s\n", getUser.name, getUser.surname);
            printf("ID: %s\n\n", getUser.id);
        }
    } else {
        while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", getUser.id, getUser.name, getUser.surname, getUser.password, &getUser.role) == 5){
            printf("Name: %s %s\n\n", getUser.name, getUser.surname);
        }
    }

    while(1){
        if(yesOrNo("Would you like to go back to the previous page?") == 'y'){
            clearConsole();
            editUsers(privilege, user);
            break;
        } 
    }
}

void addUser(int privilege, char user[]){
    FILE *userFile;
    userFile = fopen("user_database.txt", "a+"); // No need to write or read to the file. We simply add the new user profile based on standard input

    if(userFile == NULL){
        printf("Error opening file!\n");
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

    printf("Enter password: ");
    fgets(new.password, STR_LEN, stdin);
    new.password[strcspn(new.password, "\n")] = '\0';

    new.role = getIntInput("Enter role (numeric): ");

    fprintf(userFile, "%s,%s,%s,%s,%d\n", new.id, new.name, new.surname, new.password, new.role);
    fclose(userFile);

    if(new.role == ADMIN){
        printf("User added: %s %s (ID: %s) role: %s\n", new.name, new.surname, new.id, admin);
    } else if(new.role == MANAGER){
        printf("User added: %s %s (ID: %s) role: %s\n", new.name, new.surname, new.id, manager);
    } else if(new.role == EMPLOYEE){
        printf("User added: %s %s (ID: %s) role: %s\n", new.name, new.surname, new.id, employee);
    }

    // Utilized recurrsion to add more users or to go back to the previous menu
    char answer = yesOrNo("Would you like to add another user?");
    if(answer == 'y'){
        clearConsole();
        addUser(privilege, user);
    } else if(answer == 'n'){
        clearConsole();
        editUsers(privilege, user);
    } else {
        printf("Invalid input. Returning to main menu.\n\n");
        menu(privilege, user);
    }
}

void updateUser(int privilege, char user[]){
    char targetID[STR_LEN];
    printf("Search ID: ");
    fgets(targetID, STR_LEN, stdin);
    targetID[strcspn(targetID, "\n")] = '\0';

    FILE *userFile = fopen("user_database.txt", "r+"); // Only reading this file is required
    FILE *temp = fopen("temp.txt", "w+"); // Only writing to this file is required

    if(userFile == NULL || temp == NULL){
        perror("Error opening files");
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
                        printf("ID successfully updated.\n");
                    } else {
                        printf("IDs do not match. Keeping old ID.\n");
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

                    printf("First and last name successfully updated.\n");
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
                        printf("Password successfully updated.\n");
                    } else {
                        printf("Passwords do not match. Keeping old password.\n");
                    }
                    break;
                case 4: // Change Role
                    // Updates the profiles role/privilege
                    // ex. an employee is promoted to manager
                    update.role = getIntInput("Enter NEW role (numeric): ");
                    printf("Role successfully updated.\n");
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

    remove("user_database.txt");
    rename("temp.txt", "user_database.txt");

    if(yesOrNo("Would you like to update another profile?") == 'y'){
        clearConsole();
        updateUser(privilege, user);
    } else {
        clearConsole();
        editUsers(privilege, user);
    }
}

void deleteUser(int privilege, char user[]){
    
    FILE *userFile;
    FILE *temp;
    userFile = fopen("user_database.txt", "r+");
    temp = fopen("temp.txt", "w+");
    
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

    remove("user_database.txt");
    rename("temp.txt", "user_database.txt");

    if(found){
        printf("%s\'s data has been successfully deleted.\n", tempName);
        if(yesOrNo("Woul you like to delete another user?") == 'y'){
            clearConsole();
            deleteUser(privilege, user);
        } else {
            clearConsole();
            editUsers(privilege, user);
        }
    } else {
        if(yesOrNo("User not found. Would you like to try again? ") == 'y'){
            deleteUser(privilege, user);
        } else {
            editUsers(privilege, user);
        }
    }
}

void editInventories(int privilege, char user[]){
    int action = 0;
    if(privilege == ADMIN){
        printf("1) Display Inventory\n2) Add New Items\n3) Update Exsisting Items\n4) Delete Exsisting Items\n5) Back\n6) Exit Application\n\n");
        action = getIntInput("Enter one of the given options: ");

        switch(action){
            case 1:
                clearConsole();
                printf("\n");
                displayAdvancedInventoryPage(privilege, user);
                break;
            case 2:
                clearConsole();
                printf("\n");
                addItem(privilege, user);
                break;
            case 3:
                clearConsole();
                printf("\n");
                updateItem(privilege, user);
                break;
            case 4:
                clearConsole();
                printf("\n");
                deleteItem(privilege, user);
                break;
            case 5:
                clearConsole();
                printf("\n");
                menu(privilege, user);
                break;
            case 6:
                printf("\n");
                exitApp();
                break;
            default:
                clearConsole();
                printf("Invalid input. Enter one of the given numbers.\n");
                editInventories(privilege, user);
                break;
        }
    } else if(privilege == MANAGER){
        printf("1) Display Inventory\n2) Add New Items\n3) Update Exsisting Items\n4) Delete Exsisting Items\n5) Back\n6) Exit Application\n\n");
        action = getIntInput("Enter one of the given options: ");

        switch(action){
            case 1:
                clearConsole();
                printf("\n");
                displayAdvancedInventoryPage(privilege, user);
                break;
            case 2:
                clearConsole();
                printf("\n");
                addItem(privilege, user);
                break;
            case 3:
                clearConsole();
                printf("\n");
                updateItem(privilege, user);
                break;
            case 4:
                clearConsole();
                printf("\n");
                deleteItem(privilege, user);
                break;
            case 5:
                clearConsole();
                printf("\n");
                menu(privilege, user);
                break;
            case 6:
                printf("\n");
                exitApp();
                break;
            default:
                clearConsole();
                printf("Invalid input. Enter one of the given numbers.\n");
                editInventories(privilege, user);
                break;
        }
    } else {
        printf("1) Display Inventory\n2) Update Exsisting Items\n3) Back\n4) Exit Application\n\n");
        action = getIntInput("Enter one of the given options: ");

        switch(action){
            case 1:
                clearConsole();
                printf("\n");
                displayAdvancedInventoryPage(privilege, user);
                break;
            case 2:
                clearConsole();
                printf("\n");
                updateItem(privilege, user);
                break;
            case 3:
                clearConsole();
                printf("\n");
                menu(privilege, user);
                break;
            case 4:
                printf("\n");
                exitApp();
                break;
            default:
                clearConsole();
                printf("Invalid input. Enter one of the given numbers.\n");
                editInventories(privilege, user);
                break;
        } 
    }
}

void displayBasicInventory(){

    FILE *inv;
    inv = fopen("inventory.txt", "r+");

    struct Inventory item;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        printf("%d: %s\n", item.index, item.name);
    }

    fclose(inv);
}
void displayAdvancedInventory(){
    FILE *inv;
    inv = fopen("inventory.txt", "r+");

    if(inv == NULL){
        printf("An issue occurd while attempting to open the file.\n");
        return;
    }

    struct Inventory item;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        printf("%d: %s\t\tcount: %d \tcost: %.2f\n", item.index, item.name, item.quantity, item.cost);
    }

    fclose(inv);
}

void displayAdvancedInventoryPage(int privilege, char user[]){
    FILE *inv;
    inv = fopen("inventory.txt", "r+");

    if(inv == NULL){
        printf("An issue occurd while attempting to open the file.\n");
        editInventories(privilege, user);
        return;
    }

    struct Inventory item;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        printf("%d: %s\t\tcount: %d \tcost: %.2f\n", item.index, item.name, item.quantity, item.cost);
    }

    fclose(inv);

    char key;
    while(1){
        if(yesOrNo("Would you like to go back to the previous page?") == 'y'){
            clearConsole();
            editInventories(privilege, user);
            break;
        }
    }
    
}

void addItem(int privilege, char user[]){

    FILE *inv;
    FILE *temp;
    inv = fopen("inventory.txt", "r+");
    temp = fopen("temp.txt", "w+");

    if(inv == NULL){
        printf("Error connecting to inventory file.\n");
        editInventories(privilege, user);
        return;
    }

    struct Inventory item;
    char input[100];

    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        fprintf(temp, "%d,%s,%d,%.2f\n", item.index, item.name, item.quantity, item.cost);
    }

    // Read item name
    printf("Enter name of item: ");
    if(fgets(item.name, sizeof(item), stdin) != NULL){
        item.name[strcspn(item.name, "\n")] = '\0';
    }

    printf("Enter current quantity: ");
    if(fgets(input, sizeof(input), stdin) != NULL){
        sscanf(input, "%d", &item.quantity);
    }

    printf("Enter the retail price of the item: $");
    if(fgets(input, sizeof(input), stdin) != NULL){
        sscanf(input, "%f", &item.cost);
    }

    fprintf(temp, "%d,%s,%d,%.2f\n", ++(item.index), item.name, item.quantity, item.cost);

    fclose(inv);
    fclose(temp);

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    char proceed;
    if(yesOrNo("Would you like to add another item?") == 'y'){
        clearConsole();
        addItem(privilege, user);
    } else {
        clearConsole();
        editInventories(privilege, user);
    }
}

void updateItem(int privilege, char user[]){
    displayAdvancedInventory();

    char targetItem[STR_LEN];
    printf("Enter the name of the item to update: ");
    fgets(targetItem, sizeof(targetItem), stdin);
    targetItem[strcspn(targetItem, "\n")] = '\0';

    // Checks current user privileges and enables/disables 
    // item modification accordingly
    int command;
    if(privilege == ADMIN){
        printf("\n%s selected,\n1) Change Name\n2) Change Quantity\n3) Change Price\n\n", targetItem);
        command = getIntInput("Enter one of the given options: ");
    } else if(privilege == MANAGER){
        printf("\n%s selected,\n1) Change Name\n2) Change Quantity\n\n", targetItem);
        command = getIntInput("Enter one of the given options: ");
    } else {
        command = CHANGE_QUANTITY;
    }

    FILE *inv = fopen("inventory.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if(inv == NULL || temp == NULL){
        printf("Error opening files.\n");
        return;
    }

    struct Inventory item;
    char input[STR_LEN];
    char symbol;
    int found = 0;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        if(strcmp(item.name, targetItem) == 0){
            found = 1;
            printf("\n%s found.\tCurrent count: %d\tcost: $%.2f\n", item.name, item.quantity, item.cost);
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
                                printf("Invalid input. Disregarding changes.\n");
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
                    printf("Invalid input. No changes were made.\n");
                    break;
            }
            fprintf(temp, "%d,%s,%d,%.2f\n", item.index, item.name, item.quantity, item.cost);
        } else {
            fprintf(temp, "%d,%s,%d,%.2f\n", item.index, item.name, item.quantity, item.cost);
        }
    }

    fclose(inv);
    fclose(temp);

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    if(!found){
        printf("Item not found.\n\n");
        updateItem(privilege, user);
    } else {
        printf("\nItem updated successfully.\n");
        if(yesOrNo("Would you like to update another item?") == 'y'){
            clearConsole();
            updateItem(privilege, user);
        } else {
            clearConsole();
            editInventories(privilege, user);
        }
    }
}

void deleteItem(int privilege, char user[]){
    displayBasicInventory();

    char targetItem[STR_LEN];
    printf("Enter the name of the item to delete: ");
    fgets(targetItem, sizeof(targetItem), stdin);
    targetItem[strcspn(targetItem, "\n")] = '\0';

    FILE *inv;
    FILE *temp;

    inv = fopen("inventory.txt", "r");
    temp = fopen("temp.txt", "w");

    if(inv == NULL || temp == NULL){
        printf("Error opening files.\n");
        return;
    }

    int found = 0, count = 0;

    char input[10], confirmation;
    struct Inventory item;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        if(strcmp(targetItem, item.name) == 0){
            found = 1;
            if(yesOrNo("Are you sure you want to delete?") == 'y'){
                printf("%s successfully deleted.\n");
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

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    if(!found){
        while(1){
            if(yesOrNo("Item not found.\nWould you like to try again?") == 'y'){
                clearConsole();
                deleteItem(privilege, user);
                break;
            } else {
                clearConsole();
                editInventories(privilege, user);
                break;
            }
        }
    } else {
        if(yesOrNo("Would you like to delete another item?") == 'y'){
            clearConsole();
            deleteItem(privilege, user);
        } else {
            clearConsole();
            editInventories(privilege, user);
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
        printf("Invalid input. Please enter a valid number.\n");
    }
}

char yesOrNo(const char *prompt){
    char inputStr[STR_LEN], answer;
    while(1){
        printf("%s (y/n): ", prompt);
        if(fgets(inputStr, STR_LEN, stdin) != NULL){
            if(sscanf(inputStr, " %c", &answer) == 1){
                answer = tolower(answer);
                if(answer == 'y' || answer == 'n'){
                    return answer;
                }
            }
        }
        printf("Invalid input. Please enter 'y' or 'n'.\n");
    }
}

void menu(int privilege, char user[]){
    switch(privilege){
        case ADMIN:
            adminMenu(privilege, user);
            break;
        case MANAGER:
            managerMenu(privilege, user);
            break;
        case EMPLOYEE:
            employeeMenu(privilege, user);
            break;
        default:
            printf("An unexpected error occurd. Exiting application.");
            exitApp();
            break;
            
    }
}

void adminMenu(int privilege, char user[]){
    clearConsole();
    printf("Hello %s\n", user);
    int action = 0;

    printf("1) Edit Inventories \n2) Edit Users\n3) Logout\n4) Exit Application\n\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1: // Edit Inventories
            clearConsole();
            editInventories(privilege, user);
            break;
        case 2: // Edit Users
            clearConsole();
            editUsers(privilege, user);
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
            printf("Invalid input. Please enter one of the given numbers.\n");
            menu(privilege, user);
            break;
    }
}

void managerMenu(int privilege, char user[]){
    clearConsole();
    printf("Hello %s\n", user);
    int action = 0;

    printf("1) Edit Inventories \n2) Edit Users\n3) Logout\n4) Exit Application\n\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1: // Edit Inventories
            clearConsole();
            editInventories(privilege, user);
            break;
        case 2: // Edit Users
            clearConsole();
            editUsers(privilege, user);
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
            printf("Invalid input. Please enter one of the given numbers.\n");
            menu(privilege, user);
            break;
    }
}

void employeeMenu(int privilege, char user[]){
    clearConsole();
    printf("Hello %s\n", user);
    int action = 0;

    printf("1) Edit Inventories\n2) Logout\n3) Exit Application\n\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1: // Edit Inventories
            clearConsole();
            editInventories(privilege, user);
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
            printf("Invalid input. Please enter one of the given numbers.\n");
            menu(privilege, user);
            break;
    }
}
