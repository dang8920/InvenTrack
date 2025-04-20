#include <stdio.h>
#include <time.h>
#include "source/utils.h"
#include "source/user.h"
#include "source/menus.h"
#include "source/inventory.h"

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

int main(){
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #else
    #endif
    clearConsole();
    login();
    
    return 0;
}

