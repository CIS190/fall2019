#include <iostream>
#include <stdexcept>

using namespace std;

int main()
{
    try
    {
        throw 1;
        // string {"abc"}.substr(10);
        // auto i = new int [1000000000000];
        // throw "error message?";
    }
    catch (const int i)
    {
        cerr << i;
    }
    catch (const out_of_range & e)
    {
        cerr << "out_of_range: " << e.what();
    }
    catch (const exception e)
    {
        cerr << "exception: " << e.what();
    }
    catch (...)
    {
        cerr << "other\n";
        throw;
    }
}
