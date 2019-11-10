#include <iostream>
#include <vector>
#include <functional>

using namespace std;

int main()
{
    vector<int> v {1, 2, 3, 4};

    using namespace placeholders;

    replace_if(v.begin(), v.end(), bind(less<int>(), _1, 3), 3);

    for (auto & i : v)
        cout << i << " ";
}
