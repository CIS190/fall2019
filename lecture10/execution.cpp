#include <execution>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> v;
    for (int i = 0; i < 1e2; ++i)
    {
        v.emplace_back(i);
    }

    auto f = [](auto & i)
             {
                 cerr << i << " ";
             };

    for_each(v.begin(), v.end(), f);

    cin.get();
    cerr << "\n";


    for_each(execution::par, v.begin(), v.end(), f);

    cin.get();
    cerr << "\n";

    for_each(execution::par_unseq, v.begin(), v.end(), f);

    // parallelization can help a lot for sort (increase the vector size)
    // sort(execution::par, v.begin(), v.end());
}
