#ifndef _ERROR_H_
#include <stdlib.h>

#define ERRORLIST(FILTER)                                       \
FILTER(_ERROR_EXPECTED_EXPRESSION, "Expected expression")       \
FILTER(_ERROR_EXPECTED_LITERAL, "Expected Literal")             \
FILTER(_ERROR_EXPECTED, "Expected ")                            \

#define E(ENUM, MSG) ENUM,
enum ErrorType{ERRORLIST(E)};
#undef E

typedef struct ErrorDesc
{
    const char* msg;
    struct ErrorDesc* next;
} ErrorDesc;

ErrorDesc* CreateParserError(const char* msg, unsigned int lines);
#endif