#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>


#include <fcntl.h>
#include <sys/ioctl.h>


typedef unsigned char byte_t;

int convert_from_complement(unsigned int value, int bit_len);

void close_serial(int fd);

void wait_for(double sec);

