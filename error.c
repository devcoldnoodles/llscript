#include "error.h"

ErrorDesc* CreateParserError(const char* msg, unsigned int lines)
{
    ErrorDesc* temp = (ErrorDesc*)malloc(sizeof(ErrorDesc));
    temp->msg = msg;
    return temp;
}