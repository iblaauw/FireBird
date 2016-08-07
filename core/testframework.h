#pragma once

#include <string>
#include <fstream>

class TestFramework
{
private:
    static TestFramework* instance;

    bool success;
    bool shutoff; // turns true when EOF is reached
    std::ifstream* inFile;

public:
    static TestFramework* GetInstance() { return instance; }

    static void Activate(std::ifstream* inFile);
    void LogOutput(const std::string& output);
    bool Validate() const;

private:
    TestFramework();
    std::string GetNextOutput();
};

