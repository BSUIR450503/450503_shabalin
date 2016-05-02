#include "lab4l.h"

int main(int argc, char *argv[])
{
#if (defined __linux__)
    non_block(NB_ENABLE);
#endif

    int count = 0;
    int current = 1;
    myprocess * ms[MAX_COUNT];
    int thread_numbers[MAX_COUNT];
    string param;
    param.reserve(MAX_COUNT);

    while (true)
	{
		char c = 'a';
        while(c!='q')
        {
			c = mygetch();
			switch(c)
			{
			case '+':
				if(count < MAX_COUNT)
				{
					count++;
					thread_numbers[count]=count;
					myprocess * t = new myprocess(thread_numbers[count]);
					ms[count-1]=t;
				};
				break;
			case '-':
				if(count > 0)
				{
					ms[count-1]->endprint();
					count--;
				};
				break;
			default:
				if(ms[current]->nextcanprint() && count>0)
				{
					if(current > count)
						current = 1;
				ms[current-1]->allowprint();
				current++;
				}
				break;
			}
			#if (defined WIN32)
            Sleep(100);
			#endif
		}
        for (int i=0;i<count;i++)
        {
			ms[i]->endprint();
		}
        exit(0);
	}
    return 0;
 }
