#include "parser.h"
#include "token.h"

struct Token
{
#define T(SIGN, STR, PREC) SIGN,
    const enum {TOKEN(T)EOT} type;
#undef T
	const int precedence;
	const char str[12];
#define T(SIGN, STR, PREC) {SIGN, PREC, STR},
} tokens[] = {TOKEN(T){EOT}};
#undef T

SyntaxNode* ParseExpr(TokenDesc* desc, ParserError* error);
SyntaxNode* ParseAssign(TokenDesc** desc, ParserError* error);
SyntaxNode* ParseTernaryOperator(TokenDesc** desc, ParserError* error);

TokenDesc* CreateTokenDesc(short value, size_t lines, void* literal)
{
    TokenDesc* temp = (TokenDesc*)malloc(sizeof(TokenDesc));
    if(!temp)
		return NULL;
    temp->value = value;
    temp->lines = lines;
    temp->literal.p = literal;
	temp->next = 0;
    return temp;
}

TokenDesc* Scan(const char* src)
{
    size_t lines = 0;
    TokenDesc head;
    TokenDesc* desc = &head;
    while(*src)
    {
        switch (*src)
        {
        case '\n':
            ++lines;
            break;
        case '\r':case ' ': case '\f': case '\t': case '\v':
            break;
        case '0':
        if(*(src + 1) == 'x')
        {
            int integerValue = 0;
            src += 2;
            while(*src)
            {
                if (*src >= '0' && *src <= '9')
                {
                    integerValue *= 16;
                    integerValue += *src - '0';
                }
                else if (*src >= 'A' && *src <= 'F')
                {
                    integerValue *= 16;
                    integerValue += *src - '0';
                }
                else
				{
                    break;
				}
                ++src;
            }
            desc->next = CreateTokenDesc(LITERAL_INTEGER, lines, (void*)integerValue);
            desc = desc->next;
            break;
        }
        case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
        {
            desc->next = (TokenDesc *)malloc(sizeof(TokenDesc));
            desc = desc->next;
            int integerValue = 0;
            double floatValue = .0;
            while (*src >= '0' && *src <= '9')
            {
                integerValue *= 10;
                integerValue += *src - '0';
                ++src;
            }
            if (*src == '.')
            {
                double decimalIndex = 1;
                while (*src >= '0' && *src <= '9')
                {
                    decimalIndex *= 0.1;
                    floatValue += (*src - '0') * decimalIndex;
                    ++src;
                }
                desc->value = LITERAL_FLOAT;
                desc->literal.d = integerValue + floatValue;
                continue;
            }
            desc->value = LITERAL_INTEGER;
            desc->literal.i = integerValue;
            continue;
        }
        break;
        case '\"':
        {
            const size_t capcity = 1024 * 1024;
            char temp[capcity];
            int pos = 0;
			while(*++src != '\"')
			{
				switch (*src)
				{
				case '\\':
					switch (*(++src))
					{
					case '\\':
						temp[pos++] = '\\';
						break;
					case 't':
						temp[pos++] = '\t';
						break;
					case 'r':
						temp[pos++] = '\r';
						break;
					case 'n':
						temp[pos++] = '\n';
						break;
					case '\'':
						temp[pos++] = '\'';
						break;
					case '\"':
						temp[pos++] = '\"';
						break;
					}
					break;
				default:
					temp[pos++] = *src;
					break;
				}
			}
			desc->next = CreateTokenDesc(LITERAL_STRING, lines, NULL);
			desc = desc->next;
			desc->literal.s = (char*)malloc(pos + 1);
            mempcpy(desc->literal.s, temp, pos);
			desc->literal.s[pos] = 0;
        }
        break;
        case '\'':
        {
            char temp;
            if (*(++src) == '\\')
            {
                switch (*(++src))
                {
                case '\\':
                    temp = '\\';
                    break;
                case 't':
                    temp = '\t';
                    break;
                case 'r':
                    temp = '\r';
                    break;
                case 'n':
                    temp = '\n';
                    break;
                case '\'':
                    temp = '\'';
                    break;
                case '\"':
                    temp = '\"';
                    break;
                default:
                    temp = '\\';
                    break;
                }
            }
            else
			{
                temp = *src;
			}
            desc->next = CreateTokenDesc(LITERAL_CHAR, lines, (void*)temp);
            desc = desc->next;
        }
        break;
        case ':':
            desc->next = CreateTokenDesc(COLON, lines, NULL);
            desc = desc->next;
            break;
        case ';':
            desc->next = CreateTokenDesc(SEMICOLON, lines, NULL);
            desc = desc->next;
            break;
        case '.':
            desc->next = CreateTokenDesc(PERIOD, lines, NULL);
            desc = desc->next;
            break;
        case ',':
            desc->next = CreateTokenDesc(COMMA, lines, NULL);
            desc = desc->next;
            break;
        case '?':
            desc->next = CreateTokenDesc(CONDITIONAL, lines, NULL);
            desc = desc->next;
            break;
        case '(':
            desc->next = CreateTokenDesc(LPAREN, lines, NULL);
            desc = desc->next;
            break;
        case ')':
            desc->next = CreateTokenDesc(RPAREN, lines, NULL);
            desc = desc->next;
            break;
        case '{':
            desc->next = CreateTokenDesc(LBRACE, lines, NULL);
            desc = desc->next;
            break;
        case '}':
            desc->next = CreateTokenDesc(RBRACE, lines, NULL);
            desc = desc->next;
            break;
        case '[':
            desc->next = CreateTokenDesc(LBRACKET, lines, NULL);
            desc = desc->next;
            break;
        case ']':
            desc->next = CreateTokenDesc(RBRACKET, lines, NULL);
            desc = desc->next;
            break;
        case '<':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(LE, lines, NULL), ++src;
            else                        desc->next = CreateTokenDesc(RBRACKET, lines, NULL);
            desc = desc->next;
            break;
        case '>':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(GE, lines, NULL), ++src;
            else                        desc->next = CreateTokenDesc(GT, lines, NULL);
            desc = desc->next;
            break;
        case '!':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(NEQ, lines, NULL), ++src;
            else                        desc->next = CreateTokenDesc(NOT, lines, NULL);
            desc = desc->next;
            break;
        case '=':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(EQ, lines, NULL), ++src;
            else                        desc->next = CreateTokenDesc(ASSIGN, lines, NULL);
            desc = desc->next;
            break;
        case '+':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_ADD, lines, NULL), ++src;
            else if(*(src + 1) == '+')  desc->next = CreateTokenDesc(INC, lines, NULL), ++src;
            else                        desc->next = CreateTokenDesc(ADD, lines, NULL);
            desc = desc->next;
            break;
        case '-':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_SUB, lines, NULL), ++src;
            else if(*(src + 1) == '-')  desc->next = CreateTokenDesc(DEC, lines, NULL), ++src;
            else                        desc->next = CreateTokenDesc(SUB, lines, NULL);
            desc = desc->next;
            break;
        case '*':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_MUL, lines, NULL), ++src;
            else if(*(src + 1) == '*')  desc->next = CreateTokenDesc(POW, lines, NULL), ++src;
            else                        desc->next = CreateTokenDesc(MUL, lines, NULL);
            desc = desc->next;
            break;
        case '/':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_DIV, lines, NULL), ++src, desc = desc->next;
            else if(*(src + 1) == '*')  {src += 2; while(*(src + 1) && !(*src == '*' && *(src + 1) == '/')) ++src; src += 2;}
            else if(*(src + 1) == '/')  {src += 2; while(*src && *src != '\n') ++src;}
            else                        desc->next = CreateTokenDesc(DIV, lines, NULL), desc = desc->next;
            break;
        case '|':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_BOR, lines, NULL), ++src;
            else if(*(src + 1) == '*')  desc->next = CreateTokenDesc(OR, lines, NULL), ++src;
            else                        desc->next = CreateTokenDesc(BOR, lines, NULL);
            desc = desc->next;
            break;
        case '&':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_BAND, lines, NULL), ++src;
            else if(*(src + 1) == '*')  desc->next = CreateTokenDesc(AND, lines, NULL), ++src;
            else                        desc->next = CreateTokenDesc(BAND, lines, NULL);
            desc = desc->next;
            break;
        case '^':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_BXOR, lines, NULL), ++src;
            else                        desc->next = CreateTokenDesc(BXOR, lines, NULL);
            desc = desc->next;
            break;
        case '%':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_MOD, lines, NULL), ++src;
            else                        desc->next = CreateTokenDesc(MOD, lines, NULL);
            desc = desc->next;
            break;
        default:
        {
            const char* temp = src;
            while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || *src == '_')
                ++src;
			if (src - temp > 0)
			{
                desc->next = (TokenDesc*)malloc(sizeof(TokenDesc));
                desc = desc->next;
                desc->value = IDENTIFIER;
                desc->literal.s = (char*)malloc(src - temp + 1);
                mempcpy(desc->literal.s, temp, src - temp);
				desc->literal.s[src - temp] = 0;
                for(int seq = META; seq <= NAMESPACE; ++seq)
                    if(!strcmp(desc->literal.s, tokens[seq].str))
                        desc->value = tokens[seq].type;
				continue;
			}
		}
        break;
        }
        ++src;
    }
    return head.next;
}

SyntaxNode* CreateSyntaxNode(SyntaxNode* lexpr, SyntaxNode* rexpr, SyntaxNode* expr, GenerateFunc func, void* data)
{
    SyntaxNode* temp = (SyntaxNode*)malloc(sizeof(SyntaxNode));
    if(!temp)    return NULL;
    temp->lexpr = lexpr;
    temp->rexpr = rexpr;
    temp->expr = expr;
    temp->Gererate = func;
    temp->data = data;
    return temp;
}

ParserError* CreateParserError(const char* msg, size_t lines)
{
    ParserError* temp = (ParserError*)malloc(sizeof(ParserError));
    temp->msg = msg;
    temp->lines = lines;
    return temp;
}

void DeleteSyntaxNode(SyntaxNode* node)
{
    if(!node)   return;
    DeleteSyntaxNode(node->lexpr);
    DeleteSyntaxNode(node->rexpr);
    DeleteSyntaxNode(node->expr);
    free(node);
}

SyntaxNode* ParseExpr(TokenDesc* desc, ParserError* error)
{
    return ParseAssign(desc, error);
}

SyntaxNode* ParseAssign(TokenDesc** desc, ParserError* error)
{
    TokenDesc* temp = *desc;
    SyntaxNode* lexpr = NULL;
    SyntaxNode* rexpr = NULL;

    if(!(lexpr = ParseTernaryOperator(&temp, error)))
        goto ErrorHandle;

    short value = (**desc).value;
    switch ((**desc).value)
    {
    case ASSIGN:
    case ASSIGN_ADD:
    case ASSIGN_SUB:
    case ASSIGN_MUL:
    case ASSIGN_DIV:
    {
        if (!(rexpr = ParseAssign(&temp, error)))
        {
            error->next = CreateParserError("Expected expression", temp->lines);
            error = error->next;
            goto ErrorHandle;
        }
        lexpr = CreateSyntaxNode(lexpr, rexpr, NULL, SyntaxAssignGenerateCode, value);
    }
    break;
    default:
        break;
    }

    *desc = temp;
    return lexpr;
ErrorHandle:
    if(lexpr)   DeleteSyntaxNode(lexpr);
    if(rexpr)   DeleteSyntaxNode(rexpr);
    return NULL;
}

SyntaxNode* ParseTernaryOperator(TokenDesc** desc, ParserError* error)
{
    TokenDesc* temp = *desc;
    SyntaxNode* lexpr = NULL;
    SyntaxNode* rexpr = NULL;
    SyntaxNode* expr = NULL;


    if(!(lexpr = ParseOr(&temp)))
        goto ErrorHandle;

    if(temp->value == CONDITIONAL)
    {
        if(!(expr = ParseTernaryOperator(&temp, error)))
        {
            goto ErrorHandle;
        }
        if(temp->value != COLON)
        {
            goto ErrorHandle;
        }
        if(!(rexpr = ParseTernaryOperator(&temp, error)))
        {
            goto ErrorHandle;
        }
        lexpr = CreateSyntaxNode(lexpr, rexpr, expr, SyntaxTernaryOperatorGenerateCode, NULL);
    }

    *desc = temp;
    return lexpr;
ErrorHandle:
    if(lexpr)   DeleteSyntaxNode(lexpr);
    if(rexpr)   DeleteSyntaxNode(rexpr);
    if(expr)    DeleteSyntaxNode(expr);
    return NULL;
}