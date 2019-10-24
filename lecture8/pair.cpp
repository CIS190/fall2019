#include <iostream>
#include <utility>

using namespace std;

int main()
{
    pair<int, string> p1 {1, "hello"};
    // auto p2 {1, "hello"}; // doesn't work, could be list, vector, etc.
    auto p2 {make_pair(1, "hello")};

    cout << p1.first << " " << p2.second;
}
