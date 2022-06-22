#ifndef HEADER_UTILS_H
#define HEADER_UTILS_H

#include <time.h>

void removeTrailingNewline(char *str);
int formatDate(struct tm *date, char *buffer, size_t sizeBuffer);

#endif