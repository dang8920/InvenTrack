#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "user.h"
#include "menus.h"
#include "inventory.h"

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

void showInventoryInformation(){
    FILE *inv = fopenInvFileRead();

    if(inv == NULL){
        perror("❌ Error displaying contents of the file");
        return;
    }

    struct Inventory item;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        printf("%5d: %30s\tcount: %d \tcost: $%.2f\n", item.index, item.name, item.quantity, item.cost);
    }

    closeFileSafe(inv, "editInventory: inventory database");
}

void inventoryPage(int userPrivilege, char user[]){
    printf("============ Inventory 📦 ============\n");
    FILE *inv = fopenInvFileRead();

    if(inv == NULL){
        perror("❌ Error displaying inventoy items");
        editInventory(userPrivilege, user);
        return;
    }

    struct Inventory item;
    while(fscanf(inv, "%d,%[^,],%d,%f\n", &item.index, item.name, &item.quantity, &item.cost) == 4){
        printf("%5d: %30s\tcount: %d \tcost: $%.2f\n", item.index, item.name, item.quantity, item.cost);
    }

    closeFileSafe(inv, "showInventoryInormation: inventory database");

    char key;
    while(1){
        if(promptYesOrNo("Would you like to go back to the previous page?") == 'y'){
            clearConsole();
            editInventory(userPrivilege, user);
            break;
        }
    }
    
}

void addItem(int userPrivilege, char user[]){
    showInventoryInformation();
    printf("============ Add Items ============\n");
    printf("Enter CANCEL to cancel\n\n");

    FILE *inv = fopenInvFileRead();
    FILE *temp = fopenTempFileWrite();

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
    getSanitizedInput(item.name, STR_LEN);
    // Cancels adding a new user
    if(strcmp(item.name, "CANCEL") == 0){
        closeFileSafe(inv, "addItem: inventory database");
        closeFileSafe(temp, "addItem: temp file");

        remove(FILE_INVENTORY);
        rename(FILE_TEMP, FILE_INVENTORY);
        clearConsole();
        editInventory(userPrivilege, user);
        return;
    }

    printf("Enter quantity: ");
    getSanitizedInput(input, STR_LEN);
    // Cancels adding a new user
    if(strcmp(input, "CANCEL") == 0){
        closeFileSafe(inv, "addItem: inventory database");;
        closeFileSafe(temp, "addItem: temp file");
        
        remove(FILE_INVENTORY);
        rename(FILE_TEMP, FILE_INVENTORY);
        clearConsole();
        editInventory(userPrivilege, user);
        return;
    } else {
        sscanf(input, "%d", &item.quantity);
    }

    printf("Enter the price of the item: $");
    getSanitizedInput(input, STR_LEN);
    // Cancels adding a new user
    if(strcmp(input, "CANCEL") == 0){
        closeFileSafe(inv, "addItem: inventory database");
        closeFileSafe(temp, "addItem: temp file");
        
        remove(FILE_INVENTORY);
        rename(FILE_TEMP, FILE_INVENTORY);
        clearConsole();
        editInventory(userPrivilege, user);
        return;
    } else {
        sscanf(input, "%f", &item.cost);
    }

    fprintf(temp, "%d,%s,%d,%.2f\n", ++(item.index), item.name, item.quantity, item.cost);
    printf("✅ %s successfully add to inventory\n", item.name);

    closeFileSafe(inv, "addItem: inventory database");
    closeFileSafe(temp, "addItem: temp file");

    remove(FILE_INVENTORY);
    rename(FILE_TEMP, FILE_INVENTORY);

    char proceed;
    if(promptYesOrNo("Would you like to add another item?") == 'y'){
        clearConsole();
        addItem(userPrivilege, user);
    } else {
        clearConsole();
        editInventory(userPrivilege, user);
    }
}

void updateItem(int userPrivilege, char user[]){
    showInventoryInformation();
    printf("\n============ Update Items ============\n");
    printf("Enter CANCEL to cancel\n");

    char targetItem[STR_LEN];
    printf("\nEnter the name of the item to update: ");
    getSanitizedInput(targetItem, STR_LEN);
    if(cancelProcess(targetItem)){
        clearConsole();
        editInventory(userPrivilege, user);
        return;
    }

    FILE *inv = fopenInvFileRead();
    FILE *temp = fopenTempFileWrite();

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
            int command;
            // Checks current user privileges and enables/disables 
            // item modification accordingly
            if(userPrivilege == ADMIN){
                printf("\n%s found,\n1) Change Name\n2) Change Quantity\n3) Change Price\n4) Cancel\n\n", targetItem);
                command = getIntInput("Enter one of the given options: ");
                if(command == 4){
                    editInventory(userPrivilege, user); // Previous page
                    return; // Prevents shutdown bugs
                }
            } else if(userPrivilege == MANAGER){
                printf("\n%s found,\n1) Change Name\n2) Change Quantity\n3) Cancel\n\n", targetItem);
                command = getIntInput("Enter one of the given options: ");
                if(command == 3){
                    editInventory(userPrivilege, user); // Previous page
                    return; // Prevents shutdown bugs
                }
            } else {
                command = CHANGE_QUANTITY;
            }
            switch(command){
                case CHANGE_NAME:
                    printf("Enter new name: ");
                    getSanitizedInput(item.name, STR_LEN);
                    if(cancelProcess(item.name)){
                        closeFileSafe(inv, "updateItem: inventory database");
                        closeFileSafe(temp, "updateItem: temp file");
                        remove(FILE_TEMP);
                        editInventory(userPrivilege, user);
                        break;
                        return;
                    }
                    break;
                case CHANGE_QUANTITY:
                    printf("Enter updated quantity: ");

                    getSanitizedInput(input, STR_LEN);
                    if(cancelProcess(item.name)){
                        closeFileSafe(inv, "updateItem: inventory database");
                        closeFileSafe(temp, "updateItem: temp file");
                        remove(FILE_TEMP);
                        editInventory(userPrivilege, user);
                        break;
                        return;
                    } else if(sscanf(input, "%d", &item.quantity) == 1){

                    } else {
                            printf("❌ Invalid input. Disregarding changes.\n");
                    }
                    break;
                case CHANGE_PRICE:
                    printf("Enter updated price: $");
                    getSanitizedInput(input, STR_LEN);
                    if(cancelProcess(item.name)){
                        closeFileSafe(inv, "updateItem: inventory database");
                        closeFileSafe(temp, "updateItem: temp file");
                        remove(FILE_TEMP);
                        editInventory(userPrivilege, user);
                        break;
                        return;
                    } else {
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

    closeFileSafe(inv, "updateItem: inventory database");
    closeFileSafe(temp, "updateItem: temp file");

    remove(FILE_INVENTORY);
    rename(FILE_TEMP, FILE_INVENTORY);

    if(!found){
        printf("❌ Item not found.\n\n");
        updateItem(userPrivilege, user);
    } else {
        printf("\n✅ Item updated successfully.\n");
        if(promptYesOrNo("Would you like to update another item?") == 'y'){
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
    showInventoryInformation();
    printf("Enter CANCEL to cancel\n\n");

    char targetItem[STR_LEN];
    printf("Enter the name of the item to delete: ");
    getSanitizedInput(targetItem, STR_LEN);
    if(cancelProcess(targetItem)){
        clearConsole();
        editInventory(userPrivilege, user);
        return;
    }

    FILE *inv = fopenInvFileRead();
    FILE *temp = fopenTempFileWrite();

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
            closeFileSafe(inv, "deleteItem: inventory database");
            closeFileSafe(temp, "deleteItem: temp file");

            remove(FILE_INVENTORY);
            rename(FILE_TEMP, FILE_INVENTORY);
            clearConsole();
            editInventory(userPrivilege, user);
            return;
        }
        if(strcmp(targetItem, item.name) == 0){
            found = 1;
            if(promptYesOrNo("Are you sure you want to proceed with deletion?") == 'y'){
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

    closeFileSafe(inv, "deleteItem: inventory database");
    closeFileSafe(temp, "deleteItem: temp file");

    remove(FILE_INVENTORY);
    rename(FILE_TEMP, FILE_INVENTORY);

    if(!found){
        while(1){
            if(promptYesOrNo("❌ Item not found.\nWould you like to try again?") == 'y'){
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
        if(promptYesOrNo("Would you like to delete another item?") == 'y'){
            clearConsole();
            deleteItem(userPrivilege, user);
        } else {
            clearConsole();
            editInventory(userPrivilege, user);
        }
    }
}

