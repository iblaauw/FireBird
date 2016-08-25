#pragma once

#include "tokenizer.h"
#include "context.h"
#include "expression.h"
#include "function.h"
#include "functionsignature.h"
#include "type.h"

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
        firefly::IL::ContextPtr currentContext;
    public:
        Builder(firefly::token::Tokenizer& tokenizer);

        void Build();

    private:
        void InitBuiltins();
        void Load(int amount); // Load this many tokens from the tokenizer

        firefly::IL::TypePtr ParseType();
        StringView ParseIdentifier();
        void ParseSpecific(firefly::token::TokenType type);

        firefly::IL::FunctionPtr ParseFunction();
        firefly::IL::FunctionSignaturePtr ParseFuncSig(
            firefly::IL::ContextPtr funcContext);
        void ParseFunctionArgs(
            firefly::IL::FunctionSignaturePtr sig,
            firefly::IL::ContextPtr funcContext);
        void ParseFunctionContext(firefly::IL::FunctionPtr func);

        firefly::IL::ExpressionPtr ParseExpression();
    };
}
}
