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

    FileCompiler compiler(inname, outname);
    bool success = compiler.Run();

    if (!success)
        return -1;

    return 0;
}
