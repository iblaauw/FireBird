#pragma once

#include <vector>
#include <string>
#include <map>
#include <exception>

class ParseArgs;

// ParseSpec string format:
// A single letter represents the switch
// A '+' means it takes an argument
// A '?' means it is optional
class ParseSpec
{
private:
    typedef std::map<char, bool> ShortMap;
    typedef std::map<std::string, bool> LongMap;
    typedef std::map<std::string, char> AssociationMap;

    ShortMap shorts;
    LongMap longs;
    AssociationMap associations;
    std::vector<char> required;
public:
    ParseSpec(std::string spec);
    void AddShort(char val, bool takesArg, bool required);
    void AddLong(std::string val, bool takesArg);
    void Associate(std::string longVal, char shortVal);

    void PrintHelpText();

private:
    void TryGet(char shortVal, bool* exists, bool* takeArg);
    void TryGet(std::string longVal, bool* exists, bool* takeArg);

    friend class ParseArgs;
};

class ParseArgs
{
public:
    static ParseArgs Parse(int argc, char** argv, ParseSpec& spec);

    bool IsSet(char name);
    bool IsSet(std::string name);
    char* Get(char name);
    char* Get(std::string name);
    const std::vector<char*>& GetUnmarked() { return unmarked; }
private:
    ParseArgs(ParseSpec* spec);
    void HandleArg(char* val);
    void HandleUnmarked(char* val);
    void HandleShort(char* val);
    void HandleLong(char* val);
    void ExpectingArgGuard();
    void Validate();

    ParseSpec* spec;
    bool expectingArg;
    bool isLong;
    char current;
    std::string currentLong;

    std::map<char, char*> shortArgs;
    std::map<std::string, char*> longArgs;
    std::vector<char*> unmarked;
};

class ParseException : public std::exception
{
private:
    std::string message;
public:
    ParseException(const char* message) : message(message) {}
    ParseException(std::string message) : message(message) {}
    const char* what() const throw() override { return message.c_str(); }
};
