#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	const string filename {"test.txt"};

	ofstream ofs {filename};
	if (!ofs)
		cerr << "oh no";
	ofs << "test";

	ifstream ifs {filename};
	if (!ifs)
		cerr << "oh no again";

	if (string s; ifs >> s)
		cout << s;
	else
		cerr << "nothing to read?";
}
