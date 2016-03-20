#include "lab1l.h"

int runprocess()
{
#if (defined WIN32)
	int a,b,c,rez;
	cout<<"a*(b+c)\n";
	cout<<"a=";
	cin>>a;
	cout<<"b=";
	cin>>b;
	cout<<"c=";
	cin>>c;

	FILE *f;
	f=fopen("f1","w+");
	fclose(f);
	ofstream out;
	out.open("f1");
	out.put(char(b));
	out.put(char(c));
	out.close();

    STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcInfo;
	TCHAR CommandLine[] = TEXT("process");
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);
    ZeroMemory(&ProcInfo, sizeof(ProcInfo));
    Sleep(5000);
    if(!CreateProcess(NULL,CommandLine,NULL,NULL,FALSE,0,NULL,NULL,&StartupInfo,&ProcInfo))
	{
		printf( "CreateProcess failed.\n" );
	}
    WaitForSingleObject(ProcInfo.hProcess,INFINITE);
    
	ifstream in;
	char tmp;
	in.open("f1");
	in.get(tmp);
	in.close();
	remove("f1");
	rez=a*int(tmp);
    cout<<a<<"*"<<int(tmp)<<"="<<rez;
	
    CloseHandle(ProcInfo.hProcess);
    CloseHandle(ProcInfo.hThread);
#endif	

#if (defined __linux__)
	int a,b,c,rez;
	cout<<"a*(b+c)\n";
	cout<<"a=";
	cin>>a;
	cout<<"b=";
	cin>>b;
	cout<<"c=";
	cin>>c;

	FILE *f;
	f=fopen("f1","w+");
	fclose(f);
	ofstream out;
	out.open("f1");
	out.put(char(b));
	out.put(char(c));
	out.close();

	pid_t pid; 
	int status;
	switch(pid = fork()) {
	case -1:
		perror("fork");
		cout << "Error";
		exit(1);
	case 0:
		execl("process.out",NULL);
		exit(0);
	default:
		wait(&status);
	}
    
	ifstream in;
	char tmp;
	in.open("f1");
	in.get(tmp);
	in.close();
	remove("f1");
	rez=a*int(tmp);
    cout<<a<<"*"<<int(tmp)<<"="<<rez;
#endif	
	return rez;
}