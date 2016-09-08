#pragma once

#include "contextmanager.h"
#include "expression.h"
#include "typedexpression.h"
#include "constantexpression.h"
#include "functionexpression.h"
#include "identifierexpression.h"
#include "operandexpression.h"
#include "assignmentexpression.h"
#include "variableexpression.h"
#include "subcontextexpression.h"
#include "funcdeclargsexpression.h"
#include "functiondeclarationexpression.h"

namespace firefly
{
namespace frontend
{
    class ExpressionFactory
    {
    public:
        ExpressionFactory(ContextManager& contextManager);

        IL::ConstantExpressionPtr       CreateConstant(StringView val);
        IL::VariableExpressionPtr       CreateVariable(StringView val);
        IL::FunctionExpressionPtr       CreateFunction(StringView val);
        IL::TypeExpressionPtr           CreateType(StringView val);
        IL::IdentifierExpressionPtr     CreateIdentifier(StringView val);

        IL::DeclarationExpressionPtr    CreateDeclaration(IL::TypeExpressionPtr type, IL::IdentifierExpressionPtr id);
        IL::OperandExpressionPtr        CreateOperand(IL::OperandType type, IL::TypedExpressionPtr lhs, IL::TypedExpressionPtr rhs);
        IL::AssignmentExpressionPtr     CreateAssignment(IL::VariableExpressionPtr var, IL::TypedExpressionPtr rhs);
        IL::FunctionDeclarationExpressionPtr CreateFunctionDeclaration(IL::DeclarationExpressionPtr decl, IL::FuncDeclArgsExpressionPtr args, IL::SubContextExpressionPtr subcontext);


        IL::SubContextExpressionPtr     CreateSubContext();
        IL::FuncDeclArgsExpressionPtr   CreateFuncDeclArgs();

        void AddStatement(IL::SubContextExpressionPtr subcontext, IL::ExpressionPtr statement);
        void AddFuncDeclArg(IL::FuncDeclArgsExpressionPtr args, IL::DeclarationExpressionPtr decl);
    };
}
}

