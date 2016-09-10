#include "../include/SPI_Master.hpp"

void SPI_Master::OpenBus() {
    const char *spi_name = "/dev/spidev0.0";
    spiDev = open(spi_name, O_RDWR);
    int mode = SPI_MODE_0;
    ioctl(spiDev, SPI_IOC_WR_MODE, &mode);
    // Writing a 0 indicates MSb first; anything else indicates LSb first.
    int lsb_setting = 0;
    ioctl(spiDev, SPI_IOC_WR_LSB_FIRST, &lsb_setting);
    // 0 corresponds to 8 bits per word.
    int bits_per_word = 0;
    ioctl(spiDev, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);

}

int SPI_Master::WriteData(uint8_t txBuffer[], uint8_t rxBuffer[], int len) {
    // Send data
    //    * tx_buf - a pointer to the data to be transferred
    //    * rx_buf - a pointer to storage for received data
    //    * len - length in bytes of tx and rx buffers
    //    * speed_hz - the clock speed, in Hz
    //    * delay_usecs - delay between last bit and deassertion of CS
    //    * bits_per_word - override global word length for this transfer
    //    * cs_change - strobe chip select between transfers?
    //    * pad - ??? leave it alone.
    cout << "Writing: ";
    for(int i=0;i<len;i++) {
        cout << (int)txBuffer[i] << " ";
    }
    cout << endl;
    int res = 0;
    struct spi_ioc_transfer xfer;
    memset(&xfer, 0, sizeof(xfer));
    xfer.tx_buf = (unsigned long)txBuffer;
    xfer.rx_buf = (unsigned long)rxBuffer;
    xfer.len = len;
    xfer.speed_hz = 500000;
    xfer.cs_change = 1;
    xfer.bits_per_word = 8;
    res = ioctl(spiDev, SPI_IOC_MESSAGE(1), &xfer);
    cout << "Output: " << res << endl << "Reading: ";
    for(int i=0;i<len;i++) {
        cout << (int)rxBuffer[i] << " ";
    }
    cout << endl;
    return res;
}

