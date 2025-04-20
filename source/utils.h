// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define STR_LEN 50

// File Handling
FILE *openFileChecked(const char *filename, const char *mode, const char *context);
void closeFileSafe(FILE *fp, const char *context);

// Input
int getIntInput(const char *prompt);
char promptYesOrNo(const char *prompt);
void getPassword(char *password, int maxLength);
void flushInput();
int cancelProcess(char string[]);
void getSanitizedInput(char *dest, int size);

// Console Helpers
void clearConsole();
void delay(int numberOfSeconds);
void exitApp();
int handleMenu(const char *greeting, const char *options[], int optionCount);

#endif
