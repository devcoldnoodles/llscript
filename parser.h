#ifndef __PARSER_H__
#define __PARSER_H__
#include "error.h"

typedef struct ParserDesc
{

} ParserDesc;

struct SyntaxNode;

typedef int (*GenerateFunc)(struct SyntaxNode* node, ParserDesc** data);

typedef struct SyntaxNode
{
    struct SyntaxNode*  lexpr;
    struct SyntaxNode*  expr;
    struct SyntaxNode*  rexpr;
    GenerateFunc        Gererate;
    void*               data;
} SyntaxNode;

int SyntaxAssignGenerateCode(SyntaxNode* node, ParserDesc** data);
int SyntaxUnaryOperatorGenerateCode(SyntaxNode* node, ParserDesc** data);
int SyntaxBinaryOperatorGenerateCode(SyntaxNode* node, ParserDesc** data);
int SyntaxTernaryOperatorGenerateCode(SyntaxNode* node, ParserDesc** data);
int SyntaxLogicalOrGenerateCode(SyntaxNode* node, ParserDesc** data);
int SyntaxLogicalAndGenerateCode(SyntaxNode* node, ParserDesc** data);
int SyntaxCompareGenerateCode(SyntaxNode* node, ParserDesc** deta);
int SyntaxAddGenerateCode(SyntaxNode* node, ParserDesc** data);
int SyntaxMulGenerateCode(SyntaxNode* node, ParserDesc** data);
int SyntaxPrefixGenerateCode(SyntaxNode* node, ParserDesc** data);
int SyntaxPostfixGenerateCode(SyntaxNode* node, ParserDesc** data);
int SyntaxFunctionCallGenerateCode(SyntaxNode* node, ParserDesc** data);

SyntaxNode* ParseExpr(TokenDesc* desc, ErrorDesc* error);
SyntaxNode* ParseAssign(TokenDesc** desc, ErrorDesc* error);
SyntaxNode* ParseTernaryOperator(TokenDesc** desc, ErrorDesc* error);
SyntaxNode* ParseLogicalOr(TokenDesc** desc, ErrorDesc* error);
SyntaxNode* ParseLogicalAnd(TokenDesc** desc, ErrorDesc* error);
SyntaxNode* ParseCompare(TokenDesc** desc, ErrorDesc* error);
SyntaxNode* ParserAdd(TokenDesc** desc, ErrorDesc* error);
SyntaxNode* ParserMul(TokenDesc** desc, ErrorDesc* error);
SyntaxNode* ParsePrefix(TokenDesc** desc, ErrorDesc* error);
SyntaxNode* ParsePostfix(TokenDesc** desc, ErrorDesc* error);
SyntaxNode* ParseElement(TokenDesc** desc, ErrorDesc* error);

#endif