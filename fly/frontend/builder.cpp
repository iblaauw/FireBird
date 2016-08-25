#include "builder.h"

using namespace firefly;

using Builder = frontend::Builder;
using Token = token::Token;


Builder::Builder(token::Tokenizer& tokenizer)
    : tokenizer(tokenizer)
{
    currentContext = IL::Context::GetTopLevel();
    InitBuiltins();
}

void Builder::Build()
{
    while (true)
    {
        if (tokenizer.Size() == 0)
        {
            bool success = tokenizer.Advance();
            if (!success && tokenizer.Size() == 0)
                return;
        }

        // Only functions are allowed at the top level at the moment
        IL::FunctionPtr func = ParseFunction();
        currentContext->AddFunction(func);
    }
}

void Builder::InitBuiltins()
{
    // Builtin types
    IL::TypePtr intType = std::make_shared<IL::Type>("int");
    IL::TypePtr voidType = std::make_shared<IL::Type>("void");

    IL::ContextPtr context = IL::Context::GetTopLevel();
    context->AddType(intType);
    context->AddType(voidType);
}

void Builder::Load(int amount)
{
    if (tokenizer.Size() >= amount)
        return;

    while (tokenizer.Size() < amount)
    {
        bool success = tokenizer.Advance();
        if (!success)
            throw FrontendException("Unexpected end of file");
    }
}

IL::TypePtr Builder::ParseType()
{
    Load(1);
    const Token& token = tokenizer.Current();
    if (first.type != token::UNKNOWN)
        throw FrontEndException("Expected type");

    StringView typeName = token.val;

    TypePtr type;
    bool success = currentContext->TryGetType(typeName, &type);
    if (!success)
        throw FrontEndException("Unknown type.");

    tokenizer.Consume();

    return type;
}

StringView Builder::ParseIdentifier()
{
    Load(1);
    const Token& token = tokenizer.Current();
    if (first.type != token::UNKNOWN)
        throw FrontEndException("Expected type");

    tokenizer.Consume();

    return token.val;
}

void Builder::ParseSpecific(token::TokenType type)
{
    Load(1);
    const Token& token = tokenizer.Current();
    if (first.type != type)
        throw FrontEndException("Expected ''");

    tokenizer.Consume();
}

IL::FunctionPtr Builder::ParseFunction()
{
    ContextPtr funcContext = currentContext->CreateChild();
    IL::FunctionSignaturePtr sig = ParseFuncSig(funcContext);

    IL::FunctionPtr func = std::make_shared<IL::Function>();
    func->functionContext = funcContext;
    func->signature = sig;

    ParseSpecific(token::BRACKET_OPEN);
    ContextPtr oldContext = currentContext;
    currentContext = funcContext;

    ParseFunctionContext(func);

    currentContext = oldContext;
    ParseSpecific(token::BRACKET_CLOSE);

    return func;
}

IL::FunctionSignaturePtr Builder::ParseFuncSig(ContextPtr funcContext)
{
    TypePtr returntype = ParseType();
    StringView name = ParseIdentifier();
    if (currentContext->IsNameUsed(name))
        throw FrontEndException("The name '" + static_cast<std::string>(name) + "' is already being used.");

    auto sig = std::make_shared<IL::FunctionSignature>();
    sig->name = name;
    sig->returnType = returntype;

    ParseSpecific(token::PAREN_OPEN);
    ParseFunctionArgs();
    ParseSpecific(token::PAREN_CLOSE);

    return sig;
}

void Builder::ParseFunctionArgs(FunctionSignaturePtr sig, ContextPtr funcContext)
{
    Load(1);
    const Token& tok = tokenizer.Current();
    if (tok.type == token::PAREN_CLOSE)
        return;

    while (true)
    {
        IL::VariablePtr variable = std::make_shared<IL::VariablePtr>();
        variable->type = ParseType();
        variable->name = ParseIdentifier();

        funcContext->AddVariable(variable);
        sig->argumentTypes.push_back(variable->type);

        Load(1);
        const Token& nextTok = tokenizer.Current();
        if (nextTok.type == token::PAREN_CLOSE)
            return;

        ParseSpecific(token::COMMA);
    }
}

void Builder::ParseFunctionContext(IL::FunctionPtr func)
{
    // Once we are inside a function, parse a full context
    // worth of stuff

    while (true)
    {
        Load(1);
        if (tok.type == toke::BRACKET_CLOSE)
            return;

        // ParseExpression will return a ErrorExpression if something fails
        IL::ExpressionPtr expr = ParseExpression();
        func->AddExpression(expr);
    }
}

