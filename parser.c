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

SyntaxNode* ParseExpr(TokenDesc** desc);
SyntaxNode* ParseAssign(TokenDesc** desc);
SyntaxNode* ParseTernaryOperator(TokenDesc** desc);

TokenDesc* CreateTokenDesc(short value, void* literal)
{
    TokenDesc* temp = (TokenDesc*)malloc(sizeof(TokenDesc));
    if(!temp)
		return NULL;
    temp->value = value;
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
            desc->next = CreateTokenDesc(LITERAL_INTEGER, (void*)integerValue);
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
			desc->next = CreateTokenDesc(LITERAL_STRING, 0);
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
            desc->next = CreateTokenDesc(LITERAL_CHAR, (void*)temp);
            desc = desc->next;
        }
        break;
        case ':':
            desc->next = CreateTokenDesc(COLON, NULL);
            desc = desc->next;
            break;
        case ';':
            desc->next = CreateTokenDesc(SEMICOLON, NULL);
            desc = desc->next;
            break;
        case '.':
            desc->next = CreateTokenDesc(PERIOD, NULL);
            desc = desc->next;
            break;
        case ',':
            desc->next = CreateTokenDesc(COMMA, NULL);
            desc = desc->next;
            break;
        case '?':
            desc->next = CreateTokenDesc(CONDITIONAL, NULL);
            desc = desc->next;
            break;
        case '(':
            desc->next = CreateTokenDesc(LPAREN, NULL);
            desc = desc->next;
            break;
        case ')':
            desc->next = CreateTokenDesc(RPAREN, NULL);
            desc = desc->next;
            break;
        case '{':
            desc->next = CreateTokenDesc(LBRACE, NULL);
            desc = desc->next;
            break;
        case '}':
            desc->next = CreateTokenDesc(RBRACE, NULL);
            desc = desc->next;
            break;
        case '[':
            desc->next = CreateTokenDesc(LBRACKET, NULL);
            desc = desc->next;
            break;
        case ']':
            desc->next = CreateTokenDesc(RBRACKET, NULL);
            desc = desc->next;
            break;
        case '<':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(LE, NULL), ++src;
            else                        desc->next = CreateTokenDesc(RBRACKET, NULL);
            desc = desc->next;
            break;
        case '>':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(GE, NULL), ++src;
            else                        desc->next = CreateTokenDesc(GT, NULL);
            desc = desc->next;
            break;
        case '!':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(NEQ, NULL), ++src;
            else                        desc->next = CreateTokenDesc(NOT, NULL);
            desc = desc->next;
            break;
        case '=':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(EQ, NULL), ++src;
            else                        desc->next = CreateTokenDesc(ASSIGN, NULL);
            desc = desc->next;
            break;
        case '+':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_ADD, NULL), ++src;
            else if(*(src + 1) == '+')  desc->next = CreateTokenDesc(INC, NULL), ++src;
            else                        desc->next = CreateTokenDesc(ADD, NULL);
            desc = desc->next;
            break;
        case '-':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_SUB, NULL), ++src;
            else if(*(src + 1) == '-')  desc->next = CreateTokenDesc(DEC, NULL), ++src;
            else                        desc->next = CreateTokenDesc(SUB, NULL);
            desc = desc->next;
            break;
        case '*':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_MUL, NULL), ++src;
            else if(*(src + 1) == '*')  desc->next = CreateTokenDesc(POW, NULL), ++src;
            else                        desc->next = CreateTokenDesc(MUL, NULL);
            desc = desc->next;
            break;
        case '/':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_DIV, NULL), ++src, desc = desc->next;
            else if(*(src + 1) == '*')  {src += 2; while(*(src + 1) && !(*src == '*' && *(src + 1) == '/')) ++src; src += 2;}
            else if(*(src + 1) == '/')  {src += 2; while(*src && *src != '\n') ++src;}
            else                        desc->next = CreateTokenDesc(DIV, NULL), desc = desc->next;
            break;
        case '|':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_BOR, NULL), ++src;
            else if(*(src + 1) == '*')  desc->next = CreateTokenDesc(OR, NULL), ++src;
            else                        desc->next = CreateTokenDesc(BOR, NULL);
            desc = desc->next;
            break;
        case '&':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_BAND, NULL), ++src;
            else if(*(src + 1) == '*')  desc->next = CreateTokenDesc(AND, NULL), ++src;
            else                        desc->next = CreateTokenDesc(BAND, NULL);
            desc = desc->next;
            break;
        case '^':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_BXOR, NULL), ++src;
            else                        desc->next = CreateTokenDesc(BXOR, NULL);
            desc = desc->next;
            break;
        case '%':
            if(*(src + 1) == '=')       desc->next = CreateTokenDesc(ASSIGN_MOD, NULL), ++src;
            else                        desc->next = CreateTokenDesc(MOD, NULL);
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

#define FREE_SUCCESS    0
#define FREE_FAIL       1

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

void DeleteSyntaxNode(SyntaxNode* node)
{
    if(!node)                   return;
    if(node->lexpr)     DeleteSyntaxNode(node->lexpr);
    if(node->rexpr)     DeleteSyntaxNode(node->rexpr);
    if(node->expr)      DeleteSyntaxNode(node->expr);
}

SyntaxNode* ParseExpr(TokenDesc** desc)
{
    return ParseAssign(desc);
}

SyntaxNode* ParseAssign(TokenDesc** desc)
{
    TokenDesc* temp = *desc;
    SyntaxNode* lexpr = NULL;
    SyntaxNode* rexpr = NULL;

    if(!(lexpr = ParseTernaryOperator(&temp)))
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
        if (!(rexpr = ParseAssign(&temp)))
            goto ErrorHandle;
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

SyntaxNode* ParseTernaryOperator(TokenDesc** desc)
{
    TokenDesc* temp = *desc;
    SyntaxNode* lexpr = NULL;
    SyntaxNode* rexpr = NULL;
    SyntaxNode* expr = NULL;

    if(!(lexpr = ParseOr(&temp)))
        goto ErrorHandle;

    if(temp->value == CONDITIONAL)
    {
        if(!(expr = ParseTernaryOperator(&temp)))
        {
            goto ErrorHandle;
        }
        if(temp->value != COLON)
        {
            goto ErrorHandle;
        }
        if(!(rexpr = ParseTernaryOperator(&temp)))
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