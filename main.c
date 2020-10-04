#include "mhz19b.h"

#include "i2c.h"


int main(void) {
    const char* tty_device_name = "/dev/serial0";
    const char* i2c_device_name = "/dev/i2c-1";
    const unsigned char lps331ap_i2c_address = 0x5c;
    const unsigned char sht31dis_i2c_address = 0x45;

    i2c_set_device_name(i2c_device_name);

    if (connect_mhz19b(tty_device_name) < 0) return -1;

    byte_t buf[128];
    buf[0] = 0x45;
    buf[1] = 0x21;
    buf[2] = 0x30;

    // read temp/rh
    if (i2c_write(sht31dis_i2c_address, 0x21, &buf[2], 1) < 0) return -1;
    wait_for(0.1);
    if (i2c_read(sht31dis_i2c_address, 0x00, buf, 6) < 0) return -1;

    const int _16BIT_MAX = (1 << 16) - 1;

    unsigned int temp_raw = (buf[0] << 8) | buf[1];
    unsigned int rh_raw = (buf[3] << 8) | buf[4];

    double temp = -45 + 175.0 * temp_raw / _16BIT_MAX;
    double rh = 100.0 * rh_raw / _16BIT_MAX;


    // read pressure
    buf[0] = 0x90;
    if (i2c_write(lps331ap_i2c_address, 0x20, buf, 1) < 0) return -1;
    wait_for(0.1);
    if (i2c_read(lps331ap_i2c_address, 0x28, buf, 1) < 0) return -1;
    unsigned press_xl = buf[0];
    if (i2c_read(lps331ap_i2c_address, 0x29, buf, 1) < 0) return -1;
    unsigned press_l = buf[0];
    if (i2c_read(lps331ap_i2c_address, 0x2A, buf, 1) < 0) return -1;
    unsigned press_h = buf[0];

    __u32 pressure = (press_h << 16) | (press_l << 8) | press_xl;     
    double pressure_hpa = convert_from_complement(pressure, 24) / 4096.0;

    // read co2
    int co2 = get_co2();


  
    printf("%2.1f [C]  %2d [%%]  %6.2f [hPa]  CO2: %4d [ppm]\n", temp, (int)rh, pressure_hpa, co2);


    close_mhz19b();
      
    return 0;
}

