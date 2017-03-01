#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <unistd.h>
int writeByte(int file, int *data)
    {
    if (write(file, data, 1) != 1)
        {
        return 0;
        }
    return 1;
    }

int readByte(int file, int *data)
    {
    if (read(file, data, 1) != 1)
        {
        return 0;
        }
    return 1;
    }

int main(void)
    {
    int file;
    char filename[] = "/dev/i2c-1";
    file = open(filename, "O_RDWR");
    if (file < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
    }
    int addr = 0x04;
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
    }
    int ATtinyData = 240;
    writeByte(file, &ATtinyData);
    }
