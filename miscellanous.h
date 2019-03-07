#ifndef _MISC_H_
#define _MISC_H_

#define MIN_PORT 1
#define MAX_PORT 65534 // Max port number = 2^16, in this case 2^16 - 1

#define PROGRAM_NAME "port-scanner"
#define ADDRESS_REGEX "^[0-9]|[1-9][0-9]|[1-2][0-5][0-5]$"

void usage(FILE* std);

#endif
