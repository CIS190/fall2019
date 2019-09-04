#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void printLogs(ostream & os)
{
	os << "something happened\n";
}

int main()
{
	ofstream ofs {"test.log"};
	ostringstream oss {"Log file:\n"};

	if (true)
		printLogs(cout);
	else if (true)
		printLogs(ofs);
	else
		printLogs(oss);
}
