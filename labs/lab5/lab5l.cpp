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
extern "C" {
__declspec(dllexport) void fileRead(OperationInfo *info) 
{
	info->Overlapped.Offset = 0;

	ReadFile(info->hFile, info->buf, info->NumberOfBytes, NULL, &info->Overlapped);
	WaitForSingleObject(info->Overlapped.hEvent, 1000);
}
__declspec(dllexport) void fileWrite(OperationInfo *info, int i)
{
	info->Overlapped.Offset = info->pos_out_file + i*3;

	WriteFile(info->hFile, info->buf, info->Overlapped.InternalHigh, NULL, &info->Overlapped);
	WaitForSingleObject(info->Overlapped.hEvent, 1000);
}
}
#endif

#ifdef __linux__
extern "C" {
#include <aio.h>
struct FileInfo 
{
	char readFileName[500];
	char writeFileName[500];
	struct aiocb readCb;
	struct aiocb writeCb;
	char buffer[300];
	int bytesRead;
	int bytesWrite;
};

void fileRead(struct FileInfo *fileInfo) 
{
	printf("\n\nReading from %s...\n",fileInfo->readFileName);

	int file = open(fileInfo->readFileName, O_RDONLY, 0);
	if (file == -1)
	{
		printf("Can't to open file for read!");
		return;
	}

	fileInfo->readCb.aio_nbytes = sizeof(fileInfo->buffer);
	fileInfo->readCb.aio_fildes = file;
	fileInfo->readCb.aio_offset = 0;
	fileInfo->readCb.aio_buf = fileInfo->buffer;
  
	aio_read(&fileInfo->readCb);
	while(aio_error(&fileInfo->readCb) == EINPROGRESS);
	fileInfo->bytesRead = aio_return(&fileInfo->readCb);
	close(file);
}
}
extern "C" {
void fileWrite(struct FileInfo *fileInfo) 
{

	int file = open(fileInfo->writeFileName, O_CREAT | O_RDWR | O_APPEND, 0666);
	if (file == -1) 
	{
		printf("Can't to open file for write!");
		return;
	}

	fileInfo->writeCb.aio_nbytes = fileInfo->bytesRead;
	fileInfo->writeCb.aio_fildes = file;
	fileInfo->writeCb.aio_buf = fileInfo->buffer;
  
	aio_write(&fileInfo->writeCb);
	while(aio_error(&fileInfo->writeCb) == EINPROGRESS);
	fileInfo->bytesWrite = aio_return(&fileInfo->writeCb);
	close(file);
}
}
#endif