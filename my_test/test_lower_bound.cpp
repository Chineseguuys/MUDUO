#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Comp
{
    bool operator() (const int& a, const int& b) const
    {
        return a > b;
    }

    bool equal(const int& a, const int& b) const 
    {
        return a == b;
    }
};



int main(int argc, char* argv[], char* env[])
{
    Comp comp;
    vector<int> vec {1,2,3,4,5,6,7,8};
    int value = 4;
    vector<int>::const_iterator it = lower_bound(vec.begin(), vec.end(), value, comp);
    cout << *it << endl;
    return 0;
}

