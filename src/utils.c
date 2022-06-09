#ifndef HEADER_UTILS
#define HEADER_UTILS

#include <string.h>

void removeTrailingNewline(char *str)
{
    str[strcspn(str, "\r\n")] = 0;
}

#endif