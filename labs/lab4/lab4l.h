#if (defined WIN32)
#include <windows.h>
#include <conio.h>
#endif

#if (defined __linux__)
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <linux/sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#define NB_ENABLE 1
#define NB_DISABLE 0
#define MAX_COUNT 20
static int num = 0;

#if (defined __linux__)
void * print_routine(void * n);
#endif
#if (defined WIN32)
DWORD WINAPI print_routine(void * n);
#endif

class myprocess
{
public:
    int this_thread_number;
	int number;
#if (defined __linux__)
    pthread_t * thread = new pthread_t();
    pthread_mutex_t * printmutex = new pthread_mutex_t();
    pthread_mutex_t * breakmutex = new pthread_mutex_t();
#endif
#if (defined WIN32)
    HANDLE this_thread_handle;
    CRITICAL_SECTION printmutex,breakmutex;
    DWORD threadID;
#endif
	myprocess();
    myprocess(int & thread_number);
    bool nextcanprint();
    void printcompleted();
    bool isBreak();
    bool isCompleted();
    void allowprint();
    void endprint();
};

#if (defined __linux__)
int kbhit();
void non_block(int state);
char mygetch();
#endif
#if (defined WIN32)
int mygetch();
#endif