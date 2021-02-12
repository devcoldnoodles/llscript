#include <stdio.h>
#include "parser.h"

int main() 
{
    FILE* fp = fopen("script.txt", "r");
	if (!fp)
	{
		printf("Failed to find script");
		return EXIT_FAILURE;
	}
	char* buffer;
	long size;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	buffer = (char*)malloc(size + 1);
	if (!buffer)
	{
		printf("Failed to allocate buffer");
		return EXIT_FAILURE;
	}
	memset(buffer, 0, size + 1);
	fseek(fp, 0, SEEK_SET);
	fread(buffer, size, 1, fp);
	buffer[size] = 0;
	fclose(fp);
    TokenDesc* desc = Scan(buffer);
    while(desc)
    {
        printf("%d", desc->value);
        desc = desc->next;
    }
    return 0;
}