#ifndef _ERROR_H_
typedef struct ErrorDesc
{
    const char*         msg;
    struct ErrorDesc* next;
} ErrorDesc;

ErrorDesc* CreateParserError(const char* msg, size_t lines);
#endif