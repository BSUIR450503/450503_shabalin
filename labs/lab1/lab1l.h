#if (defined __linux__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#endif

#if (defined WIN32)
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

int runprocess();

