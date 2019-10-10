#include <iostream>
#include <array>

using namespace std;

int main()
{
    int bad[] {1, 2, 3};

    // C-style arrays are prone to subtle bugs like this
    for (int i = 0; i <= 3; ++i)
        cout << bad[i] << "\n";

    // In C++17 the type and size can be deduced if the array is initialized
    array<int, 3> {1, 2, 3};
    for (unsigned int i = 0; i < arr.size(); ++i)
        cout << arr[i] << "\n";

    // Bound-checked access is also available
    for (unsigned int i = 0; i <= 3; ++i)
        cout << arr.at(i) << "\n";

    // We can also access elements like this, since the size is fixed.
    // This check is at _compile_ time!
    cout << get<0>(arr) << "\n";

    // cout << get<3>(arr) << "\n"; // Won't compile
}
