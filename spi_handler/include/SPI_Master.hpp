#ifndef SPI_MASTER_HPP_
#define SPI_MASTER_HPP_

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <ros/ros.h>

typedef unsigned char uint8_t;

class SPI_Master {
    int spiDev;
public:
    void OpenBus();
    int WriteData(uint8_t data[], uint8_t response[], int len);
};

#endif

