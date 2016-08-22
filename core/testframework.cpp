#include "testframework.h"

#include <iostream>

/*static*/ TestFramework* TestFramework::instance = nullptr;

/*static*/ void TestFramework::Activate(std::ifstream* inFile)
{
    instance = new TestFramework();
    instance->inFile = inFile;
}

void TestFramework::LogOutput(const std::string& output)
{
    if (shutoff)
        return;

    std::string correct = GetNextOutput();

    if (output == correct)
        return;

    std::cout << "Test failure: incorrect output '" << output << "'. Expected output '" << correct << "'." << std::endl;
    success = false;
}

bool TestFramework::Validate() const
{
    if (!success)
    {
        return false;
    }

    std::string line;
    bool moreOutputExpected = (bool)std::getline(*inFile, line);

    if (moreOutputExpected)
    {
        std::cout << "Test failure: expected more output than was produced" << std::endl;
        return false;
    }

    return true;
}

TestFramework::TestFramework() :
    success(true),
    shutoff(false),
    inFile()
{}

std::string TestFramework::GetNextOutput()
{
    std::string line;
    bool success = (bool)std::getline(*inFile, line);
    if (!success)
    {
        std::cout << "Test failure: no more output is expected" << std::endl;
        shutoff = true;
        return "";
    }
    return line;
}

