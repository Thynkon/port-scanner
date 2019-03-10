#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <regex.h>
#include <sys/socket.h>

#include "misc.h"

void usage(FILE *std) {
	char *usage_message = "Usage %s\n"
			"-a [--address]\t\tAdress to scan - from 0.0.0.0 to 255.255.255.255\n"
			"-h [--help]\t\tDisplay this message\n"
			"-m [--min]\t\tMin value of the ports range to scan (min = 1)\n"
			"-M [--max]\t\tMax value of the ports range to scan (up to %d)\n";

	fprintf(std, usage_message, PROGRAM_NAME, MAX_PORT);
}


int main (int argc, char* argv[]) {
	int min = 0;
	int max = 0;

	int status = 0;

	int option = 0;
	int option_index = 0;

	char *address = NULL;
	char *message = NULL;
	// Min and max before being converted to int, this allows to check if they match the regex(only strings)
	// cl_min = mix value entered from the command line
	char *cl_min = NULL;
	char *cl_max = NULL;

	size_t nmatch = 4;

	regex_t preg;
	regmatch_t pmatch[nmatch];

	struct option long_options[] = {
		{"address",	required_argument,	0, 'a'},
		{"help",	no_argument,		0, 'h'},
		{"min",		required_argument,	0, 'm'},
		{"max",		required_argument,	0, 'M'}
	
	};

	// No arguments passed to this program
	if (argc == 1) {
		usage(stderr);

		status = 1;
		goto END;
	}

	while ((option = getopt_long(argc, argv, "a:hm:M:", long_options, &option_index)) != -1) {
		switch (option) {
			case 'a':
				if ((address = strdup(optarg)) == NULL) {
					fprintf(stderr, "Strdup failed!\n");

					status = 1;
					goto END;
				}
				break; // paranoia

			case 'h':
				usage(stdout);
				
				goto END;
				break; // paranoia

			case 'm':
				if ((cl_min = strdup(optarg)) == NULL) {
					fprintf(stderr, "Strdup failed to cl_min\n");

					status = 1;
					goto END;
				}
				break; // paranoia
				
			case 'M':
				if ((cl_max = strdup(optarg)) == NULL) {
					fprintf(stderr, "Strdup failed to cl_max\n");

					status = 1;
					goto END;
				}
				break; // paranoia

			case '?':
				usage(stdout);

				status = 1;
				goto END;

				break; // paranoia

			default:
				break; // paranoia
		}
	}

	// If address was not entered by the user
	if (address == NULL) {
		usage(stderr);
		status = 1;

		goto END;
	}
	
	// Min not passed as an argument
	if (cl_min == NULL) {
		min = MIN_PORT;
	}

	// Max not passed as an argument
	if (cl_max == NULL) {
		max = MAX_PORT;
	}

	if ((regcomp(&preg, ADDRESS_REGEX, REG_EXTENDED)) != 0) {
		if (asprintf(&message, "Failed to compile ADDRESS_REGEX\n") > 0) {
			fprintf(stderr, message);
		}

		status = 1;
		goto END;
	}

	if ((regexec(&preg, address, nmatch, pmatch, 0)) != 0) {
		if (asprintf(&message, "%s is not a valid ip address\n", address) > 0) {
			fprintf(stderr, message);
		}

		status = 1;
		goto END;
	}

	fprintf(stdout, "Your ip address : %s\n", address);
	regfree(&preg);

	// Allows to free memory without having a mess(free() and return everywhere)
	END:

	if (message != NULL) {
		free(message);
		message = NULL;
	}

	if (address != NULL) {
		free(address);
		address = NULL;
	}

	if (cl_min != NULL) {
		free(cl_min);
		cl_min = NULL;
	}

	if (cl_max != NULL) {
		free(cl_max);
		cl_max = NULL;
	}

	return status;
}
