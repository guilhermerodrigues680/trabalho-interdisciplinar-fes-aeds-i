#include <string.h>
#include <time.h>
#include "utils.h"

void removeTrailingNewline(char *str)
{
    str[strcspn(str, "\r\n")] = 0;
}

int formatDate(struct tm *date, char *buffer, size_t sizeBuffer)
{
    if (strftime(buffer, sizeBuffer, "%d/%m/%Y %H:%M", date) > sizeBuffer)
        return 0;
    else
        return 1;
}