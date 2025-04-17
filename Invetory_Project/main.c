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

#define STR_LEN 50

void login();
void exitApp();
void clearConsole();
void delay(int number_of_seconds);
void flushInput();

void getUserData(char currentID[], char currentPassword[]);
void overview();
void editUsers(int privilege, char user[]);
    void displayUser(int privilege, char user[]);
    void addUser(int privilege, char user[]);
    void updateUser(int privilege, char user[]);
    void deleteUser(int privilege, char user[]);
void editInventories(int privilege, char user[]);
    void displayInventory(int privilege, char user[]);
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

struct user{
    char name[STR_LEN];
    char surname[STR_LEN];
    char id[STR_LEN];
    char password[STR_LEN];
    int role;
};

struct Inventory{

};

int main(){
    clearConsole();
    login();
    
    return 0;
}

void login(){
    char currentID[STR_LEN], currentPassword[STR_LEN];

    printf("Enter ID: ");
    fgets(currentID, STR_LEN, stdin);
    currentID[strcspn(currentID, "\n")] = '\0';

    printf("Enter password: ");
    fgets(currentPassword, STR_LEN, stdin);
    currentPassword[strcspn(currentPassword, "\n")] = '\0';

    getUserData(currentID, currentPassword);
}

void exitApp(){
    clearConsole();
    printf("Exiting InvenTrack in...");
    printf("3...");delay(1);
    printf("2...");delay(1);
    printf("1...");delay(1);
}

void clearConsole(){
    printf("\e[1;1H\e[2J"); // Clears the console on any OS
}

void delay(int number_of_seconds)
{
	// Converting time into milli_seconds
	int milli_seconds = 1000 * number_of_seconds;

	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + milli_seconds)
		;
}

void flushInput(){
    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}

void getUserData(char currentID[], char currentPassword[]){
    FILE *userFile;
    userFile = fopen("user_database.txt", "r");

    if(userFile == NULL){
        printf("Error retrieving user data\n");
        printf("Try again in 3...");
        delay(1);
        printf("2...");
        delay(1);
        printf("1...");
        delay(1);

        clearConsole();
        login();
        return;
    }

    int found = 0;

    char fileID[STR_LEN], name[STR_LEN], surname[STR_LEN], filePassword[STR_LEN];
    int role;
    while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", fileID, name, surname, filePassword, &role) == 5){
        if(strcmp(fileID, currentID) == 0 && strcmp(filePassword, currentPassword) == 0){
            found = 1;
            break;
        }
    }

    fclose(userFile);

    if(found){
        menu(role, name);
    } else {
        printf("Invalid ID or Password\nTry again\n\n");
        login();
    }

}

void overview(){

}

void editUsers(int privilege, char user[]){
    clearConsole();

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

    
}

void addUser(int privilege, char user[]){
    FILE *userFile;
    userFile = fopen("user_database.txt", "a+");

    if(userFile == NULL){
        printf("Error opening file!\n");
        return;
    }

    char buffer[STR_LEN];
    char name[STR_LEN], surname[STR_LEN], id[STR_LEN], password[STR_LEN];
    int role;

    printf("Enter first name: ");
    fgets(name, STR_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter last name: ");
    fgets(surname, STR_LEN, stdin);
    surname[strcspn(surname, "\n")] = '\0';

    printf("Enter ID: ");
    fgets(id, STR_LEN, stdin);
    id[strcspn(id, "\n")] = '\0';

    printf("Enter password: ");
    fgets(password, STR_LEN, stdin);
    password[strcspn(password, "\n")] = '\0';

    role = getIntInput("Enter role (numeric): ");

    fprintf(userFile, "%s,%s,%s,%s,%d\n", id, name, surname, password, role);
    fclose(userFile);

    printf("User added: %s %s (%s) role=%d\n", name, surname, id, role);

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

    FILE *userFile = fopen("user_database.txt", "r+");
    FILE *temp = fopen("temp.txt", "w+");

    if(userFile == NULL || temp == NULL){
        perror("Error opening files");
        return;
    }

    int found = 0, role;
    char id[STR_LEN], name[STR_LEN], surname[STR_LEN], password[STR_LEN];
    char input1[STR_LEN], input2[STR_LEN];

    while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", id, name, surname, password, &role) == 5){
        if(strcmp(targetID, id) == 0){
            found = 1;
            printf("\nCurrently editing %s's profile\n", name);
            printf("1) Change ID\n2) Change Name\n3) Change Password\n4) Change Role\n5) Cancel\n");

            int action = getIntInput("Enter an option: ");

            switch(action){
                case 1:
                    printf("Enter NEW ID: ");
                    fgets(input1, STR_LEN, stdin);
                    input1[strcspn(input1, "\n")] = '\0';

                    printf("Confirm NEW ID: ");
                    fgets(input2, STR_LEN, stdin);
                    input2[strcspn(input2, "\n")] = '\0';

                    if(strcmp(input1, input2) == 0){
                        strcpy(id, input1);
                        printf("ID successfully updated.\n");
                    } else {
                        printf("IDs do not match. Keeping old ID.\n");
                    }
                    break;
                case 2:
                    printf("Enter NEW First Name: ");
                    fgets(name, STR_LEN, stdin);
                    name[strcspn(name, "\n")] = '\0';

                    printf("Enter NEW Last Name: ");
                    fgets(surname, STR_LEN, stdin);
                    surname[strcspn(surname, "\n")] = '\0';

                    printf("First and last name successfully updated.\n");
                    break;
                case 3:
                    printf("Enter NEW password: ");
                    fgets(input1, STR_LEN, stdin);
                    input1[strcspn(input1, "\n")] = '\0';

                    printf("Confirm NEW password: ");
                    fgets(input2, STR_LEN, stdin);
                    input2[strcspn(input2, "\n")] = '\0';

                    if(strcmp(input1, input2) == 0){
                        strcpy(password, input1);
                        printf("Password successfully updated.\n");
                    } else {
                        printf("Passwords do not match. Keeping old password.\n");
                    }
                    break;
                case 4:
                    role = getIntInput("Enter NEW role (numeric): ");
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

        fprintf(temp, "%s,%s,%s,%s,%d\n", id, name, surname, password, role);
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
    
    char targetID[STR_LEN], id[STR_LEN], name[STR_LEN], surname[STR_LEN], password[STR_LEN], tempName[STR_LEN], input[STR_LEN];
    int role, found = 0;
    printf("Enter the ID number of the user to be deleted: ");
    if(fgets(targetID, STR_LEN, stdin) != NULL){
        targetID[strcspn(targetID, "\n")] = '\0';

        while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", id, name, surname, password, &role) == 5){
            if(strcmp(targetID, id) == 0){
                found = 1;
                strcpy(tempName, name);
                continue;
            } else {
                fprintf(temp, "%s,%s,%s,%s,%d\n", id, name, surname, password, role);
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

    printf("1) Display Inventory\n2) Add New Items\n3) Update Exsisting Items\n4) Delete Exsisting Items\n5) Back\n6) Exit Application\n\n");
    action = getIntInput("Enter one of the given options: ");

    switch(action){
        case 1:
        clearConsole();
            printf("\n");
            displayInventory(privilege, user);
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
}

void displayInventory(int privilege, char user[]){
    FILE *inv;
    inv = fopen("inventory.txt", "r");

    if(inv == NULL){
        printf("An issue occurd while attempting to open the file.\n");
        editInventories(privilege, user);
        return;
    }

    char itemName[STR_LEN];
    int itemQuantity;
    float itemPrice;
    while(fscanf(inv, "%[^,],%d,%f\n", itemName, &itemQuantity, &itemPrice) == 3){
        printf("\t%s\n", itemName);
        printf("Quantity: %d\n", itemQuantity);
        printf("Cost: $%.2f\n\n", itemPrice);
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
    inv = fopen("inventory.txt", "a+");

    if(inv == NULL){
        printf("Error connecting to inventory file.\n");
        editInventories(privilege, user);
        return;
    }

    char item[STR_LEN];
    int quantity;
    float cost;
    char input[100];

    // Read item name
    printf("Enter name of item: ");
    if(fgets(item, sizeof(item), stdin) != NULL){
        item[strcspn(item, "\n")] = '\0';
    }

    printf("Enter current quantity: ");
    if(fgets(input, sizeof(input), stdin) != NULL){
        sscanf(input, "%d", &quantity);
    }

    printf("Enter the retail price of the item: $");
    if(fgets(input, sizeof(input), stdin) != NULL){
        sscanf(input, "%f", &cost);
    }

    fprintf(inv, "%s,%d,%.2f\n", item, quantity, cost);
    fclose(inv);

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

    char targetItem[STR_LEN];
    printf("Enter the name of the item to update: ");
    fgets(targetItem, sizeof(targetItem), stdin);
    targetItem[strcspn(targetItem, "\n")] = '\0';

    FILE *inv = fopen("inventory.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if(inv == NULL || temp == NULL){
        printf("Error opening files.\n");
        return;
    }

    char item[STR_LEN];
    char input[STR_LEN];
    char symbol;
    int quantity;
    float cost;
    int found = 0;
    while(fscanf(inv, "%[^,],%d,%f\n", item, &quantity, &cost) == 3){
        if(strcmp(item, targetItem) == 0){
            found = 1;
            printf("Item found.\nEnter updated quantity or symbol (+/-): ");
            if(fgets(input, sizeof(input), stdin) != NULL){
                if(sscanf(input, "%d", &quantity) == 1){} 
                else if(sscanf(input, "%c", &symbol) == 1){
                    if(symbol == '+'){
                        quantity++;
                    } else if(symbol == '-'){
                        quantity--;
                    } else {
                        printf("Invalid input. Disregarding changes.\n");
                    }
                }
            }
            printf("Enter updated price: $");
            if(fgets(input, STR_LEN, stdin) != NULL){
                sscanf(input, "%f", &cost);
            }

            fprintf(temp, "%s,%d,%.2f\n", item, quantity, cost);
        } else {
            fprintf(temp, "%s,%d,%.2f\n", item, quantity, cost);
        }
    }

    fclose(inv);
    fclose(temp);

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    if(!found){
        printf("Item not found.\n\n");
    } else {
        printf("Item updated successfully.\n");
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

    int found = 0;

    char item[STR_LEN], input[10], confirmation;
    int quantity;
    float cost;
    while(fscanf(inv, "%[^,],%d,%f\n", item, &quantity, &cost) == 3){
        if(strcmp(targetItem, item) == 0){
            found = 1;
            if(yesOrNo("Are you sure you want to delete?") == 'y'){
                printf("%s successfully deleted.\n");
                continue;
            } else {
                fprintf(temp, "%s,%d,%.2f\n", item, quantity, cost);
            }
        } else {
            fprintf(temp, "%s,%d,%.2f\n", item, quantity, cost);
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

    printf("1) Overview\n2) Edit Inventories \n3) Edit users\n4) Exit\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1:
            printf("\nYou have entered the \"Overview\" page\n");
            break;
        case 2:
            clearConsole();
            editInventories(privilege, user);
            break;
        case 3:
            printf("\n");
            editUsers(privilege, user);
            break;
        case 4:
            printf("\n");
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

    printf("1) Overview\n2) Edit Inventories \n3) Exit\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1:
            printf("\nYou have entered the \"Overview\" page\n");
            break;
        case 2:
            clearConsole();
            editInventories(privilege, user);
            break;
        case 3:
            printf("\n");
            editUsers(privilege, user);
            break;
        case 4:
            printf("\n");
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

    printf("1) Edit Inventories \n2) Exit\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1:
            printf("\nYou have entered the \"Overview\" page\n");
            break;
        case 2:
            clearConsole();
            editInventories(privilege, user);
            break;
        case 3:
            printf("\n");
            editUsers(privilege, user);
            break;
        case 4:
            printf("\n");
            exitApp();
            break;
        default:
            printf("\n");
            printf("Invalid input. Please enter one of the given numbers.\n");
            menu(privilege, user);
            break;
    }
}
