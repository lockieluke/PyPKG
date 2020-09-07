//
// Created by Sherlock Luk on 9/2/2020.
//

#include "console.h"

#ifndef UNICODE
#endif

void WriteLine(char* message) {
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), message, strlen(message), 0, nullptr);
}
