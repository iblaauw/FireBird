#include "builder.h"
#include "frontendexception.h"
#include "hiddenerror.h"

#include <iostream>
#include <cassert>

using namespace firefly;

using Builder = frontend::Builder;
using Token = token::Token;

///*********** HELPERS **********************************/
//
//inline IL::ExpressionPtr MakeError(const std::string& error)
//{
//    return IL::CreateError<IL::Expression>(error);
//}
//
//template <class T>
//inline IL::TypedExpressionPtr AsTyped(std::shared_ptr<T>& val)
//{
//    return std::static_pointer_cast<IL::TypedExpression>(val);
//}
//
//template <class T>
//inline IL::ExpressionPtr AsExpression(std::shared_ptr<T>& val)
//{
//    return std::static_pointer_cast<IL::Expression>(val);
//}
//
//template <class T>
//inline IL::ExpressionPtr AsExpression(std::shared_ptr<T>&& val)
//{
//    return std::static_pointer_cast<IL::Expression>(std::forward<std::shared_ptr<T>>(val));
//}
//
//bool IsTypedExpression(IL::ExpressionPtr expr)
//{
//    if (expr == nullptr)
//        return false;
//
//    auto type = expr->GetType();
//    switch (type)
//    {
//    case IL::OPERAND:
//    case IL::ASSIGNMENT:
//    case IL::FUNC_CALL:
//    case IL::VARIABLE:
//    case IL::CONSTANT:
//    case IL::TYPE:
//        return true;
//    case IL::ERROR:
//    case IL::DECLARATION:
//    default:
//        return false;
//    }
//}
//
//bool IsNumChar(char c)
//{
//    return c >= '0' && c <= '9';
//}
//
//bool IsConstant(const token::Token& token)
//{
//    if (token.type != token::UNKNOWN)
//        return false;
//
//    StringView val = token.val;
//    if (val.Size() == 0)
//        return false;
//
//    char c = val[0];
//    return IsNumChar(c);
//}
//
//inline bool IsTopLevel(const IL::ExpressionPtr& expression)
//{
//    auto t = expression->GetType();
//    return t == IL::DECLARATION || t == IL::ASSIGNMENT || t == IL::FUNC_CALL || t == IL::ERROR;
//}
//
///**************** Builder *************/
//
//Builder::Builder(token::Tokenizer& tokenizer)
//    : tokenizer(tokenizer), functionSet()
//{
//    //currentContext = IL::Context::GetTopLevel();
//    //InitBuiltins();
//}
//
//void Builder::Build()
//{
//    while (true)
//    {
//        if (tokenizer.size() == 0)
//        {
//            bool success = tokenizer.Advance();
//            if (!success && tokenizer.size() == 0)
//                return;
//        }
//
//        std::cout << "Finding function..." << std::endl;
//
//        // Only functions are allowed at the top level at the moment
//        IL::FunctionExpressionPtr func = ParseFunction();
//        functionSet.push_back(func);
//        //currentContext->AddFunction(func);
//    }
//}
//
//void Builder::DebugPrint() const
//{
//    IL::TreePrinter printer;
//    printer.Print("=== Begin Tree Output ===");
//    printer.PrintLine();
//
//    for (auto& func : functionSet)
//    {
//        func->DebugPrint(printer);
//        printer.PrintLine();
//    }
//}
//
//void Builder::Load(int amount)
//{
//    if (tokenizer.size() >= amount)
//        return;
//
//    while (tokenizer.size() < amount)
//    {
//        bool success = tokenizer.Advance();
//        if (!success)
//            throw FrontEndException("Unexpected end of file");
//    }
//}
//
///************* Parse Function ********************/
//
//IL::FunctionExpressionPtr Builder::ParseFunction()
//{
//    std::cout << "Parsing Function" << std::endl;
//    IL::FunctionExpressionPtr func = ParseFunctionSig();
//    if (func->GetType() == IL::ERROR)
//        return func;
//
//    ParseFunctionArgs(func);
//    ParseExpressionSet(func->expressions);
//
//    return func;
//}
//
//IL::FunctionExpressionPtr Builder::ParseFunctionSig()
//{
//    Load(3);
//    const Token& tok1 = tokenizer.At(0);
//    const Token& tok2 = tokenizer.At(1);
//    const Token& tok3 = tokenizer.At(2);
//
//    tokenizer.Consume();
//    tokenizer.Consume();
//    tokenizer.Consume();
//
//    if (tok1.type != token::UNKNOWN)
//        return IL::CreateError<IL::FunctionExpression>("Unexpected token '" + tok1.ToString() + "', expected function definition.");
//    if (tok2.type != token::UNKNOWN)
//        return IL::CreateError<IL::FunctionExpression>("Unexpected token '" + tok2.ToString() + "', expected function definition.");
//    if (tok3.type != token::PAREN_OPEN)
//        return IL::CreateError<IL::FunctionExpression>("Unexpected token '" + tok3.ToString() + "', expected '(' for function definition.");
//
//    auto expr = std::make_shared<IL::FunctionExpression>();
//    expr->returnType = tok1.val;
//    expr->name = tok2.val;
//
//    return expr;
//}
//
//void Builder::ParseFunctionArgs(IL::FunctionExpressionPtr func)
//{
//    std::cout << "Parsing func args" << std::endl;
//
//    Load(1);
//    const Token& firstTok = tokenizer.Current();
//    if (firstTok.type == token::PAREN_CLOSE)
//    {
//        tokenizer.Consume();
//        return;
//    }
//
//    while (true)
//    {
//        Load(2);
//        const Token& tok = tokenizer.Current();
//        const Token& tok2 = tokenizer.At(1);
//
//        auto declExpr = DoDeclarationExpression(tok, tok2, nullptr); // DoDeclarationExpression consumes the 2 tokens
//        func->args.push_back(declExpr);
//
//        Load(1);
//        const Token& tok3 = tokenizer.Current();
//        if (tok3.type == token::PAREN_CLOSE)
//        {
//            tokenizer.Consume();
//            break;
//        }
//
//        // If it's not a close paren, it must be a comma
//        if (tok3.type != token::COMMA)
//        {
//            auto error = IL::CreateError<IL::DeclarationExpression>("Unexpected token '" + tok3.ToString() + "'. Expected '('.");
//            func->args.push_back(error);
//            tokenizer.Consume();
//            break;
//        }
//
//        // Eat comma
//        tokenizer.Consume();
//    }
//}
//
//void Builder::ParseExpressionSet(std::vector<IL::ExpressionPtr>& expressions)
//{
//    std::cout << "Parsing Function Expressions..." << std::endl;
//
//    expressions.clear();
//
//    Load(1);
//    const Token& starttok = tokenizer.Current();
//    if (starttok.type != token::BRACKET_OPEN)
//    {
//        expressions.push_back(MakeError("Unexpected token '" + starttok.ToString() + "'. Expected '{'."));
//        return;
//    }
//
//    // Eat '{'
//    tokenizer.Consume();
//
//    while (true)
//    {
//        Load(1);
//        auto& tok = tokenizer.Current();
//        if (tok.type == token::BRACKET_CLOSE)
//        {
//            tokenizer.Consume();
//            break;
//        }
//
//        IL::ExpressionPtr expr = ParseTopExpression();
//        expressions.push_back(expr);
//    }
//}
//
//
///********** PARSE EXPRESSION ***************************/
//
//IL::ExpressionPtr Builder::ParseTopExpression()
//{
//    std::cout << "Parsing Expression" << std::endl;
//
//    IL::ExpressionPtr expr = ParseSubExpression(nullptr);
//    while (true)
//    {
//        Load(1);
//        if (tokenizer.Current().type == token::SEMICOLON)
//        {
//            tokenizer.Consume();
//            break;
//        }
//
//        expr = ParseSubExpression(expr);
//    }
//
//    return expr;
//}
//
//IL::ExpressionPtr Builder::ParseSubExpression(IL::ExpressionPtr lhs)
//{
//    Load(1);
//    const Token& tok = tokenizer.Current();
//
//    switch (tok.type)
//    {
//        case token::SEMICOLON:
//            return MakeError("Unexpected token ';'");
//        case token::PLUS:
//            return AsExpression( DoOperandExpression(IL::PLUS, lhs) );
//        case token::MINUS:
//            return AsExpression( DoOperandExpression(IL::MINUS, lhs) );
//        case token::STAR:
//            return AsExpression( DoOperandExpression(IL::TIMES, lhs) );
//        case token::SLASH:
//            return AsExpression( DoOperandExpression(IL::DIVIDES, lhs) );
//        case token::EQUALS:
//            return AsExpression( DoAssignmentExpression(lhs) );
//        case token::UNKNOWN:
//        {
//            if (IsConstant(tok))
//                return DoConstantExpression(tok, lhs);
//            Load(2);
//            const token::Token& tok2 = tokenizer.At(1);
//            if (tok2.type == token::UNKNOWN)
//                return DoDeclarationExpression(tok, tok2, lhs);
//            return DoVariableExpression(tok, lhs);
//        }
//        default:
//            tokenizer.Consume();
//            return MakeError("Unexpected token '"+ tok.ToString() +"'");
//    }
//    return MakeError("FATAL: should never be reached");
//}
//
//IL::OperandExpressionPtr Builder::DoOperandExpression(IL::OperandType optype, IL::ExpressionPtr lhs)
//{
//    std::cout << "Found Operand" << std::endl;
//    tokenizer.Consume();
//
//    if (!IsTypedExpression(lhs))
//        return IL::CreateError<IL::OperandExpression>("Unexpected token '+'.");
//
//    IL::ExpressionPtr rhs = ParseSubExpression(nullptr);
//    if (rhs->GetType() == IL::ERROR)
//        return IL::RecastError<IL::OperandExpression>(rhs);
//
//    if (!IsTypedExpression(rhs))
//        return IL::CreateError<IL::OperandExpression>("Invalid second argument for '+'.");
//
//    IL::TypedExpressionPtr typedL = AsTyped(lhs);
//    IL::TypedExpressionPtr typedR = AsTyped(rhs);
//
//    auto expr = std::make_shared<IL::OperandExpression>();
//    expr->type = optype;
//    expr->left = typedL;
//    expr->right = typedR;
//
//    return contextManager.Process(expr);
//}
//
//IL::AssignmentExpressionPtr Builder::DoAssignmentExpression(IL::ExpressionPtr lhs)
//{
//    std::cout << "Found assignment" << std::endl;
//    tokenizer.Consume();
//
//    if (lhs == nullptr)
//        return IL::CreateError<IL::AssignmentExpression>("Unexpected token '='. Expected variable name.");
//
//    if (lhs->GetType() != IL::VARIABLE)
//        return IL::CreateError<IL::AssignmentExpression>("Invalid assignment, can only assign to a variable");
//    auto variable = std::static_pointer_cast<IL::VariableExpression>(lhs);
//
//    auto rhs = ParseSubExpression(nullptr);
//
//    if (rhs->GetType() == IL::ERROR)
//        return IL::RecastError<IL::AssignmentExpression>(rhs);
//
//    if (!IsTypedExpression(rhs))
//        return IL::CreateError<IL::AssignmentExpression>("Invalid assignment value.");
//
//    auto assignment = std::make_shared<IL::AssignmentExpression>();
//    assignment->variable = variable;
//    assignment->value = AsTyped(rhs);
//
//    return contextManager.Process(assignment);
//}
//
//IL::DeclarationExpressionPtr Builder::DoDeclarationExpression(const token::Token& typeTok, const token::Token& varTok, IL::ExpressionPtr lhs)
//{
//    std::cout << "Found Declaration" << std::endl;
//    tokenizer.Consume();
//    tokenizer.Consume();
//
//    if (lhs != nullptr)
//        return IL::CreateError<IL::DeclarationExpression>("Unexpected token '"+ typeTok.ToString() +"'. Expected ';'");
//
//    auto expr = std::make_shared<IL::DeclarationExpression>();
//
//    expr->type = typeTok.val;
//    expr->name = varTok.val;
//
//    return contextManager.Process(expr);
//}
//
//IL::VariableExpressionPtr Builder::DoVariableExpression(const token::Token& varTok, IL::ExpressionPtr lhs)
//{
//    std::cout << "Found Variable" << std::endl;
//    tokenizer.Consume();
//    if (lhs != nullptr)
//        return IL::CreateError<IL::VariableExpression>("Unexpected token '" + varTok.ToString() + "'. Expected ';'.");
//
//    auto expr = std::make_shared<IL::VariableExpression>();
//    expr->name = varTok.val;
//    return contextManager.Process(expr);
//}
//
//IL::ConstantExpressionPtr Builder::DoConstantExpression(const token::Token& cTok, IL::ExpressionPtr lhs)
//{
//    std::cout << "Found const" << std::endl;
//    tokenizer.Consume();
//    if (lhs != nullptr)
//        return IL::CreateError<IL::ConstantExpression>("Unexpected token '" + cTok.ToString() +  "'. Expected ';'");
//
//    const StringView& raw = cTok.val;
//    for (int i = 0; i < raw.Size(); i++)
//    {
//        if (!IsNumChar(raw[i]))
//            return IL::CreateError<IL::ConstantExpression>("Invalid name '"+ static_cast<std::string>(raw) +"'. Variables can't start with numerals.");
//    }
//
//    auto expr = std::make_shared<IL::ConstantExpression>();
//    expr->constant = raw;
//
//    return contextManager.Process(expr);
//}
//
//
//IL::ExpressionPtr Builder::DoUnknown(const token::Token& cTok, IL::ExpressionPtr lhs)
//{
//    assert(cTok.type == token::UNKNOWN);
//
//    bool var, func, type;
//    contextManager.Current().TypeOfName(cTok.val, &var, &func, &type);
//
//    if (var)
//    {
//        if (lhs == nullptr)
//            return DoVariableExpression(cTok, lhs);
//        if (lhs->GetType() == 
//    }
//
//    if (func)
//        return IL::CreateError<IL::ExpressionPtr>("Function calls not implemented yet.");
//
//    if (type)
//        return DoTypeExpression(cTok, lhs);
//
//    return IL::CreateError<IL::Expression>("Unknown name '"+ static_cast<std::string>(cTok.val) +"'.");
//}
//
//IL::TypeExpressionPtr Builder::DoTypeExpression(const token::Token& cTok, IL::ExpressionPtr lhs)
//{
//    assert(cTok.type == token::UNKNOWN);
//
//    if (lhs == nullptr)
//        return IL::CreateError<TypeExpression>("Unexpected token '"+ cTok.ToString() + "'.");
//
//    
//}






/*******************NEW*********************************/

//// UTILS

template <class T>
std::shared_ptr<T> ErrorUnexpected(const token::Token& tok)
{
    return IL::CreateError<T>("Unexpected token '"+tok.ToString()+"'.");
}

template <class T>
std::shared_ptr<T> ErrorExpecting(const token::Token& tok, const std::string& expecting)
{
    return IL::CreateError<T>("Unexpected token '"+tok.ToString()+"'. Expecting ");
}

template <class T, class U>
std::shared_ptr<T> Upcast(std::shared_ptr<U> val)
{
    return std::static_pointer_cast<T>(val);
}

template <class T, class U>
std::shared_ptr<T> Downcast(std::shared_ptr<U> val)
{
    auto casted = std::dynamic_pointer_cast<T>(val);
    if (casted == nullptr)
        throw frontend::FrontEndException("INTERNAL ERROR: downcast failed.");
    return casted;
}

bool IsConst(StringView val)
{
    // TODO: add much more rigorous checks here
    assert(val.Size() != 0);
    if (val[0] <= '9' && val[0] >= '0')
        return true;
    return false;
}

#define REQUIRE_OR_RETURN(required, type, expecting) \
do { \
if (!RequireSpecific( required )) { \
    auto& macrotok = tokenizer.Current(); \
    tokenizer.Consume(); \
    return ErrorExpecting< type >(macrotok, (expecting)); \
} \
} \
while (false)

bool IsStatement(IL::ExpressionPtr expr)
{
    if (expr == nullptr)
        return false;
    auto type = expr->GetType();
    return type == IL::DECLARATION || type == IL::ASSIGNMENT || type == IL::FUNC_CALL || type == IL::ERROR;
}

bool IsTyped(IL::ExpressionPtr expr)
{
    if (expr == nullptr)
        return false;
    auto type = expr->GetType();
    return type == IL::OPERAND || type == IL::VARIABLE || type == IL::CONSTANT || type == IL::ERROR;
}

//// Process Functions

IL::ExpressionPtr Builder::Process(IL::ExpressionPtr lhs)
{
    Load(1);
    const token::Token& tok = tokenizer.Current();
    tokenizer.Consume();
    switch (tok.type)
    {
        case token::UNKNOWN:
        {
            IL::ExpressionPtr rhs = UnknownToExpression(tok.val);
            return HandleUnknown(lhs, rhs);
        }
        case token::PLUS:
            return HandleOperand(IL::PLUS, lhs);
        case token::MINUS:
            return HandleOperand(IL::MINUS, lhs);
        case token::STAR:
            return HandleOperand(IL::TIMES, lhs);
        case token::SLASH:
            return HandleOperand(IL::DIVIDES, lhs);
        case token::EQUALS:
        {
            return HandleAssignment(lhs);
        }
        default:
            return ErrorUnexpected<IL::Expression>(tok);
    }
    return ErrorUnexpected<IL::Expression>(tok);
}

IL::ExpressionPtr Builder::UnknownToExpression(StringView val)
{
    if (IsConst(val))
        return Upcast<IL::Expression>( factory.CreateConstant(val) );

    bool var, func, type;
    contextManager.Current()->TypeOfName(val, &var, &func, &type);

    if (var)
        return Upcast<IL::Expression>( factory.CreateVariable(val) );

    if (func)
        return Upcast<IL::Expression>( factory.CreateFunction(val) );

    if (type)
        return Upcast<IL::Expression>( factory.CreateType(val) );

    return Upcast<IL::Expression>( factory.CreateIdentifier(val) );
}

IL::ExpressionPtr Builder::HandleUnknown(IL::ExpressionPtr lhs, IL::ExpressionPtr rhs)
{
    assert(rhs != nullptr);
    if (lhs == nullptr)
        return rhs;

    IL::ExpressionType ltype = lhs->GetType();
    IL::ExpressionType rtype = rhs->GetType();

    if (ltype == IL::TYPE && rtype == IL::VARIABLE)
    {
        return IL::CreateError<IL::Expression>("Name already used.");
    }

    if (rtype == IL::FUNCTION)
    {
        return IL::CreateError<IL::Expression>("Function calls are not implemented.");
    }

    if (ltype == IL::TYPE && rtype == IL::IDENTIFIER)
    {
        auto type = Downcast<IL::TypeExpression>(lhs);
        auto id = Downcast<IL::IdentifierExpression>(rhs);
        return factory.CreateDeclaration(type, id);
    }

    return IL::CreateError<IL::Expression>("Unexpected identifier.");
}

IL::ExpressionPtr Builder::HandleOperand(IL::OperandType type, IL::ExpressionPtr lhs)
{
    if (!IsTyped(lhs))
        return IL::CreateError<IL::Expression>("Unexpected operator.");

    IL::TypedExpressionPtr ltyped = Downcast<IL::TypedExpression>(lhs);
    IL::TypedExpressionPtr rhs = RequireTyped();

    return Upcast<IL::Expression>( factory.CreateOperand(type, ltyped, rhs) );
}

IL::ExpressionPtr Builder::HandleAssignment(IL::ExpressionPtr lhs)
{
    if (lhs == nullptr || lhs->GetType() != IL::VARIABLE)
        return IL::CreateError<IL::Expression>("Unexpected '=', can only assign to a variable.");

    IL::VariableExpressionPtr variable = Downcast<IL::VariableExpression>(lhs);
    IL::TypedExpressionPtr rhs = RequireTyped();
    return Upcast<IL::Expression>( factory.CreateAssignment(variable, rhs) );
}

// Take in a token and previously found typed expression. If the token can't be a
//  typedexpression, then returns nullptr.
IL::TypedExpressionPtr Builder::ProcessTyped(const token::Token& tok, IL::TypedExpressionPtr lhs)
{
    switch (tok.type)
    {
        case token::UNKNOWN:
        {
            if (lhs != nullptr)
                return nullptr;

            bool var, func, type;
            contextManager.Current()->TypeOfName(tok.val, &var, &func, &type);
            if (var)
            {
                auto variable = factory.CreateVariable(tok.val);
                return Upcast<IL::TypedExpression>(variable);
            }

            return nullptr;
        }
        case token::PLUS:
            return HandleTypedOperand(IL::PLUS, lhs);
        case token::MINUS:
            return HandleTypedOperand(IL::MINUS, lhs);
        case token::STAR:
            return HandleTypedOperand(IL::TIMES, lhs);
        case token::SLASH:
            return HandleTypedOperand(IL::DIVIDES, lhs);
        default:
            return nullptr;
    }
}

IL::TypedExpressionPtr Builder::HandleTypedOperand(IL::OperandType type, IL::TypedExpressionPtr lhs)
{
    if (lhs == nullptr)
        return Upcast<IL::TypedExpression>( IL::CreateError<IL::VariableExpression>("Unexpected operator.") );

    IL::TypedExpressionPtr rhs = RequireTyped();
    return Upcast<IL::TypedExpression>( factory.CreateOperand(type, lhs, rhs) );
}




//// Require Functions

// Requires a single token of a particular type
// Only consumes the token if it was expected.
bool Builder::RequireSpecific(token::TokenType tokentype)
{
    assert(tokentype != token::UNKNOWN);
    Load(1);
    const token::Token& tok = tokenizer.Current();
    if (tok.type != tokentype)
        return false;

    tokenizer.Consume();
    return true;
}

IL::TypeExpressionPtr Builder::RequireType()
{
    Load(1);
    const token::Token& tok = tokenizer.Current();
    tokenizer.Consume();
    if (tok.type != token::UNKNOWN)
        return ErrorExpecting<IL::TypeExpression>(tok, "a known type.");

    return factory.CreateType(tok.val);
}

IL::IdentifierExpressionPtr Builder::RequireIdentifier()
{
    Load(1);
    const token::Token& tok = tokenizer.Current();
    tokenizer.Consume();
    if (tok.type != token::UNKNOWN)
        return ErrorExpecting<IL::IdentifierExpression>(tok, "an identifier.");

    return factory.CreateIdentifier(tok.val);
}

IL::DeclarationExpressionPtr Builder::RequireDeclaration()
{
    auto type = RequireType();
    auto id = RequireIdentifier();
    return factory.CreateDeclaration(type, id);
}

IL::ExpressionPtr Builder::RequireStatement()
{
    IL::ExpressionPtr current = nullptr;
    while (true)
    {
        Load(1);
        const auto& tok = tokenizer.Current();
        if (tok.type == token::SEMICOLON)
        {
            tokenizer.Consume();
            if (current == nullptr)
                return ErrorUnexpected<IL::Expression>(tok);
            if (!IsStatement(current))
                return IL::CreateError<IL::Expression>("A statement can only be either a declaration, assignment, or function call.");
            return current;
        }

        current = Process(current);

        // Fast pass errors back up.
        if (current != nullptr && current->GetType() == IL::ERROR)
            return current;
    }
}

IL::TypedExpressionPtr Builder::RequireTyped()
{
    IL::TypedExpressionPtr current = nullptr;
    while (true)
    {
        Load(1);
        const token::Token& tok = tokenizer.Current();

        auto next = ProcessTyped(tok, current);

        if (next == nullptr)
        {
            if (current == nullptr)
            {
                tokenizer.Consume();
                return Upcast<IL::TypedExpression>( ErrorUnexpected<IL::VariableExpression>(tok) );
            }
            return current;
        }

        tokenizer.Consume();
        current = next;
    }
}

IL::SubContextExpressionPtr Builder::RequireSubContext()
{
    REQUIRE_OR_RETURN(token::BRACKET_OPEN, IL::SubContextExpression, "'{'");

    IL::SubContextExpressionPtr subcontext = factory.CreateSubContext();

    while (true)
    {
        Load(1);
        if (tokenizer.Current().type == token::BRACKET_CLOSE)
        {
            tokenizer.Consume();
            break;
        }

        IL::ExpressionPtr statement = RequireStatement();
        factory.AddStatement(subcontext, statement);
    }

    REQUIRE_OR_RETURN(token::BRACKET_CLOSE, IL::SubContextExpression, "'}'");

    return subcontext;
}

IL::FuncDeclArgsExpressionPtr Builder::RequireFuncDeclArgs()
{
    REQUIRE_OR_RETURN(token::PAREN_OPEN, IL::FuncDeclArgsExpression, "'('");

    IL::FuncDeclArgsExpressionPtr funcArgs = factory.CreateFuncDeclArgs();

    bool first = true;
    while (true)
    {
        Load(1);
        auto tok = tokenizer.Current();

        if (tok.type == token::PAREN_CLOSE)
        {
            tokenizer.Consume();
            break;
        }

        // Is a comma needed before arg?
        if (first)
        {
            first = false;
        }
        else
        {
            REQUIRE_OR_RETURN(token::COMMA, IL::FuncDeclArgsExpression, "','");
        }

        IL::DeclarationExpressionPtr decl = RequireDeclaration();
        factory.AddFuncDeclArg(funcArgs, decl);
    }

    REQUIRE_OR_RETURN(token::PAREN_CLOSE, IL::FuncDeclArgsExpression, "')'");

    return funcArgs;
}

IL::FunctionDeclarationExpressionPtr Builder::RequireFunctionDeclaration()
{
    auto decl = RequireDeclaration();
    auto args = RequireFuncDeclArgs();
    auto subcontext = RequireSubContext();

    return factory.CreateFunctionDeclaration(decl, args, subcontext);
}

/*** Public functions ***/

Builder::Builder(token::Tokenizer& tokenizer)
    : contextManager(), factory(contextManager), tokenizer(tokenizer), functionSet()
{
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

        std::cout << "Finding function..." << std::endl;

        // Only functions are allowed at the top level at the moment
        IL::FunctionDeclarationExpressionPtr func = RequireFunctionDeclaration();
        functionSet.push_back(func);
        //currentContext->AddFunction(func);
    }

}

void Builder::DebugPrint() const
{
    IL::TreePrinter printer;
    printer.Print("=== Begin Tree Output ===");
    printer.PrintLine();

    for (auto& func : functionSet)
    {
        func->DebugPrint(printer);
        printer.PrintLine();
    }
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


