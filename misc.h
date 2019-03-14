#ifndef _MISC_H_
#define _MISC_H_

#define MIN_PORT 1
#define MAX_PORT 65534 // Max port number = 2^16, in this case 2^16 - 1

#define PROGRAM_NAME "port-scanner"

void usage(FILE* std);

#endif
