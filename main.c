#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <regex.h>
#include <sys/socket.h>

#include "miscellanous.h"

void usage(FILE* std) {
	char* message = "Usage %s\n"
			"-a [--address]\t\tAdress to scan\n"
			"-h [--help]\t\tDisplay this message\n"
			"-m [--min]\t\tMin value of the ports range to scan\n"
			"-M [--max]\t\tMax value of the ports range to scan\n";

	fprintf(std, message, PROGRAM_NAME);

}


int main (int argc, char* argv[]) {
	int min = 0;
	int max = 0;

	int status = 0;

	int option = 0;
	int option_index = 0;

	char* address = NULL;
	char* message = NULL;
	// Min and max before being converted to int, this allows to check if they match the regex(only strings)
	char* cl_min = NULL;
	char* cl_max = NULL;

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

	while ((option = getopt_long(argc, argv, "a:m:M:", long_options, &option_index)) != -1) {
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
	
	// Min not passed as an argument
	if (cl_min == NULL) {
		min = MIN_PORT;
	}

	// Max not passed as an argument
	if (cl_max == NULL) {
		max = MAX_PORT;
	}

	END:

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
