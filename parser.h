#ifndef __PARSER_H__
#define __PARSER_H__

typedef struct ParserError
{
    const char*         msg;
    size_t              lines;
    struct ParserError* next;
} ParserError;

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

SyntaxNode* ParseExpr(TokenDesc* desc, ParserError* error);
SyntaxNode* ParseAssign(TokenDesc** desc, ParserError* error);
SyntaxNode* ParseTernaryOperator(TokenDesc** desc, ParserError* error);
SyntaxNode* ParseLogicalOr(TokenDesc** desc, ParserError* error);
SyntaxNode* ParseLogicalAnd(TokenDesc** desc, ParserError* error);
SyntaxNode* ParseCompare(TokenDesc** desc, ParserError* error);
SyntaxNode* ParserAdd(TokenDesc** desc, ParserError* error);
SyntaxNode* ParserMul(TokenDesc** desc, ParserError* error);
SyntaxNode* ParsePrefix(TokenDesc** desc, ParserError* error);
SyntaxNode* ParsePostfix(TokenDesc** desc, ParserError* error);

#endif