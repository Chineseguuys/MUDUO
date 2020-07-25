#include <string>
#include <iostream>
#include <fstream>
#include <exception>

using namespace std;

double read_data(istream& in)
{
    double heighest;
    double next;

    if (in >> next)
        heighest = next;
    else 
        return 0;
    try {
    while (in >> next)
        {
            cout << next << " ";
            if (next > heighest)
                heighest = next;
        }
    }
    catch (exception& exc)
    {
        cout << "Error happend" << exc.what() << '\n';
    }
    cout << "\n";
    return heighest;
}


int main()
{
    double max;

    string output;
    cin >> output;

    ifstream infile;
    infile.open(output.c_str());

    if (infile.fail()){
        cout << "Error opening " << output << "\n";
        return 1;
    }
    
    max = read_data(infile);
    infile.close();

    cout << "Max value = " << max << "\n";
    return 0;
}