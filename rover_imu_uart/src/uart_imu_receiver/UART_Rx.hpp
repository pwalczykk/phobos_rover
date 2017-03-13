#ifndef UART_Rx_HPP_
#define UART_Rx_HPP_

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <stdlib.h>
#include <string.h>

class UART_Rx{
    int uart0_filestream;
    struct termios options;

protected:
    int rx_length;
    char rx_buffer[256];

public:
    UART_Rx(const char* device_addres){
        // Open UART device
        uart0_filestream = -1;
        uart0_filestream = open(device_addres, O_RDWR | O_NOCTTY | O_NDELAY);
        if(uart0_filestream){
            printf("ERROR - Unable to acces UART\n");
        }

        // UART device settings
        tcgetattr(uart0_filestream, &options);
        options.c_cflag = B230400 | CS8 | CLOCAL | CREAD;
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(uart0_filestream, TCIFLUSH);
        tcsetattr(uart0_filestream, TCSANOW, &options);
    }

    ~UART_Rx(){
        close(uart0_filestream);
    }
    bool ReadBuffer(){
		
		int checkWrite = write(uart0_filestream,"1",1);
		
        rx_length = read(uart0_filestream, (void*)rx_buffer, 256);

        if(rx_length == 0){
printf("empty buffer\n");
            return false;
        }
        else if(rx_length > 0){
printf("jest cos\n");
            rx_buffer[rx_length] = '\0';
            return true;
        }
        else{
            printf("UART RX ERROR!\n");
            return false;
        }
    }
};

#endif
