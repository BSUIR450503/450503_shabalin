#include "lab4l.h"

#if (defined WIN32)
CRITICAL_SECTION readymutex;
myprocess::myprocess(int & thread_number)
{
	num++;
    this_thread_number = thread_number;
    InitializeCriticalSection(&printmutex);
    InitializeCriticalSection(&breakmutex);
    if (thread_number == 1)
    {
        InitializeCriticalSection(&readymutex);
    }
    this_thread_handle = CreateThread(NULL,100,print_routine,(void*)this,0,&threadID);
}
#endif
#if (defined __linux__)
pthread_mutex_t * readymutex = new pthread_mutex_t();
myprocess::myprocess(int &thread_number)
{
	num++;
	this_thread_number = thread_number;

	pthread_mutex_init(printmutex,NULL);
	pthread_mutex_lock(printmutex);
	pthread_mutex_init(breakmutex,NULL);
	if (thread_number == 1)
	{
		pthread_mutex_init(readymutex,NULL);
	}
	pthread_create(thread,NULL,print_routine, (void*)this);
	}
#endif

myprocess::myprocess(){}

bool myprocess::nextcanprint()
{
#if (defined __linux__)
    if (pthread_mutex_trylock(readymutex) != 0) return false;
    pthread_mutex_unlock(readymutex);
#endif
#if (defined WIN32)
    if (TryEnterCriticalSection(&readymutex) == 0) return false;
    LeaveCriticalSection(&readymutex);
#endif
	return true;
}

void myprocess::printcompleted()
{
#if (defined __linux__)
    pthread_mutex_lock(printmutex);
    pthread_mutex_unlock(readymutex);
#endif
#if (defined WIN32)
    LeaveCriticalSection(&printmutex);
    LeaveCriticalSection(&readymutex);
#endif
}

bool myprocess::isBreak()
{
#if (defined __linux__)
    if (pthread_mutex_trylock(breakmutex) != 0) return true;
    pthread_mutex_unlock(breakmutex);
#endif
#if (defined WIN32)
    if (TryEnterCriticalSection(&breakmutex) == 0) return true;
    LeaveCriticalSection(&breakmutex);
#endif
	return false;
}

bool myprocess::isCompleted()
{
#if (defined __linux__)
    if (pthread_mutex_trylock(printmutex) != 0)
    {
        return true;
    }
    pthread_mutex_unlock(printmutex);
    return false;
	#endif
#if (defined WIN32)
    if (TryEnterCriticalSection(&printmutex) == 0) return false;
    LeaveCriticalSection(&printmutex);
    return true;
#endif
}

void myprocess::allowprint()
{
#if (defined __linux__)
    pthread_mutex_lock(readymutex);
    pthread_mutex_unlock(printmutex);
#endif
#if (defined WIN32)
    EnterCriticalSection(&printmutex);
#endif
}

void myprocess::endprint()
{
	num--;
#if (defined __linux__)
    pthread_mutex_lock(breakmutex);
#endif
#if (defined WIN32)
    EnterCriticalSection(&breakmutex);
#endif
}

#if (defined __linux__)
int kbhit()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

void non_block(int state)
{
    struct termios ttystate;
	tcgetattr(STDIN_FILENO, &ttystate);
	if (state==NB_ENABLE)
	{
		ttystate.c_lflag &= ~ICANON;
		ttystate.c_lflag &= ~ECHO;
		ttystate.c_cc[VMIN] = 1;
	}
	else if (state==NB_DISABLE)
	{
		ttystate.c_lflag |= ICANON;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

char mygetch()
{
    char c;
    int i=kbhit();
	if (i)
	{
		c=fgetc(stdin);
		if (c=='q')
			i=1;
		else
			i=0;
	}
	else 
		c='z';
    return c;
}
#endif
#if (defined WIN32)
int mygetch()
{
    if (_kbhit())
        return _getch();
    else
        return -1;
}
#endif

#if (defined __linux__)
void * print_routine(void * n)
#endif
#if (defined WIN32)
DWORD WINAPI print_routine(void * n)
#endif
{
	myprocess * ptr = (myprocess*)n;
	int number = num;
	while (true)
	{
		if ( !(ptr->isCompleted()) )
		{
#if (defined WIN32)
			EnterCriticalSection(&readymutex);
#endif
			printf("thread %d\n",number);
            ptr->printcompleted();
		}
        else
			if (ptr->isBreak())
				break;
		}
	return 0;
}