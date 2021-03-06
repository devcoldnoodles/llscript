#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stdlib.h>
#include <string.h>

#define TOKEN(_ELEM_)                	\
	/* category : symbols */          	\
	_ELEM_(LBRACE, "{", 0)            	\
	_ELEM_(LBRACKET, "[", 0)          	\
	_ELEM_(LPAREN, "(", 0)            	\
	_ELEM_(RBRACE, "}", 0)            	\
	_ELEM_(RBRACKET, "]", 0)          	\
	_ELEM_(RPAREN, ")", 0)            	\
	_ELEM_(PERIOD, ".", 0)            	\
	_ELEM_(COMMA, ",", 1)             	\
	_ELEM_(COLON, ":", 0)             	\
	_ELEM_(SEMICOLON, ";", 0)         	\
	/* unary operator */              	\
	_ELEM_(NOT, "!", 0)               	\
	_ELEM_(BNOT, "~", 0)              	\
	_ELEM_(INC, "++", 0)              	\
	_ELEM_(DEC, "--", 0)              	\
	/* binary operator */             	\
	_ELEM_(ARROW, "=>", 0)            	\
	_ELEM_(ASSIGN, "=", 2)            	\
	_ELEM_(ADD, "+", 12)              	\
	_ELEM_(SUB, "-", 12)              	\
	_ELEM_(MUL, "*", 11)              	\
	_ELEM_(DIV, "/", 11)              	\
	_ELEM_(MOD, "%", 11)              	\
	_ELEM_(ASSIGN_ADD, "+=", 3)			\
	_ELEM_(ASSIGN_SUB, "-=", 3)			\
	_ELEM_(ASSIGN_MUL, "*=", 3)			\
	_ELEM_(ASSIGN_DIV, "/=", 3)			\
	_ELEM_(ASSIGN_MOD, "%=", 3)			\
	_ELEM_(POW, "**", 11)				\
	_ELEM_(BOR, "|", 6)               	\
	_ELEM_(BAND, "&", 8)              	\
	_ELEM_(BXOR, "^", 7)              	\
	_ELEM_(ASSIGN_BOR, "|=", 3)			\
	_ELEM_(ASSIGN_BAND, "&=", 3)		\
	_ELEM_(ASSIGN_BXOR, "^=", 3)		\
	_ELEM_(OR, "||", 4)               	\
	_ELEM_(AND, "&&", 5)              	\
	_ELEM_(EQ, "==", 5)              	\
	_ELEM_(NEQ, "!=", 5)              	\
	_ELEM_(LT, "<", 5)              	\
	_ELEM_(LE, "<=", 5)              	\
	_ELEM_(GT, ">", 5)              	\
	_ELEM_(GE, ">=", 5)              	\
	/* tarnery operator */            	\
	_ELEM_(CONDITIONAL, "?", 3)       	\
	/* keywords */          			\
	_ELEM_(META, "meta", 0)				\
	_ELEM_(NEW, "new", 0)				\
	_ELEM_(BYTE, "byte", 0)           	\
	_ELEM_(IF, "if", 0)               	\
	_ELEM_(ELSE, "else", 0)           	\
	_ELEM_(MATCH, "match", 0)         	\
	_ELEM_(LOOP, "loop", 0)           	\
	_ELEM_(DEFAULT, "default", 0)     	\
	_ELEM_(RETURN, "return", 0)       	\
	_ELEM_(SELF, "self", 0)           	\
	_ELEM_(CONST, "const", 0)         	\
	_ELEM_(STATIC, "static", 0)       	\
	_ELEM_(ENUM, "enum", 0)           	\
	_ELEM_(STRUCT, "struct", 0)       	\
	_ELEM_(OPERATOR, "operator", 0)   	\
	_ELEM_(GET, "get", 0)             	\
	_ELEM_(SET, "set", 0)             	\
	_ELEM_(NAMESPACE, "namespace", 0) 	\
	/* literals */						\
	_ELEM_(LITERAL, "", 0)				\
	_ELEM_(LITERAL_INTEGER, "", 0)		\
	_ELEM_(LITERAL_FLOAT, "", 0)		\
	_ELEM_(LITERAL_CHAR, "", 0)			\
	_ELEM_(LITERAL_STRING, "", 0)		\
	_ELEM_(IDENTIFIER, "", 0)			\

typedef struct TokenDesc
{
	short value;
	size_t lines;
	struct TokenDesc* next;
	union {
		void* p;
		char c;
		char* s;
		int i;
		double d;
	} literal;
} TokenDesc;

#endif