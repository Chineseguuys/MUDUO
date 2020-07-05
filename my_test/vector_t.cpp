#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char* argv[], char* env[])
{
    vector<int> integer_arr {1,2,3,4,5};
    cout << integer_arr.size() << endl;

    integer_arr.resize(integer_arr.size() * 2);
    for (auto& it : integer_arr)
        cout << it << ", ";

    cout << endl;
    return 0;
}