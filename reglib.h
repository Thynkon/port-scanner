#ifndef _REGEXLIB_H
#define _REGEXLIB_H

#define ADDRESS_REGEX "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])[.]){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$"

int reg_match(regex_t *preg, const char *pattern, int cflags,
	char *str_to_check, size_t nmatch, regmatch_t pmatch[], int eflags, char *message, char *regex_name);

#endif
