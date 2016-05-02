#if (defined WIN32)
#include <windows.h> 
#include <conio.h>
#endif
#if (defined __linux__)
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define SIZE_BUFF 512

struct Data
{
#if (defined WIN32)
	BOOL   fConnected; 
	DWORD  cbRead; 
	DWORD  cbWritten;

	HANDLE hSem;
	HANDLE hSemClient;
	HANDLE hNamedPipe; 

	PROCESS_INFORMATION prInfo;
	STARTUPINFO stInfo;
#endif
#if (defined __linux__)
	int pid; 
	int semid; 
	struct sembuf buf1, buf; 
	char *msg, *segptr;
	int shmid; 

	key_t key;
#endif
};

struct Data CreateApplication(char *, struct Data);
void CloseApplication(struct Data);

struct Data Server(struct Data);
struct Data Client(struct Data);

bool ReceiveDataChannel(char *, int, struct Data *);
bool SendDataChannel(char *, int, struct Data *);

void InitSettings(struct Data *);
struct Data SetExistsData(struct Data);

