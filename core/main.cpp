#include "common.h"
#include "opvalue.h"
#include "memory.h"
#include "OpWrapper.h"
#include "processor.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        cout << "Error: please specify a file" << endl;
        return -1;
    }

    char* filename = argv[1];
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

    return 0;
}
