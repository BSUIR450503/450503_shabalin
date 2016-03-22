#if (defined __linux__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#define proc pid_t
#endif

#if (defined WIN32)
#include <windows.h>
#include <conio.h>
#define proc HANDLE
#endif

#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

#if (defined __linux__)
int getch();
#endif

int start();
proc addprocess();
void removeprocess();
void endprocess();