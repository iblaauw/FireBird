#pragma once

#include "contextmanager.h"
#include "tokenizer.h"
#include "expressionfactory.h"

#include "expression.h"
#include "functionexpression.h"
#include "operandexpression.h"
#include "assignmentexpression.h"
#include "declarationexpression.h"
#include "variableexpression.h"
#include "constantexpression.h"
#include "identifierexpression.h"
#include "functiondeclarationexpression.h"

#include <vector>

namespace firefly
{
namespace frontend
{
    // Combines tokens from the tokenizer and builds
    // the IL tree structurer
    class Builder
    {
    private:
        ContextManager contextManager;
        ExpressionFactory factory;
        firefly::token::Tokenizer& tokenizer;
        std::vector<firefly::IL::FunctionDeclarationExpressionPtr> functionSet;
    public:
        Builder(firefly::token::Tokenizer& tokenizer);
        void Build();
        void DebugPrint() const;

    private:
        void Load(int amount);

        // Processing statements
        IL::ExpressionPtr Process(IL::ExpressionPtr lhs);

        // Helpers for Process
        IL::ExpressionPtr UnknownToExpression(StringView val);
        IL::ExpressionPtr HandleUnknown(IL::ExpressionPtr lhs, IL::ExpressionPtr rhs);
        IL::ExpressionPtr HandleOperand(IL::OperandType type, IL::ExpressionPtr lhs);
        IL::ExpressionPtr HandleAssignment(IL::ExpressionPtr lhs);

        // Typed expressions
        IL::TypedExpressionPtr ProcessTyped(const token::Token& tok, IL::TypedExpressionPtr lhs);
        IL::TypedExpressionPtr HandleTypedOperand(IL::OperandType type, IL::TypedExpressionPtr lhs);

        // Requires
        bool RequireSpecific(token::TokenType tokentype);
        IL::TypeExpressionPtr RequireType();
        IL::IdentifierExpressionPtr RequireIdentifier();
        IL::DeclarationExpressionPtr RequireDeclaration();
        IL::ExpressionPtr RequireStatement();
        IL::TypedExpressionPtr RequireTyped();
        IL::SubContextExpressionPtr RequireSubContext();
        IL::FuncDeclArgsExpressionPtr RequireFuncDeclArgs();
        IL::FunctionDeclarationExpressionPtr RequireFunctionDeclaration();

        /*** OLD ***/

        //IL::FunctionExpressionPtr ParseFunction();
        //IL::FunctionExpressionPtr ParseFunctionSig();
        //void ParseFunctionArgs(IL::FunctionExpressionPtr func);

        //void ParseExpressionSet(std::vector<IL::ExpressionPtr>& expressions);

        //IL::ExpressionPtr ParseTopExpression();
        //IL::ExpressionPtr ParseSubExpression(IL::ExpressionPtr lhs);

        //IL::OperandExpressionPtr DoOperandExpression(IL::OperandType optype, IL::ExpressionPtr lhs);
        //IL::AssignmentExpressionPtr DoAssignmentExpression(IL::ExpressionPtr lhs);
        //IL::DeclarationExpressionPtr DoDeclarationExpression(const token::Token& typeTok, const token::Token& varTok, IL::ExpressionPtr lhs);
        //IL::VariableExpressionPtr DoVariableExpression(const token::Token& varTok, IL::ExpressionPtr lhs);
        //IL::ConstantExpressionPtr DoConstantExpression(const token::Token& cTok, IL::ExpressionPtr lhs);


    };
}
}
