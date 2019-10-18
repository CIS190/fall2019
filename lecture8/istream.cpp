#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

int main()
{
    istream_iterator<string> ii {cin};
    istream_iterator<string> eos; // end of stream iterator

    cout << *ii << "\n";
    ii++;
    cout << *ii << "\n";
    cout << *ii << "\n";

    ii++;
    vector<string> v;
    v.insert(v.begin(), ii, eos);

    for (auto & s : v)
        cout << s << " ";
}
