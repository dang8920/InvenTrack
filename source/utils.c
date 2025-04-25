// utils.c
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

#include "utils.h"

FILE *openFileChecked(const char *filename, const char *mode, const char *context){
    FILE *fp = fopen(filename, mode);
    if(fp == NULL){
        printf("❌ Error opening file for %s: %s\n", context, filename);
    }
    return fp;
}

void closeFileSafe(FILE *fp, const char *context){
    if(fp != NULL){
        fclose(fp);
    } else {
        printf("⚠️ Tried to close a NULL file pointer (%s)\n", context);
    }
}

void clearConsole(){
    printf("\e[1;1H\e[2J");
}

void delay(int numberOfSeconds){
    #ifdef _WIN32
        Sleep(1000 * numberOfSeconds);
    #else
        sleep(numberOfSeconds);
    #endif
}

void exitApp(){
    clearConsole();
    printf("Exiting InvenTrack in...");
    printf("3..."); fflush(stdout); delay(1);
    printf("2..."); fflush(stdout); delay(1);
    printf("1..."); fflush(stdout); delay(1);
    printf("\n");
}

void flushInput(){
    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}

int cancelProcess(char string[]){
    return strcmp(string, "CANCEL") == 0;
}

void getSanitizedInput(char *dest, int size){
    if(fgets(dest, size, stdin) != NULL){
        dest[strcspn(dest, "\n")] = '\0';
    }
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

char promptYesOrNo(const char *prompt){
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

void getPassword(char *password, int maxLength){
    int i = 0;
    char ch;

    #ifdef _WIN32
        while(1){
            ch = getch();
            if(ch == 13) break; // Enter
            if(ch == 8 && i > 0){
                i--; printf("\b \b");
            } else if(i < maxLength - 1){
                password[i++] = ch;
                printf("*");
            }
        }
    #else
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        while((ch = getchar()) != '\n' && i < maxLength - 1){
            if(ch == 127 || ch == 8){
                if(i > 0){
                    i--; printf("\b \b");
                    fflush(stdout);
                }
            } else {
                password[i++] = ch;
                printf("*");
                fflush(stdout);
            }
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    #endif

    password[i] = '\0';
    printf("\n");
}

int handleMenu(const char *greeting, const char *options[], int optionCount){
    int action;
    printf("%s\n", greeting);

    for(int i = 0; i < optionCount; i++){
        printf("%s\n", options[i]);
    }

    printf("\n");
    action = getIntInput("Enter one of the options: ");
    return action;
}
