#include "parse_args.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

int main(int argc, char** argv)
{
    ParseSpec argspec("o+");
    argspec.AddLong("version", false);
    ParseArgs args = ParseArgs::Parse(argc, argv, argspec);

    if (args.IsSet("version"))
    {
        cout << "FireEgg, v. 0.1.0" << endl;
        return 0;
    }

    char* filename = args.Get('o');
    cout << "Processing file " << filename << endl;


    return 0;
}
