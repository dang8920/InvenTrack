#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#define ADMIN 1
#define MANAGER 2
#define EMPLOYEE 3

#define CLEAR_CONSOLE 1
#define KEEP_CONSOLE 0

void login(int clearOption);
void exitApp();
void clearConsole();
void delay(int number_of_seconds);

void getUserData(char currentID[], char currentPassword[]);
void overview();
void editUsers();
void editInventories(int privilege, char name[]);
    void displayInventory(int privilege, char name[]);
    void addItem(int privilege, char name[]);
    void updateItem(int privilege, char name[]);
    void deleteItem(int privilege, char name[]);
    void saveInventory(int privilege, char name[]);

void getPassword(char *password, int maxLength); // Allows users to enter their password without having other people seeing
int getIntInput(const char *prompt);

void menu(int privilege, char name[]);
void adminMenu(int privilege, char name[]);
void managerMenu(int privilege, char name[]);
void employeeMenu(int privilege, char name[]);

struct user{
    char name[50];
    char surname[50];
    char id[12];
    char password[50];
    int role;
};

struct Inventory{

};

int main(){
    struct user current;
    current.role = ADMIN;
    login(CLEAR_CONSOLE);
    
    return 0;
}

void login(int clearOption){
    if(clearOption == 1){
        clearConsole();
    }

    char currentId[10];
    char currentPassword[50];
    printf("Enter id: ");
    scanf("%s", currentId);

    printf("Enter password: ");
    getPassword(currentPassword, 50);
    getUserData(currentId, currentPassword);
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

void getUserData(char currentID[], char currentPassword[]){
    FILE *userFile;
    userFile = fopen("users.txt", "r");

    if(userFile == NULL){
        printf("Error retrieving user data\n");
        printf("Try again in 3...");
        delay(1);
        printf("2...");
        delay(1);
        printf("1...");
        delay(1);
        login(CLEAR_CONSOLE);
        return;
    }

    int found = 0;

    char fileID[50], name[50], surname[50], filePassword[50];
    int role;
    while(fscanf(userFile, "%[^,],%[^,],%[^,],%[^,],%d\n", fileID, name, surname, filePassword, &role) == 5){
        if(strcmp(fileID, currentID) == 0 && strcmp(filePassword, currentPassword) == 0){
            found = 1;
            break;
        }
    }

    if(found){
        menu(role, name);
    } else {
        printf("Invalid ID or Password\nTry again\n\n");
        login(KEEP_CONSOLE);
    }

    fclose(userFile);
}

void overview(){

}

void editUsers(){
    FILE *userFile;
    userFile = fopen("users.txt", "a+");

    if(userFile == NULL){
        printf("Error opening file!\n");
        return;
    }

    struct user newUser;
    printf("Enter first name: ");
    scanf("%s", newUser.name);

    printf("Enter last name: ");
    scanf("%s", newUser.surname);

    printf("Enter ID: ");
    scanf("%s", newUser.id);

    printf("Enter password: ");
    scanf("%s", newUser.password);

    printf("Enter role: ");
    scanf("%d", &newUser.role);

    printf("\n");
    fprintf(userFile, "%s,%s,%s,%s,%d\n", newUser.id, newUser.name, newUser.surname, newUser.password, newUser.role);

    fclose(userFile);
}

void editInventories(int privilege, char name[]){
    int action = 0;

    printf("1) Display Inventory\n2) Add New Items\n3) Update Exsisting Items\n4) Delete Exsisting Items\n5) Back\n6) Exit Application\n\n");
    action = getIntInput("Enter one of the given options: ");

    switch(action){
        case 1:
            printf("\n");
            displayInventory(privilege, name);
            break;
        case 2:
            printf("\n");
            addItem(privilege, name);
            break;
        case 3:
            printf("\n");
            updateItem(privilege, name);
            break;
        case 4:
            printf("\n");
            deleteItem(privilege, name);
            break;
        case 5:
            printf("\n");
            menu(privilege, name);
            break;
        case 6:
            printf("\n");
            exitApp();
            break;
        default:
            clearConsole();
            printf("Invalid input. Enter one of the given numbers.\n");
            editInventories(privilege, name);
            break;
    }
}

void displayInventory(int privilege, char name[]){
    clearConsole();
    FILE *inv;
    inv = fopen("inventory.txt", "r");

    if(inv == NULL){
        printf("An issue occurd while attempting to open the file.\n");
        editInventories(privilege, name);
        return;
    }

    char itemName[50];
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
        printf("Enter b to go back: ");
        scanf("%c", &key);

        if(key == 'b' || key == 'B'){
            clearConsole();
            editInventories(privilege, name);
            break;
        } else {
            printf("Invalid input\n");
        }
    }
    
}

void addItem(int privilege, char name[]){
    clearConsole();

    FILE *inv;
    inv = fopen("inventory.txt", "a+");

    if(inv == NULL){
        printf("Error connecting to inventory file.\n");
        editInventories(privilege, name);
        return;
    }

    char item[50];
    int quantity;
    float cost;
    printf("Enter name of item: ");
    fgets(item, 50, stdin);
    item[strcspn(item, "\n")] = '\0';

    printf("Enter current quantity: ");
    scanf("%d", &quantity);

    printf("Enter the retail price of the item: $");
    scanf("%f", &cost);

    fprintf(inv, "%s,%d,%.2f\n", item, quantity, cost);
    fclose(inv);

    char proceed;
    while(1){
        printf("Would you like to enter another item? (y/n): ");
        scanf("%c", &proceed);
        if(proceed == 'Y' || proceed == 'y'){
            clearConsole();
            addItem(privilege, name);
            break;
        } else if(proceed == 'N' || proceed == 'n'){
            clearConsole();
            editInventories(privilege, name);
            break;
        } else {
            printf("Invalid input\n");
        }
    }
}

void updateItem(int privilege, char name[]){

}

void deleteItem(int privilege, char name[]){

}

void saveInventory(int privilege, char name[]){

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
    int value;
    int result;

    while(1){
        printf("%s", prompt);

        result = scanf("%d", &value);

        if(result == 1){
            while(getchar() != '\n');
            return value;
        } else {
            while(getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
        }
    }
}

void menu(int privilege, char name[]){
    clearConsole();
    switch(privilege){
        case ADMIN:
            adminMenu(privilege, name);
            break;
        case MANAGER:
            managerMenu(privilege, name);
            break;
        case EMPLOYEE:
            employeeMenu(privilege, name);
            break;
        default:
            printf("An unexpected error occurd. Exiting application.");
            exitApp();
            break;
            
    }
}

void adminMenu(int privilege, char name[]){
    clearConsole();
    printf("Hello %s\n", name);
    int action = 0;

    printf("1) Overview\n2) Edit Inventories \n3) Edit users\n4) Exit\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1:
            printf("\nYou have entered the \"Overview\" page\n");
            break;
        case 2:
            clearConsole();
            editInventories(privilege, name);
            break;
        case 3:
            printf("\n");
            editUsers(name);
            break;
        case 4:
            printf("\n");
            exitApp();
            break;
        default:
            printf("\n");
            printf("Invalid input. Please enter one of the given numbers.\n");
            break;
    }
}

void managerMenu(int privilege, char name[]){
    clearConsole();
    printf("Hello %s\n", name);
    int action = 0;

    printf("1) Overview\n2) Edit Inventories \n3) Exit\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1:
            printf("\nYou have entered the \"Overview\" page\n");
            break;
        case 2:
            clearConsole();
            editInventories(privilege, name);
            break;
        case 3:
            printf("\n");
            editUsers();
            break;
        case 4:
            printf("\n");
            exitApp();
            break;
        default:
            printf("\n");
            printf("Invalid input. Please enter one of the given numbers.\n");
            break;
    }
}

void employeeMenu(int privilege, char name[]){
    clearConsole();
    printf("Hello %s\n", name);
    int action = 0;

    printf("1) Edit Inventories \n2) Exit\n");
    action = getIntInput("Enter one of the options: ");
    

    switch(action){
        case 1:
            printf("\nYou have entered the \"Overview\" page\n");
            break;
        case 2:
            clearConsole();
            editInventories(privilege, name);
            break;
        case 3:
            printf("\n");
            editUsers();
            break;
        case 4:
            printf("\n");
            exitApp();
            break;
        default:
            printf("\n");
            printf("Invalid input. Please enter one of the given numbers.\n");
            break;
    }
}
