#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <regex.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <errno.h>

/* close() function */
#include <unistd.h>

#include "misc.h"
#include "linked_list.h"

void usage(FILE *std) {
	char *usage_message = "%s usage\n"
			"-a [--address]\t\tAdress to scan (from 0.0.0.0 to 255.255.255.255)\n"
			"-h [--help]\t\tDisplay this message\n"
			"-m [--min]\t\tMin value of the ports range to scan (from 1)\n"
			"-M [--max]\t\tMax value of the ports range to scan (up to %d)\n"
			"-p [--port]\t\tIp address with the port to scan (x.x.x.x:p)\n";

	fprintf(std, usage_message, PROGRAM_NAME, MAX_PORT);
}

int main (int argc, char* argv[]) {
	int min = 0;
	int max = 0;

	int status = 0;
	int mode = 0;

	int option = 0;
	int option_index = 0;

	int socket_fd = 0;

	char *address = NULL;
	char *single_port_mode_address = NULL;
	char *message = NULL;
	char *regex_name = NULL;
	const char delimiter[] = ":";
	// Min and max before being converted to int, this allows to check if they match the regex(only strings)
	// cl_min = mix value entered from the command line
	char *cl_min = NULL;
	char *cl_max = NULL;

	size_t nmatch = 4;

	struct node *port_list = NULL;
	struct sockaddr_in client_socket;

	regex_t preg;
	regmatch_t pmatch[nmatch];

	struct option long_options[] = {
		{"address",	required_argument,	0, 'a'},
		{"help",	no_argument,		0, 'h'},
		{"min",		required_argument,	0, 'm'},
		{"max",		required_argument,	0, 'M'},
		{"port",	required_argument,	0, 'p'}
	
	};

	// No arguments passed to this program
	if (argc == 1) {
		usage(stderr);

		status = 1;
		goto END;
	}

	while ((option = getopt_long(argc, argv, "a:hm:M:p:", long_options, &option_index)) != -1) {
		switch (option) {
			case 'a':
				if ((address = strdup(optarg)) == NULL) {
					fprintf(stderr, "Strdup failed!\n");

					status = 1;
					goto END;
				}
				mode = 1;
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

			case 'p':
				if ((single_port_mode_address = strdup(optarg)) == NULL) {
					fprintf(stderr, "Strdup failed!\n");

					status = 1;
					goto END;
				}
				mode = 2;
				break;

			case '?':
				usage(stdout);

				status = 1;
				goto END;

				break; // paranoia

			default:
				break;
		}
	}

	// If address was not entered by the user
	if (address == NULL && mode != 2) {
		usage(stderr);
		status = 1;

		goto END;
	}

	switch (mode) {
		// Port list mode
		case 1:
			if ((regcomp(&preg, ADDRESS_REGEX, REG_EXTENDED)) != 0) {
				if (asprintf(&message, "Failed to compile address regex\n") > 0) {
					fprintf(stderr, "%s", message);
				}

				status = 1;
				goto END;
			}

			if ((regexec(&preg, address, nmatch, pmatch, 0)) != 0) {
				if (asprintf(&message, "%s is not a valid ip adress\n", address) > 0) {
					fprintf(stderr, "%s", message);
				}

				status = 1;
				goto END;
			}
			regfree(&preg);

			// If at least one port limit was given by the user, then I compile PORT_REGEX
			// By doing this, the program uses less memory (don't compile the regex if both ports limits weren't specified)
			if (cl_min != NULL || cl_max != NULL) {
				if ((regcomp(&preg, PORT_REGEX, REG_EXTENDED)) != 0) {
					if (asprintf(&message, "Failed to compile port regex\n") > 0) {
						fprintf(stderr, "%s", message);
					}

					status = 1;
					goto END;
				}
			}

			if (cl_min != NULL) {
				if ((regexec(&preg, cl_min, nmatch, pmatch, 0)) != 0) {
					if (asprintf(&message, "%s is not between 1 and 65534\n", cl_min) > 0) {
						fprintf(stderr, "%s", message);
					}

					status = 1;
					goto END;
				}

				min = atoi(cl_min);
			} else {
				min = MIN_PORT;
			}

			if (cl_max != NULL) {
				if ((regexec(&preg, cl_max, nmatch, pmatch, 0)) != 0) {
					if (asprintf(&message, "%s is not between 1 and 65534\n", cl_max) > 0) {
						fprintf(stderr, "%s", message);
					}

					status = 1;
					goto END;
				}

				max = atoi(cl_max);
			} else {
				max = MAX_PORT;
			}

			regfree(&preg);

			if (min > max) {
				if (asprintf(&message, "Min value is bigger than max value\n") > 0) {
					fprintf(stderr, "%s", message);
				}

				status = 1;
				goto END;
			} else if (min == max) {
				if (asprintf(&message, "Min and max have the same value\nIf you want to scan only one port use the -p option\n\n") > 0) {
					fprintf(stderr, "%s", message);
				}

				usage(stdout);

				status = 1;
				goto END;
			} else {
				// Do nothing
			}
			break;

		case 2: // Single port mode
			if ((regcomp(&preg, SINGLE_PORT_MODE, REG_EXTENDED)) != 0) {
				if (asprintf(&message, "Failed to compile address regex\n") > 0) {
					fprintf(stderr, "%s", message);
				}

				status = 1;
				goto END;
			}

			if ((regexec(&preg, single_port_mode_address, nmatch, pmatch, 0)) != 0) {
				if (asprintf(&message, "%s is not a valid ip adress\n", address) > 0) {
					fprintf(stderr, "%s", message);
				}

				status = 1;
				goto END;
			}
			regfree(&preg);

			address = strtok(single_port_mode_address, delimiter);

			min = atoi(strtok(NULL, delimiter));
			max = min;
			break;

		default:
			break;
	}

	client_socket.sin_family = AF_INET;

	// Setting socket ip address
	if (inet_pton(AF_INET, address, &client_socket.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");

		status = 1;
		goto END;
	}

	// Connecting to each port
	for (int port = min; port <= max; port++) {
		if ((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
			perror("Error");

			status = 1;
			goto END;
		}

		client_socket.sin_port = htons(port);

		if (connect(socket_fd, (struct sockaddr *) &client_socket, sizeof(client_socket)) == 0) {
			if (append_node(&port_list, port) < 0) {
				if (asprintf(&message, "Error : failed to add port to list\n")) {
					fprintf(stderr, "%s", message);

					free(message);
					message = NULL;
				}
			}
		}
		close(socket_fd);
	}

	if (port_list == NULL) {
		if (asprintf(&message, "All specified ports are closed\n")) {
			fprintf(stderr, "%s", message);

			free(message);
			message = NULL;
		}
	} else {
		print_nodes(&port_list);
		delete_nodes(&port_list);
	}

	// Allows to free memory without having a mess(free() and return everywhere)
	END:

	if (message != NULL) {
		free(message);
		message = NULL;
	}

	if (regex_name != NULL) {
		free(regex_name);
		regex_name = NULL;
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
