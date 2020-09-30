#include "mhz19b.h"

char calc_checksum(char *packet) {
    char checksum = 0;

    // skip first byte
    for (int i = 1; i < 8; i++) {
        checksum += packet[i];
    }

    checksum = 0xff - checksum;
    checksum += 1;

    return checksum;
}

struct termios old;
struct termios cur;
char buf[256];
int fd;

int connect_mhz19b(const char* device_name) {
    fd = open(device_name, O_RDWR);
    if (fd < 0) {
        return -1;
    }

    memset(&old, 0, sizeof(old));
    ioctl(fd, TCGETS, &old);

    memset(&cur, 0, sizeof(cur));
    cur.c_cflag = CREAD | CLOCAL | CS8;
    cur.c_iflag = INPCK;
    cfsetspeed(&cur, B9600);

    tcsetattr(fd, TCSANOW, &cur);
    ioctl(fd, TCSETS, &cur);

    return 0;
}

int get_co2() {
    buf[0] = 0xff; // start byte;
    buf[1] = 0x01; // sensor no.1
    buf[2] = 0x86; // command [read CO2 concentration]
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;
    buf[7] = 0x00;
    buf[8] = 0x00; // check sum;
    buf[8] = calc_checksum(buf);
    
    write(fd, buf, 9);

    read(fd, buf, sizeof(buf));

    int co2_high = buf[2];
    int co2_low = buf[3];

    int checksum = buf[8];
    buf[8] = 0;

    if (calc_checksum(buf) != checksum) {
        return -1;
    }

    int co2 = co2_high * 256 + co2_low;    

    return co2;
}


void close_mhz19b() {
    ioctl(fd, TCSETS, &old);
    close(fd);
}


int main() {
    const char* device_name = "/dev/serial0";
    
    if (connect_mhz19b(device_name) < 0) {
        return -1;
    }
    
    while (1) {
        int co2 = get_co2();
        if (co2 < 0) {
            printf("packet was broken\n");
        }

        printf("CO2: %d [ppm]\n", co2);

        wait_for(1.0);
    }

    close_mhz19b();

    return 0;
}
