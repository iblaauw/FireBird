#pragma once

#include "tokenizer.h"

#include "expression.h"
#include "functionexpression.h"
#include "operandexpression.h"
#include "assignmentexpression.h"
#include "declarationexpression.h"
#include "variableexpression.h"
#include "constantexpression.h"

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
        firefly::token::Tokenizer& tokenizer;
        std::vector<firefly::IL::FunctionExpressionPtr> functionSet;
    public:
        Builder(firefly::token::Tokenizer& tokenizer);
        void Build();

    private:
        void Load(int amount);

        IL::FunctionExpressionPtr ParseFunction();
        IL::FunctionExpressionPtr ParseFunctionSig();
        void ParseFunctionArgs(IL::FunctionExpressionPtr func);

        void ParseExpressionSet(std::vector<IL::ExpressionPtr>& expressions);

        IL::ExpressionPtr ParseTopExpression();
        IL::ExpressionPtr ParseSubExpression(IL::ExpressionPtr lhs);

        IL::OperandExpressionPtr DoOperandExpression(IL::OperandType optype, IL::ExpressionPtr lhs);
        IL::AssignmentExpressionPtr DoAssignmentExpression(IL::ExpressionPtr lhs);
        IL::DeclarationExpressionPtr DoDeclarationExpression(const token::Token& typeTok, const token::Token& varTok, IL::ExpressionPtr lhs);
        IL::VariableExpressionPtr DoVariableExpression(const token::Token& varTok, IL::ExpressionPtr lhs);
        IL::ConstantExpressionPtr DoConstantExpression(const token::Token& cTok, IL::ExpressionPtr lhs);


        /***** OLD *****/

//        void InitBuiltins();
//        void Load(int amount); // Load this many tokens from the tokenizer
//
//        firefly::IL::TypePtr ParseType();
//        StringView ParseIdentifier();
//        void ParseSpecific(firefly::token::TokenType type);
//
//        firefly::IL::FunctionPtr ParseFunction();
//        firefly::IL::FunctionSignaturePtr ParseFuncSig(
//            firefly::IL::ContextPtr funcContext);
//        void ParseFunctionArgs(
//            firefly::IL::FunctionSignaturePtr sig,
//            firefly::IL::ContextPtr funcContext);
//        void ParseFunctionContext(firefly::IL::FunctionPtr func);
//
//        firefly::IL::ExpressionPtr ParseExpression();
    };
}
}
