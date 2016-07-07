#include "parse_args.h"
#include "filecompiler.h"
#include <iostream>
#include <fstream>
#include <vector>

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

    char* outname = args.Get('o');

    const auto& unmarked = args.GetUnmarked();
    if (unmarked.size() == 0)
    {
        cout << "No input files specified!" << endl;
        argspec.PrintHelpText();
        return -1;
    }

    char* inname = unmarked[0];

    ifstream infile;
    infile.open(inname, ifstream::binary);
    if (!infile)
    {
        cout << "Error: Couldn't open file " << inname <<  endl;
        return -1;
    }

    ofstream outfile;
    outfile.open(outname, ofstream::trunc | ofstream::binary);
    if (!outfile)
    {
        cout << "Error: Couldn't open or create file " << outname <<  endl;
        return -1;
    }

    FileCompiler compiler;
    compiler.Run(infile, inname, outfile, outname);

    infile.close();
    outfile.close();

    return 0;
}
