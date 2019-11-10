#include <iostream>
#include <vector>
#include <functional>
#include <numeric>

using namespace std;

int main()
{
    vector<int> v1 {1, 2, 3, 4};
    vector<int> v2;

    using namespace placeholders;

    transform(v1.begin(), v1.end(), back_inserter(v2), bind(plus<int>(), 1, _1));

    for (auto & i : v2)
        cout << i << " ";

    cout << "\n\n";
    cout << accumulate(v1.begin(), v1.end(), 0, plus<int>());
}
