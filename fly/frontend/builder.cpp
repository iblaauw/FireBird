#include "builder.h"
#include "frontendexception.h"
#include "hiddenerror.h"

using namespace firefly;

using Builder = frontend::Builder;
using Token = token::Token;

/*********** HELPERS **********************************/

inline IL::ExpressionPtr MakeError(const std::string& error)
{
    auto errorExpr = std::make_shared<IL::ErrorExpression>(error);
    return std::static_pointer_cast<IL::Expression>(errorExpr);
}

template <class T>
inline IL::TypedExpressionPtr AsTyped(std::shared_ptr<T>& val)
{
    return std::static_pointer_cast<IL::TypedExpression>(val);
}

template <class T>
inline IL::ExpressionPtr AsExpression(std::shared_ptr<T>& val)
{
    return std::static_pointer_cast<IL::Expression>(val);
}

template <class T>
inline IL::ExpressionPtr AsExpression(std::shared_ptr<T>&& val)
{
    return std::static_pointer_cast<IL::Expression>(std::forward<std::shared_ptr<T>>(val));
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

bool IsConstant(const token::Token& token)
{
    if (token.type != token::UNKNOWN)
        return false;

    StringView val = token.val;
    if (val.Size() == 0)
        return false;

    char c = val[0];
    return IsNumChar(c);
}

inline bool IsTopLevel(const IL::ExpressionPtr& expression)
{
    auto t = expression->GetType();
    return t == IL::DECLARATION || t == IL::ASSIGNMENT || t == IL::FUNC_CALL || t == IL::ERROR;
}

/**************** Builder *************/

Builder::Builder(token::Tokenizer& tokenizer)
    : tokenizer(tokenizer), functionSet()
{
    //currentContext = IL::Context::GetTopLevel();
    //InitBuiltins();
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
        IL::FunctionExpressionPtr func = ParseFunction();
        functionSet.push_back(func);
        //currentContext->AddFunction(func);
    }
}

//void Builder::InitBuiltins()
//{
//    // Builtin types
//    IL::TypePtr intType = std::make_shared<IL::Type>("int");
//    IL::TypePtr voidType = std::make_shared<IL::Type>("void");
//
//    IL::ContextPtr context = IL::Context::GetTopLevel();
//    context->AddType(intType);
//    context->AddType(voidType);
//}

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

/************* Parse Function ********************/

IL::FunctionExpressionPtr Builder::ParseFunction()
{
    IL::FunctionExpressionPtr func = ParseFunctionSig();
    if (func->GetType() == IL::ERROR)
        return func;

    ParseFunctionArgs(func);
    ParseExpressionSet(func->expressions);

    return func;
}

IL::FunctionExpressionPtr Builder::ParseFunctionSig()
{
    Load(3);
    const Token& tok1 = tokenizer.At(0);
    const Token& tok2 = tokenizer.At(1);
    const Token& tok3 = tokenizer.At(2);

    IL::FunctionExpressionPtr error; //TODO: create a function error class
    if (tok1.type != token::UNKNOWN)
        return error;
    if (tok2.type != token::UNKNOWN)
        return error;
    if (tok3.type != token::PAREN_OPEN)
        return error;

    auto expr = std::make_shared<IL::FunctionExpression>();
    expr->returnType = tok1.val;
    expr->name = tok2.val;
    return expr;
}

void Builder::ParseFunctionArgs(IL::FunctionExpressionPtr func)
{
    Load(1);
    const Token& firstTok = tokenizer.Current();
    if (firstTok.type == token::PAREN_CLOSE)
    {
        tokenizer.Consume();
        return;
    }

    while (true)
    {
        Load(2);
        const Token& tok = tokenizer.Current();
        const Token& tok2 = tokenizer.At(1);

        auto declExpr = DoDeclarationExpression(tok, tok2, nullptr);
        func->args.push_back(declExpr);

        tokenizer.Consume();
        tokenizer.Consume();

        Load(1);
        const Token& tok3 = tokenizer.Current();
        if (tok3.type == token::PAREN_CLOSE)
        {
            tokenizer.Consume();
            break;
        }

        // If it's not a close paren, it must be a comma
        if (tok3.type != token::COMMA)
        {
            // TODO: emit error
            tokenizer.Consume();
            break;
        }

        // Eat comma
        tokenizer.Consume();
    }
}

void Builder::ParseExpressionSet(std::vector<IL::ExpressionPtr>& expressions)
{
    expressions.clear();

    Load(1);
    const Token& starttok = tokenizer.Current();
    if (starttok.type != token::BRACKET_OPEN)
    {
        expressions.push_back(MakeError("Unexpected token. Expected '{'."));
        return;
    }

    // Eat '{'
    tokenizer.Consume();

    while (true)
    {
        Load(1);
        auto& tok = tokenizer.Current();
        if (tok.type == token::BRACKET_CLOSE)
        {
            tokenizer.Consume();
            break;
        }

        IL::ExpressionPtr expr = ParseTopExpression();
        expressions.push_back(expr);
    }
}

/*************** OLD ************/


//IL::TypePtr Builder::ParseType()
//{
//    Load(1);
//    const Token& tok = tokenizer.Current();
//    if (tok.type != token::UNKNOWN)
//        throw FrontEndException("Expected type", tok);
//
//    StringView typeName = tok.val;
//
//    IL::TypePtr type;
//    bool success = currentContext->TryGetType(typeName, &type);
//    if (!success)
//        throw FrontEndException("Unknown type.", tok);
//
//    tokenizer.Consume();
//
//    return type;
//}
//
//StringView Builder::ParseIdentifier()
//{
//    Load(1);
//    const Token& tok = tokenizer.Current();
//    if (tok.type != token::UNKNOWN)
//        throw FrontEndException("Expected type", tok);
//
//    tokenizer.Consume();
//
//    return tok.val;
//}
//
//void Builder::ParseSpecific(token::TokenType type)
//{
//    Load(1);
//    const Token& tok = tokenizer.Current();
//    if (tok.type != type)
//        throw FrontEndException("Expected ''", tok);
//
//    tokenizer.Consume();
//}
//
//IL::FunctionPtr Builder::ParseFunction()
//{
//    IL::ContextPtr funcContext = currentContext->CreateChild();
//    IL::FunctionSignaturePtr sig = ParseFuncSig(funcContext);
//
//    IL::FunctionPtr func = std::make_shared<IL::Function>(sig, funcContext);
//    func->functionContext = funcContext;
//    func->signature = sig;
//
//    ParseSpecific(token::BRACKET_OPEN);
//    IL::ContextPtr oldContext = currentContext;
//    currentContext = funcContext;
//
//    ParseFunctionContext(func);
//
//    currentContext = oldContext;
//    ParseSpecific(token::BRACKET_CLOSE);
//
//    return func;
//}
//
//IL::FunctionSignaturePtr Builder::ParseFuncSig(IL::ContextPtr funcContext)
//{
//    IL::TypePtr returntype = ParseType();
//    StringView name = ParseIdentifier();
//    if (currentContext->IsNameUsed(name))
//        throw FrontEndException("The name '" + static_cast<std::string>(name) + "' is already being used.");
//
//    auto sig = std::make_shared<IL::FunctionSignature>();
//    sig->name = name;
//    sig->returnType = returntype;
//
//    ParseSpecific(token::PAREN_OPEN);
//    ParseFunctionArgs(sig, funcContext);
//    ParseSpecific(token::PAREN_CLOSE);
//
//    return sig;
//}
//
//void Builder::ParseFunctionArgs(IL::FunctionSignaturePtr sig, IL::ContextPtr funcContext)
//{
//    Load(1);
//    const Token& tok = tokenizer.Current();
//    if (tok.type == token::PAREN_CLOSE)
//        return;
//
//    while (true)
//    {
//        IL::VariablePtr variable = std::make_shared<IL::Variable>();
//        variable->type = ParseType();
//        variable->name = ParseIdentifier();
//
//        funcContext->AddVariable(variable);
//        sig->argumentTypes.push_back(variable->type);
//
//        Load(1);
//        const Token& nextTok = tokenizer.Current();
//        if (nextTok.type == token::PAREN_CLOSE)
//            return;
//
//        ParseSpecific(token::COMMA);
//    }
//}
//
//void Builder::ParseFunctionContext(IL::FunctionPtr func)
//{
//    // Once we are inside a function, parse a full context
//    // worth of stuff
//
//    while (true)
//    {
//        Load(1);
//        const token::Token& tok = tokenizer.Current();
//        if (tok.type == token::BRACKET_CLOSE)
//            return;
//
//        // ParseExpression will return a ErrorExpression if something fails
//        IL::ExpressionPtr expr = ParseExpression();
//        func->AddExpression(expr);
//    }
//}



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
            return AsExpression( DoOperandExpression(IL::PLUS, lhs) );
        case token::MINUS:
            return AsExpression( DoOperandExpression(IL::MINUS, lhs) );
        case token::STAR:
            return AsExpression( DoOperandExpression(IL::TIMES, lhs) );
        case token::SLASH:
            return AsExpression( DoOperandExpression(IL::DIVIDES, lhs) );
        case token::EQUALS:
            return AsExpression( DoAssignmentExpression(lhs) );
        case token::UNKNOWN:
        {
            if (IsConstant(tok))
                return DoConstantExpression(tok, lhs);
            Load(2);
            const token::Token& tok2 = tokenizer.At(1);
            if (tok2.type == token::UNKNOWN)
                return DoDeclarationExpression(tok, tok2, lhs);
            return DoVariableExpression(tok, lhs);
        }
        default:
            return MakeError("FATAL: unknown token.");
    }
    return MakeError("FATAL: should never be reached");
}

IL::OperandExpressionPtr Builder::DoOperandExpression(IL::OperandType optype, IL::ExpressionPtr lhs)
{
    tokenizer.Consume();

    if (!IsTypedExpression(lhs))
        return IL::OperandExpression::Error("Unexpexted token '+'.");

    IL::ExpressionPtr rhs = ParseSubExpression(nullptr);
    if (!IsTypedExpression(rhs))
        return IL::OperandExpression::Error("Invalid second argument for '+'.");

    IL::TypedExpressionPtr typedL = AsTyped(lhs);
    IL::TypedExpressionPtr typedR = AsTyped(rhs);

    auto expr = std::make_shared<IL::OperandExpression>();
    expr->type = optype;
    expr->left = typedL;
    expr->right = typedR;

    return expr;
}

IL::AssignmentExpressionPtr Builder::DoAssignmentExpression(IL::ExpressionPtr lhs)
{
    tokenizer.Consume();

    if (lhs == nullptr)
        return IL::CreateError<IL::AssignmentExpression>("Unexpected token '='");

    if (lhs->GetType() != IL::VARIABLE)
        return IL::CreateError<IL::AssignmentExpression>("Invalid assignment, can only assign to a variable");
    auto variable = std::static_pointer_cast<IL::VariableExpression>(lhs);

    auto rhs = ParseSubExpression(nullptr);
    if (!IsTypedExpression(rhs))
        return IL::CreateError<IL::AssignmentExpression>("Invalid assignment value.");

    auto assignment = std::make_shared<IL::AssignmentExpression>();
    assignment->variable = variable;
    assignment->value = AsTyped(rhs);

    return assignment;
}

IL::DeclarationExpressionPtr Builder::DoDeclarationExpression(const token::Token& typeTok, const token::Token& varTok, IL::ExpressionPtr lhs)
{
    tokenizer.Consume();
    tokenizer.Consume();

    if (lhs != nullptr)
        return IL::CreateError<IL::DeclarationExpression>("Unexpected variable");

    auto expr = std::make_shared<IL::DeclarationExpression>();

    expr->type = typeTok.val;
    expr->name = varTok.val;

    return expr;
}

IL::VariableExpressionPtr Builder::DoVariableExpression(const token::Token& varTok, IL::ExpressionPtr lhs)
{
    tokenizer.Consume();
    if (lhs != nullptr)
        return IL::CreateError<IL::VariableExpression>("Unexpected variable");

    auto expr = std::make_shared<IL::VariableExpression>();
    expr->variable = varTok.val;
    return expr;
}

IL::ConstantExpressionPtr Builder::DoConstantExpression(const token::Token& cTok, IL::ExpressionPtr lhs)
{
    if (lhs != nullptr)
        return IL::CreateError<IL::ConstantExpression>("Unexpected token '<TODO>'");

    const StringView& raw = cTok.val;
    for (int i = 0; i < raw.Size(); i++)
    {
        if (!IsNumChar(i))
            return IL::CreateError<IL::ConstantExpression>("Invalid token. Variables can't start with numerals.");
    }

    auto expr = std::make_shared<IL::ConstantExpression>();
    expr->constant = raw;

    return expr;
}


