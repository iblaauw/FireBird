#include "builder.h"
#include "frontendexception.h"

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
        if (tokenizer.size() == 0)
        {
            bool success = tokenizer.Advance();
            if (!success && tokenizer.size() == 0)
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
    if (tokenizer.size() >= amount)
        return;

    while (tokenizer.size() < amount)
    {
        bool success = tokenizer.Advance();
        if (!success)
            throw FrontEndException("Unexpected end of file");
    }
}

IL::TypePtr Builder::ParseType()
{
    Load(1);
    const Token& tok = tokenizer.Current();
    if (tok.type != token::UNKNOWN)
        throw FrontEndException("Expected type", tok);

    StringView typeName = tok.val;

    IL::TypePtr type;
    bool success = currentContext->TryGetType(typeName, &type);
    if (!success)
        throw FrontEndException("Unknown type.", tok);

    tokenizer.Consume();

    return type;
}

StringView Builder::ParseIdentifier()
{
    Load(1);
    const Token& tok = tokenizer.Current();
    if (tok.type != token::UNKNOWN)
        throw FrontEndException("Expected type", tok);

    tokenizer.Consume();

    return tok.val;
}

void Builder::ParseSpecific(token::TokenType type)
{
    Load(1);
    const Token& tok = tokenizer.Current();
    if (tok.type != type)
        throw FrontEndException("Expected ''", tok);

    tokenizer.Consume();
}

IL::FunctionPtr Builder::ParseFunction()
{
    IL::ContextPtr funcContext = currentContext->CreateChild();
    IL::FunctionSignaturePtr sig = ParseFuncSig(funcContext);

    IL::FunctionPtr func = std::make_shared<IL::Function>(sig, funcContext);
    func->functionContext = funcContext;
    func->signature = sig;

    ParseSpecific(token::BRACKET_OPEN);
    IL::ContextPtr oldContext = currentContext;
    currentContext = funcContext;

    ParseFunctionContext(func);

    currentContext = oldContext;
    ParseSpecific(token::BRACKET_CLOSE);

    return func;
}

IL::FunctionSignaturePtr Builder::ParseFuncSig(IL::ContextPtr funcContext)
{
    IL::TypePtr returntype = ParseType();
    StringView name = ParseIdentifier();
    if (currentContext->IsNameUsed(name))
        throw FrontEndException("The name '" + static_cast<std::string>(name) + "' is already being used.");

    auto sig = std::make_shared<IL::FunctionSignature>();
    sig->name = name;
    sig->returnType = returntype;

    ParseSpecific(token::PAREN_OPEN);
    ParseFunctionArgs(sig, funcContext);
    ParseSpecific(token::PAREN_CLOSE);

    return sig;
}

void Builder::ParseFunctionArgs(IL::FunctionSignaturePtr sig, IL::ContextPtr funcContext)
{
    Load(1);
    const Token& tok = tokenizer.Current();
    if (tok.type == token::PAREN_CLOSE)
        return;

    while (true)
    {
        IL::VariablePtr variable = std::make_shared<IL::Variable>();
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
        const token::Token& tok = tokenizer.Current();
        if (tok.type == token::BRACKET_CLOSE)
            return;

        // ParseExpression will return a ErrorExpression if something fails
        IL::ExpressionPtr expr = ParseExpression();
        func->AddExpression(expr);
    }
}


/*********** HELPERS **********************************/

inline IL::ExpressionPointer MakeError(const std::string& error)
{
    auto errorExpr = std::make_shared<IL::ErrorExpression>(error);
    return std::static_pointer_cast<Expression>(errorExpr);
}

template <class T>
inline IL::TypedExpressionPointer AsTyped(std::shared_ptr<T>& val)
{
    return std::static_pointer_cast<TypedExpression>(val);
}

template <class T>
inline IL::ExpressionPointer AsExpression(std::shared_ptr<T>& val)
{
    return std::static_pointer_cast<Expression>(val);
}

bool IsTypedExpression(IL::ExpressionPtr expr)
{
    if (expr == nullptr)
        return false;

    auto type = expr->GetType();
    switch (type)
    {
    case IL::OPERAND:
    case IL::ASSIGNMENT:
    case IL::FUNC_CALL:
    case IL::VARIABLE:
    case IL::CONSTANT:
        return true;
    case IL::ERROR:
    case IL::DECLARATION:
    default:
        return false;
    }
}

bool IsNumChar(char c)
{
    return c >= '0' && c <= '9';
}

bool IsConstant(const IL::Token& token)
{
    if (token.type != IL::UNKNOWN)
        return false;

    if (val.Size() == 0)
        return false;

    char c = val[0];
    return IsNumChar(c);
}

bool IsTopLevel(const IL::Expression& expression)
{
    auto t = expr->GetType();
    return t == IL::DECLARATION || t == IL::ASSIGNMENT || t == IL::FUNC_CALL || t == IL::ERROR;
}

/********** PARSE EXPRESSION ***************************/

IL::ExpressionPtr Builder::ParseTopExpression()
{
    IL::ExpressionPtr expr = ParseSubExpression(nullptr);
    while (!IsTopLevel(expr))
    {
        expr = ParseSubExpression(expr);
    }

    return expr;
}

IL::ExpressionPtr Builder::ParseSubExpression(IL::ExpressionPtr lhs)
{
    Load(1);
    const Token& tok = tokenizer.Current();

    switch (tok.type)
    {
        case token::SEMICOLON:
            tokenizer.Consume();
            return lhs;
        case token::PLUS:
            return DoOperandExpression(IL::PLUS, lhs);
        case token::MINUS:
            return DoOperandExpression(IL::MINUS, lhs);
        case token::STAR:
            return DoOperandExpression(IL::TIMES, lhs);
        case token::SLASH:
            return DoOperandExpression(IL::DIVIDES, lhs);
        case token::EQUAL:
            return DoAssignmentExpression(lhs);
        case UNKNOWN:
        {
            if (IsConstant(tok))
                return DoConstantExpression(tok, lhs);
            Load(2);
            token::Token& tok2 = tokenizer.At(1);
            if (tok2.type == token::UNKNOWN)
                return DoDeclarationExpression(tok, tok2, lhs);
            return DoVariableExpression(tok, lhs);
        }
        default:
            return ErrorExpression("FATAL: unknown token.");
    }
    return ErrorExpression("FATAL: should never be reached");
}

IL::ExpressionPtr Builder::DoOperandExpression(IL::OperandType optype, IL::ExpressionPtr lhs)
{
    tokenizer.Consume();

    if (!IsTypedExpression(lhs))
        return ErrorExpression("Unexpexted token '+'.");

    IL::ExpressionPtr rhs = ParseSubExpression(nullptr);
    if (!IsTypedExpression(rhs))
        return ErrorExpression("Invalid second argument for '+'.");

    TypedExpressionPtr typedL = AsTyped(lhs);
    TypedExpressionPtr typedR = AsTyped(rhs);

    auto expr = std::make_shared<OperandExpression>();
    expr->type = optype;
    expr->left = typedL;
    expr->right = typedR;

    return AsExpression(expr);
}

IL::ExpressionPtr Builder::DoAssignmentExpression(IL::ExpressionPtr lhs)
{
    tokenizer.Consume();

    if (lhs == nullptr)
        return MakeError("Unexpected token '='");

    if (lhs->GetType() != IL::VARIABLE)
        return MakeError("Invalid assignment, can only assign to a variable");
    auto variable = std::static_pointer_cast<VariableExpression>(lhs);

    auto rhs = ParseSubExpression(nullptr);
    if (!IsTypedExpression(rhs))
        return MakeError("Invalid assignment value.");

    auto assignment = std::make_shared<AssignmentExpression>();
    assignment->variable = variable;
    assignment->value = AsTyped(rhs);

    return AsExpression(assignment);
}

IL::ExpressionPtr Builder::DoDeclarationExpression(token::Token& typeTok, token::Token& varTok, IL::ExpressionPtr lhs)
{
    tokenizer.Consume();
    tokenizer.Consume();

    if (lhs != nullptr)
        return MakeError("Unexpected variable");

    auto expr = std::make_shared<DeclarationExpression>();

    expr->type = typeToken.value;
    expr->name = varToken.value;

    return AsExpression(expr);
}

IL::ExpressionPtr Builder::DoVariableExpression(token::Token& varTok, IL::ExpressionPtr lhs)
{
    tokenizer.Consume();
    if (lhs != nullptr)
        return MakeError("Unexpected variable");

    auto expr = std::make_shared<VariableExpression>();
    expr->variable = varTok;
    return AsExpression(expr);
}

IL::ExpressionPtr Builder::DoConstantExpression(token::Token& cTok, IL::ExpressionPtr lhs)
{
    if (lhs != nullptr)
        return MakeError("Unexpected token '<TODO>'");

    StringView& raw = cTok.value;
    for (int i = 0; i < raw.Size(); i++)
    {
        if (!IsNumChar(i))
            return MakeError("Invalid token. Variables can't start with numerals.");
    }

    auto expr = std::make_shared<ConstantExpression>();
    expr->constant = raw;

    return AsExpression(expr);
}


