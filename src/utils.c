#include <stdio.h>
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

void clearScreen()
{
    printf("\e[1;1H\e[2J");
}

int readDateFromStdin(struct tm *date)
{
    const size_t maxStrLength = 100;
    char dateStr[maxStrLength];
    fgets(dateStr, maxStrLength, stdin);
    removeTrailingNewline(dateStr);

    int wdDay, wdMon, wdYear, wdHour, wdMin;
    if (sscanf(dateStr, "%d/%d/%d %d:%d", &wdDay, &wdMon, &wdYear, &wdHour, &wdMin) != 5)
        return 0;

    time_t tNow = time(NULL);
    struct tm tmNow = *localtime(&tNow);

    // Ajusta a data para tm
    memset(date, 0, sizeof(struct tm));
    date->tm_mday = wdDay;
    date->tm_mon = wdMon - 1;
    date->tm_year = wdYear - 1900;
    date->tm_hour = wdHour;
    date->tm_min = wdMin;

    time_t t = mktime(date);
    if (t == -1)
        return 0;

    return 1;
}
