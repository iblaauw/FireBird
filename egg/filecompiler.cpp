#include "filecompiler.h"
#include "opparser.h"
#include <iostream>


void FileCompiler::Run(std::ifstream& infile, const std::string& inname,
    std::ofstream& ofile, const std::string& outname)
{
    std::cout << "Compiling " << inname << " -> " << outname << std::endl;
    OpParser parser;

    int linenum = 1;
    std::string line;
    try
    {
        while (std::getline(infile, line))
        {
            parser.Parse(line);

            linenum++;
        }
    }
    catch (CompileException ex)
    {
        std::cout << ex.what() << std::endl;
        std::cout << "Error occured in file " << inname << " at line " << linenum << std::endl;
        std::cout << line << std::endl;
        std::cout << std::endl;
        std::cout << "Compiling Failed" << std::endl;
        return;
    }

    try
    {
        parser.Finalize(); // Check for second pass errors
    }
    catch (CompileException ex)
    {
        std::cout << ex.what() << std::endl;
        std::cout << "Error occured in file " << inname << std::endl;
        std::cout << std::endl;
        std::cout << "Compiling Failed" << std::endl;
        return;
    }

    size_t size = parser.ops.size() * sizeof(opvalue);
    const char* data = reinterpret_cast<const char*>(parser.ops.data());

    std::cout << "Data size to write: " << size << std::endl;

    ofile.seekp(std::ios_base::beg); // seek to beginning
    ofile.write(data, size);
    ofile.flush();


    std::cout << "Successfully compiled " << inname << " -> " << outname << std::endl;
}

