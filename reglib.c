#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "reglib.h"

int reg_match(regex_t *preg, const char *PATTERN, int cflags,
		char *str_to_check, size_t nmatch, regmatch_t pmatch[], int eflags, char *message, char *regex_name) {

	int status = 0;

	if ((regcomp(preg, PATTERN, REG_EXTENDED)) != 0) {
		if (asprintf(&message, "Failed to compile %s\n", regex_name) > 0) {
			fprintf(stderr, message);
		}

		status = -1;
		return status;
	}

	if ((regexec(preg, str_to_check, nmatch, pmatch, 0)) != 0) {
		//if (asprintf(&message, "%s is not a valid ip address\n", str_to_check) > 0) {
		if (asprintf(&message, "%s does not match %s\n", str_to_check, regex_name) > 0) {
			fprintf(stderr, message);
		}

                 status = -1;
		 return status;
        }

	return status;
}
