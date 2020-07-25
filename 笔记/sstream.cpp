#include <string>
#include <iostream>
#include <fstream>
#include <exception>

using namespace std;

void read_data(istream& in)
{
    string str;
    while(in >> str)
    {
        cout << str << '\n';
    }
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

    infile.seekg(0, ios::end);
    int nrecord = infile.tellg();
    cout << "num record : " << nrecord << '\n';
    infile.seekg(0, ios::beg);

    read_data(infile);
    infile.close();
    return 0;
}