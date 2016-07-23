#include "filecompiler.h"

#include <iostream>


bool FileCompiler::Run()
{
    std::ifstream infile;
    infile.open(inname, std::ifstream::binary);
    if (!infile)
    {
        std::cout << "Error: Couldn't open file " << inname <<  std::endl;
        return false;
    }

    std::cout << "Compiling " << inname << " -> " << outname << std::endl;

    OpParser parser;

    bool success = CompileFile(infile, parser);
    if (!success)
        return false;

    infile.close();

    std::ofstream outfile;
    outfile.open(outname, std::ofstream::trunc | std::ofstream::binary);
    if (!outfile)
    {
        std::cout << "Error: Couldn't open or create file " << outname <<  std::endl;
        return false;
    }

    WriteToFile(outfile, parser);
    outfile.close();

    std::cout << "Successfully compiled " << inname << " -> " << outname << std::endl;

    return true;
}

bool FileCompiler::CompileFile(std::ifstream& infile, OpParser& parser)
{
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
        return false;
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
        return false;
    }

    return true;
}


void FileCompiler::WriteToFile(std::ofstream& outfile, OpParser& parser)
{
    size_t size = parser.ops.size() * sizeof(opvalue);
    const char* data = reinterpret_cast<const char*>(parser.ops.data());

    std::cout << "Data size to write: " << size << std::endl;

    outfile.seekp(std::ios_base::beg); // seek to beginning
    outfile.write(data, size);
    outfile.flush();
}
