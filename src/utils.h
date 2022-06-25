/* utils.h
 *
 * Este arquivo representa as declarações de funções, macros e tipos do utils.c.
 *
 *********************************************************************/

#ifndef _UTILS_H_INCLUDED_
#define _UTILS_H_INCLUDED_

#include <time.h>

void utils_remove_trailing_newline(char *str);
int utils_format_date(struct tm *date, char *buffer, size_t size_buffer);
void utils_clear_screen(void);
int utils_read_date_from_stdin(struct tm *date);

#endif