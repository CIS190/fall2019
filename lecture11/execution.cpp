#include <execution>
#include <algorithm>
#include <iostream>
#include <vector>
#include <mutex>

using namespace std;

int main()
{
    vector<double> v;
    for (int i = 0; i < 1e4; ++i)
    {
        v.emplace_back(i);
    }

    mutex m;

    int total = 0;
    auto f = [&](const auto i)
             {
                 scoped_lock lock {m};
                 total += i;
             };

    for_each(v.begin(), v.end(), f);

    cerr << total << "\n";

    total = 0;
    for_each(execution::par, v.begin(), v.end(), f);

    cerr << total << "\n";

    total = 0;
    for_each(execution::par_unseq, v.begin(), v.end(), f);

    cerr << total << "\n";
}
