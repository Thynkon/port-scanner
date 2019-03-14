#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "reglib.h"

int reg_match(regex_t *preg, const char *pattern, int cflags,
		char *str_to_check, size_t nmatch, regmatch_t pmatch[], int eflags, char *message) {

	int status = 0;

	if ((regcomp(preg, ADDRESS_REGEX, REG_EXTENDED)) != 0) {
		if (asprintf(&message, "Failed to compile ADDRESS_REGEX\n") > 0) {
			fprintf(stderr, message);
		}

		status = -1;

		free(message);
		return status;
	}

	if ((regexec(preg, str_to_check, nmatch, pmatch, 0)) != 0) {
		if (asprintf(&message, "%s is not a valid ip address\n", str_to_check) > 0) {
			fprintf(stderr, message);
		}

                 status = -1;

		 free(message);
		 return status;
        }

	return status;
}
