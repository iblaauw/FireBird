#pragma once

#include "opparser.h"

#include <fstream>

class FileCompiler
{
private:
    const char* inname;
    const char* outname;

public:
    FileCompiler(const char* inname, const char* outname) : inname(inname), outname(outname)
    {}

    bool Run();

private:
    bool CompileFile(std::ifstream& infile, OpParser& parser);
    void WriteToFile(std::ofstream& outfile, OpParser& parser);
};

