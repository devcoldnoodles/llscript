#include <stdio.h>
#include "token.h"

int main() 
{
    TokenDesc* desc = Scan("test=\"this is test\"+3+2;");
    while(desc)
    {
        printf("%d", desc->value);
        desc = desc->next;
    }
    return 0;
}