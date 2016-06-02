#ifdef _WIN32
#include <windows.h>
#include <process.h>
#endif

#ifdef __linux__
#include <linux/sched.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h> 
#include <time.h>
#include <pthread.h>
#include <string.h>

#include <dirent.h>
#include <aio.h>
#include <fcntl.h>
#include <dlfcn.h>
#endif

#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;