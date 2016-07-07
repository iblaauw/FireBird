#pragma once

#include <fstream>

class FileCompiler
{
public:
void Run(std::ifstream& infile, const std::string& inname,
         std::ofstream& ofile, const std::string& outname);
};
