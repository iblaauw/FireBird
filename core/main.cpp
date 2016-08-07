#include "common.h"
#include "opvalue.h"
#include "memory.h"
#include "OpWrapper.h"
#include "processor.h"
#include "parse_args.h"
#include "testframework.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

int main(int argc, char** argv)
{

    ParseSpec spec("t+?");
    spec.AddLong("version", false);

    ParseArgs args = ParseArgs::Parse(argc, argv, spec);

    if (args.IsSet("version"))
    {
        cout << "FireBird, version 0.1.0" << endl;
        return 0;
    }

    auto unmarked = args.GetUnmarked();
    if (unmarked.size() != 1)
    {
        cout << "Error: wrong number of arguments" << endl;
        spec.PrintHelpText();
        return -1;
    }

    if (args.IsSet('t'))
    {
        char* testFilename = args.Get('t');
        ifstream* testFile = new ifstream();
        testFile->open(testFilename, ios::in);
        if (!(*testFile))
        {
            cout << "Error: could not open file " << testFilename << endl;
            cout.flush();
            return -1;
        }

        TestFramework::Activate(testFile);
    }


    char* filename = unmarked[0];
    ifstream inFile;
    inFile.open(filename, ios::in | ios::binary);

    if (!inFile)
    {
        cout << "Error: could not open file " << filename << endl;
        cout.flush();
        return -1;
    }

    // Set up FireBird memory
    Memory memory;

    // Load file into memory
    memory.LoadFile(inFile);
    inFile.close();

    Processor processor(&memory);

    try
    {
        processor.Start(0);
    }
    catch (exception& ex)
    {
        cout << "FATAL: an error occurred." << endl;
        cout << ex.what() << endl;
        //TODO: figure out how to print backtrace
        return -1;
    }

    TestFramework* testing = TestFramework::GetInstance();
    if (testing)
    {
        if (!testing->Validate())
        {
            cout << "Test FAILED" << endl;
            return -1;
        }

        cout << "Test SUCCESS" << endl;
    }

    return 0;
}
