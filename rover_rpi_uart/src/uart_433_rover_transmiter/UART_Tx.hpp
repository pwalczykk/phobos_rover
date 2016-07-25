#ifndef UART_TX_HPP_
#define UART_TX_HPP_

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 8   // 7x4B - data, 1x4B - control

class UART_Tx{
    int uart0_filestream;
    struct termios options;

public:
    UART_Tx(const char* device_addres){
        // Open UART device
        uart0_filestream = -1;
        uart0_filestream = open(device_addres, O_WRONLY | O_NOCTTY | O_NDELAY);
        if(uart0_filestream == -1){
            printf("ERROR - Unable to acces UART\n");
        }

        // UART device settings
        tcgetattr(uart0_filestream, &options);
        options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(uart0_filestream, TCIFLUSH);
        tcsetattr(uart0_filestream, TCSANOW, &options);
    }

    ~UART_Tx(){
        close(uart0_filestream);
    }

    void Transmit(int32_t* word){
        // Transmiting bytes
        if (uart0_filestream != -1){
            int count = write(uart0_filestream, (const void*)word, (BUFF_SIZE) * sizeof(int32_t));
            if(count < 0){
                printf("'UART TX error code: %d'\n", count);
            }
        }
    }

    int32_t ControlSum(int32_t* word){
        int32_t control_sum = 0;

        // Control sum calculated as sum of all word elements
        for(int i = 0; i < BUFF_SIZE-1; i++){
            control_sum += *(word + i);
        }
        return control_sum;
    }

};

#endif
