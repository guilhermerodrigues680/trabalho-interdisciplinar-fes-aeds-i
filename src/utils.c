#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void removeTrailingNewline(char *str)
{
    str[strcspn(str, "\r\n")] = 0;
}