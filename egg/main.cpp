#include <iostream>
#include <ifstream>
#include <vector>
#include <map>

using namespace std;

struct program_args
{
    std::vector<int> unflagged;
    std::vector<
};

static void parse_args(int argc, char** argv, program_args& args)
{
    std::vector<int>
}

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        cout << "Error: please specify a file" << endl;
    }

    char* filename = argv[1];

    cout << 

    return 0;
}
