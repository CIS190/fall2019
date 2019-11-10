#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3};
    auto end = remove(v.begin(), v.end(), 1);

    for_each(v.begin(), end, [](auto & i)
                             {
                                 cout << i << " ";
                             });

    cout << "\n";
    for (auto & i : v)
        cout << i << " ";
}
