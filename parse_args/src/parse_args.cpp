#include "parse_args.h"
#include <iostream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <sstream>

// Fixes the fact that my compiler has no to_string (known bug)
template<typename T>
static std::string to_string(const T& val)
{
    std::ostringstream ss;
    ss << val;
    return ss.str();
}

// Helper map Add function
template <typename KeyType, typename ValType>
static void Add(std::map<KeyType, ValType>& selfMap, KeyType& key, ValType& val)
{
    auto iter = selfMap.find(key);
    if (iter != selfMap.end())
        throw ParseException("Option already exists: " + to_string(key));

    selfMap.insert(iter, typename std::map<KeyType,ValType>::value_type(key, val));
}

ParseSpec::ParseSpec(std::string spec) :
    shorts(), longs(), associations(), required()
{
    if (spec.size() == 0)
        return;

    // Parse the spec
    bool first = true;
    char current;
    bool required;
    bool takeArg;

    for (char c : spec)
    {
        if (isalnum(c))
        {
            if (first)
            {
                first = false;
            }
            else
            {
                // Commit the previous before continuing
                AddShort(current, takeArg, required);
            }

            current = c;
            required = true;
            takeArg = false;
        }
        else if (c == '?')
        {
            if (first)
                throw ParseException("Invalid Spec");
            required = false;
        }
        else if (c == '+')
        {
            if (first)
                throw ParseException("Invalid Spec");
            takeArg = true;
        }
        else
        {
            throw ParseException("Invalid Spec");
        }
    }

    AddShort(current, takeArg, required);
}

void ParseSpec::AddShort(char val, bool takesArg, bool isRequired)
{
    Add(shorts, val, takesArg);
    if (isRequired)
        required.push_back(val);
}

void ParseSpec::AddLong(std::string val, bool takesArg)
{
    Add(longs, val, takesArg);
}

void ParseSpec::Associate(std::string longVal, char shortVal)
{
    Add(associations, longVal, shortVal);
}

void ParseSpec::PrintHelpText()
{
    std::cout << "Invalid Usage" << std::endl;
}

void ParseSpec::TryGet(char shortVal, bool* exists, bool* takeArg)
{
    assert(exists != nullptr);
    assert(takeArg != nullptr);

    auto iter = shorts.find(shortVal);
    if (iter == shorts.end())
    {
        *exists = false;
        return;
    }

    *exists = true;
    *takeArg = iter->second;
}

void ParseSpec::TryGet(std::string longVal, bool* exists, bool* takeArg)
{
    assert(exists != nullptr);
    assert(takeArg != nullptr);

    auto iter = longs.find(longVal);
    if (iter == longs.end())
    {
        *exists = false;
        return;
    }

    *exists = true;
    *takeArg = iter->second;
}

ParseArgs::ParseArgs(ParseSpec* spec) :
    spec(spec),
    expectingArg(false),
    isLong(false),
    current(),
    currentLong(),
    shortArgs(),
    longArgs(),
    unmarked()
{}


/*static*/ ParseArgs ParseArgs::Parse(int argc, char** argv, ParseSpec& spec)
{
    ParseArgs args(&spec);
    for (int i = 1; i < argc; i++)
    {
        char* val = argv[i];
        args.HandleArg(val);
    }

    args.Validate();

    return args;
}

void ParseArgs::HandleArg(char* val)
{
    int length = strlen(val);
    if (length == 0)
        throw ParseException("Received 0 length argument");

    if (val[0] != '-')
    {
        HandleUnmarked(val);
        return;
    }

    if (length == 1)
    {
        std::cout << "Invalid argument value: -" << std::endl;
        spec->PrintHelpText();
        exit(-1);
        return;
    }

    if (val[1] == '-')
    {
        HandleLong(val+2);
        return;
    }

    HandleShort(val+1);
}

void ParseArgs::HandleUnmarked(char* val)
{
    if (!expectingArg)
    {
        unmarked.push_back(val);
    }
    else
    {
        if (isLong)
        {
            longArgs[currentLong] = val;
        }
        else
        {
            shortArgs[current] = val;
        }
        expectingArg = false;
    }
}

void ParseArgs::HandleShort(char* val)
{
    int len = strlen(val);
    assert(len > 0);

    for (int i = 0; i < len; i++)
    {
        ExpectingArgGuard();

        char c = val[i];
        bool exists;

        spec->TryGet(c, &exists, &expectingArg);
        if (!exists)
        {
            std::cout << "Unrecognized option: -" << c << std::endl;
            spec->PrintHelpText();
            exit(-1);
        }

        //TODO: support calling the same switch multiple times (think compile with -I)
        shortArgs[c] = nullptr;
        current = c;
        isLong = false;
    }
}

void ParseArgs::HandleLong(char* val)
{
    //TODO: support --arg=value
    std::string arg(val);

    assert(arg.size() > 0);

    ExpectingArgGuard();

    bool exists;
    spec->TryGet(arg, &exists, &expectingArg);
    if (!exists)
    {
        std::cout << "Unrecognized option: --" << val << std::endl;
        spec->PrintHelpText();
        exit(-1);
    }

    longArgs[arg] = nullptr;
    currentLong = arg;
    isLong = true;
}

void ParseArgs::ExpectingArgGuard()
{
    if (!expectingArg)
        return;

    if (isLong)
    {
        std::cout << "No argument given for option --" << currentLong << std::endl;
    }
    else
    {
        std::cout << "No argument given for option -" << current << std::endl;
    }

    spec->PrintHelpText();
    exit(-1);
}

void ParseArgs::Validate()
{
    ExpectingArgGuard();

    for (char c : spec->required)
    {
        if (shortArgs.find(c) == shortArgs.end())
        {
            std::cout << "Required option was not given: -" << c << std::endl;
            spec->PrintHelpText();
            exit(-1);
        }
    }
}

bool ParseArgs::IsSet(char name)
{
    return shortArgs.find(name) != shortArgs.end();
}

bool ParseArgs::IsSet(std::string name)
{
    bool islong = longArgs.find(name) != longArgs.end();

    if (islong)
        return true;

    if (name.size() == 1)
        return IsSet(name[0]);

    return false;
}

char* ParseArgs::Get(char name)
{
    auto iter = shortArgs.find(name);
    if (iter != shortArgs.end())
        return iter->second;
    return nullptr;
}

char* ParseArgs::Get(std::string name)
{
    auto iter = longArgs.find(name);
    if (iter != longArgs.end())
        return iter->second;

    if (name.size() == 1)
        return Get(name[0]);

    return nullptr;
}






