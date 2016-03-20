#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	int rez;
	char b,c;
	ifstream in;
	in.open("f1");
	in.get(b);
	in.get(c);
	in.close();

	FILE *f;
	f=fopen("f1","w+");
	fclose(f);

	rez=int(b)+int(c);
	cout<<"(b+c)\n";
	cout<<int(b)<<"+"<<int(c)<<"="<<rez<<"\n";

	ofstream out;
	out.open("f1");
	out.put(char(rez));
	out.close();
	return 0;
}