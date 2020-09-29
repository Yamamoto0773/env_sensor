#include "serial.h"


int convert_from_complement(unsigned int value, int bit_len) {
    if (value & (1 << (bit_len - 1))) {
        unsigned int filter = 1;
        for (int i = 0; i < bit_len - 1; i++) {
            filter = (filter << 1) + 1;
        }

        return ((value ^ filter) + 1) * -1;
    } else {
        return value;
    }
}

void close_serial(int fd) {
    close(fd);
}


void wait_for(double sec) {
    
    clock_t start = clock();

    while (1) {        
        clock_t now = clock();

        double elapsed_s = (double)(now - start) / CLOCKS_PER_SEC;

        if (elapsed_s >= sec) break;
    }
}

