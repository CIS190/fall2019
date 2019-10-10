#include <iostream>
#include <list>
#include <vector>

using namespace std;

int main()
{
    list<int> l {1, 2, 3, 4};
    vector<int> v {1, 2, 3, 4};

    {
        auto it = l.begin();
        it++;
        l.insert(it, 0); // inserts before the iterator
        cout << "it: " << *it << "\n";

        for (int i : l)
            cout << i << "\n";
    }

    {
        auto it2 = v.begin() + 2;
        cout << "it2: " << *it2 << "\n";

        auto it = v.begin();
        it++;
        v.insert(it, 0); // if the vector size changes, all iterators are invalidated.
        cout << "it: " << *it << "\n";

        // iterators that are after the modified element are always invalidated for vectors.
        cout << "it2: " << *it2 << "\n";

        for (int i : v)
            cout << i << "\n";
    }
}
