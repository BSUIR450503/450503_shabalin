#include "lab2l.h"

#if (defined __linux__)
int getch()
{
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~ (ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
#endif

int start()
{
	ofstream out;
	out.open("f1");
	out.put(char(0));
	out.close();

	while(1)
	{
		int key;
		key=getch();
		char number;

		ifstream in;
		in.open("f1");
		in.get(number);
		in.close();
		#if (defined __linux__)
		switch (key)
		{
			case 'w': {
						number++;
						if (number == 11) break;
						cout<<"\n+";
						addprocess();//up

						ofstream out;
						out.open("f1");
						out.clear();
						out.put(number);
						out.close();
					 }
					 break;

			case 'q': {
						if (number == 0) break;
						cout<<"\n-";
						removeprocess();//down
					 }
					 break;

			case 'e': {
						endprocess();
						return 0;
					 }
					 break;
		}
		#endif

		#if (defined WIN32)
		switch (key)
		{
			case 72: {
						number++;
						if (number == 11) break;
						cout<<"\n+";
						addprocess();//up

						ofstream out;
						out.open("f1");
						out.clear();
						out.put(number);
						out.close();
					 }
					 break;

			case 80: {
						if (number == 0) break;
						cout<<"\n-";
						removeprocess();//down
					 }
					 break;

			case 27: {
						endprocess();
						return 0;
					 }
					 break;
		}
		#endif
	}
	return 0;
}

proc addprocess()
{
	#if (defined WIN32)
    STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcInfo;
	TCHAR CommandLine[] = TEXT("process");

	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);
    ZeroMemory(&ProcInfo, sizeof(ProcInfo));

    if(!CreateProcess(NULL,CommandLine,NULL,NULL,FALSE,0,NULL,NULL,&StartupInfo,&ProcInfo))
	{
		printf( "CreateProcess failed.\n" );
	}
    return ProcInfo.hProcess;
	//WaitForSingleObject(ProcInfo.hProcess,INFINITE);
	#endif

	#if (defined __linux__)
	pid_t pid; 
	int status;
	switch(pid = fork()) {
	case -1:
		perror("fork");
		cout << "Error";
		exit(1);
	case 0:
		execl("process.out",NULL);
		return pid;
	//default:
		//wait(&status);
	}
	
	#endif
}
void removeprocess()
{
	char number;

	ifstream in;
	in.open("f1");
	in.get(number);
	in.close();

	number--;

	ofstream out;
	out.open("f1");
	out.clear();
	out.put(number);
	out.close();
}
void endprocess()
{
	char number;

	ifstream in;
	in.open("f1");
	in.get(number);
	in.close();

	number = -1;

	ofstream out;
	out.open("f1");
	out.clear();
	out.put(number);
	out.close();
	#if (defined __linux__)
	usleep(100000);
	#endif

	#if (defined WIN32)
	Sleep(100);
	#endif

	return;
}