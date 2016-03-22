#if (defined __linux__)
#include <sys/time.h>
#include <unistd.h>
#endif

#if (defined WIN32)
#include <windows.h>
#endif

#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#if (defined __linux__)
unsigned long GetTickCount()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return (tv.tv_sec*1000 + tv.tv_usec/1000);
}
#endif

int main()
{
	char charNumber;
	ifstream in;
	in.open("f1");
	in.get(charNumber);
	in.close();

	const int number = (int)charNumber;
	
	while(1)
	{
		char tmpNumber;
		int time;
		time = (GetTickCount()/100)%10+1;
		ifstream in;
		in.open("f1");
		in.get(tmpNumber);
		in.close();

		if (number > (int)tmpNumber)
		{
			return 0;
		}
		if (number == time)
		{
			cout<<"\nProcess "<<number;				
		}
		while (number == time)
		{
			time = (GetTickCount()/100)%10+1;
			#if (defined __linux__)
			usleep(100000);
			#endif

			#if (defined WIN32)
			Sleep(100);
			#endif
		}		
	}
	return 0;
}