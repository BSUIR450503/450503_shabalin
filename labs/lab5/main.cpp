#include "lab5l.h"

#ifdef _WIN32
struct OperationInfo
{
	HANDLE hFile;            
	DWORD NumberOfBytes;	  
	CHAR  buf[100];			  
	DWORD  pos_out_file;	  
	OVERLAPPED Overlapped;
} info;

HINSTANCE library;
HANDLE events[3];
HANDLE readEnd;
HANDLE writeEnd;

DWORD WINAPI ReaderThread(PVOID folderPATH)
{           
	char* name[] = {"E:\\inf\\1.txt", "E:\\inf\\2.txt","E:\\inf\\3.txt"};
	BOOL(*fileRead)(OperationInfo*) = (BOOL(*)(OperationInfo*))GetProcAddress(library, "fileRead"); 
	//__declspec(dllimport) void fileRead(OperationInfo*);

	for (int i = 0; i < 3; i++)
	{
		info.hFile = CreateFile(name[i], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		printf("Reading from %s...\n", name[i]);
		fileRead(&info);

		SetEvent(readEnd);
		WaitForSingleObject(writeEnd, INFINITE);
	}
	return 0;
}

DWORD WINAPI WriterThread(PVOID outFilePath)
{
	string file = "E:\\inf\\out.txt";             
	HANDLE hOutputFile = CreateFile(file.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	BOOL(*fileWrite)(OperationInfo*, int) = (BOOL(*)(OperationInfo*, int))GetProcAddress(library, "fileWrite");
	//__declspec(dllimport) void fileWrite(OperationInfo*, int i);

	for (int i = 0; i < 3; i++)
	{
		WaitForSingleObject(readEnd, INFINITE);
		info.hFile = hOutputFile;
		printf("Writing to %s...\n", file.c_str());
		fileWrite(&info, i);

		SetEvent(writeEnd);
	}
	CloseHandle(hOutputFile);
	return 0;
}

 int main(int argc, char *argv[])
{
	HANDLE hEvent;         
	HANDLE reading;
	HANDLE writting;

	hEvent = CreateEvent(NULL, FALSE, TRUE, TEXT("event"));  
	readEnd = CreateEvent(NULL, FALSE, FALSE, NULL);
	writeEnd = CreateEvent(NULL, FALSE, FALSE, NULL); 

	info.Overlapped.Offset = 0;
	info.Overlapped.OffsetHigh = 0;
	info.Overlapped.hEvent = hEvent;
	info.pos_out_file = 0;
	info.NumberOfBytes = sizeof(info.buf);

	library = LoadLibrary("lab5cl.dll");

	reading = CreateThread(NULL, 0, ReaderThread, NULL, 0, NULL); 
	writting = CreateThread(NULL, 0, WriterThread, NULL, 0, NULL); 

	WaitForSingleObject(writting, INFINITE);

	CloseHandle(reading);
	CloseHandle(writting);
	CloseHandle(readEnd);
	CloseHandle(writeEnd);
	CloseHandle(hEvent);

	FreeLibrary(library);

	system("pause");
	return 0;
} 
 #endif

#ifdef __linux__
struct FileInfo 
{
	char readFileName[500];
	char writeFileName[500];
	struct aiocb readCb;
	struct aiocb writeCb;
	char buffer[300];
	int bytesRead;
	int bytesWrite;
} fileInfo;

pthread_t readThread;
pthread_t writeThread;
pthread_mutex_t mutex;
char *fileNames[] = {
						"./1.txt",
						"./2.txt",
						"./3.txt"
					};

void (*fileRead)(struct FileInfo *fileInfo);
void (*fileWrite)(struct FileInfo *fileInfo);

void *reading(void * arg)
{
	for(int i = 0; i < 3; i++) 
	{
		pthread_mutex_lock(&mutex);
		strcpy(fileInfo.readFileName, fileNames[i]);

		(*fileRead)(&fileInfo);

		pthread_mutex_unlock(&mutex);
		usleep(100000);
	}
	return NULL;
}

void *writting(void * arg) 
{
	usleep(10000);
	strcpy(fileInfo.writeFileName, "./out.txt");
	

	for(int i = 0; i < 3; i++)
	{
		pthread_mutex_lock(&mutex);

		(*fileWrite)(&fileInfo);

		pthread_mutex_unlock(&mutex);
		usleep(100000);
	}	
	return NULL;
}

int main()
{
	pthread_mutex_init(&mutex, NULL);

	void *library = dlopen("./lab5l.so", RTLD_LAZY);

	fileWrite = (void(*)(struct FileInfo *fileInfo)) dlsym(library, "fileWrite");
	fileRead = (void(*)(struct FileInfo *fileInfo)) dlsym(library, "fileRead");

	pthread_create(&readThread, NULL, reading, NULL);
	pthread_create(&writeThread, NULL, writting, NULL);

	pthread_join(readThread, NULL);
	pthread_join(writeThread, NULL);

	dlclose(library);
	return 0;
}
#endif