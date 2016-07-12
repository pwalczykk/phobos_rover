#ifndef UART_TX_HPP_
#define UART_TX_HPP_

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <stdlib.h>
#include <string.h>

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
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
        options.c_iflag = IGNPAR;
        options.c_iflag = 0;
        options.c_iflag = 0;
        tcflush(uart0_filestream, TCIFLUSH);
        tcsetattr(uart0_filestream, TCSANOW, &options);
    }

    ~UART_Tx(){
        close(uart0_filestream);
    }

    void Transmit(char* word){
        // Transmiting buffer
        int word_length = strlen(word);
        char* tx_buffer = (char*)malloc(word_length * sizeof(char));
        tx_buffer = word;

        // Transmiting bytes
        if (uart0_filestream != -1){
            int count = write(uart0_filestream, (const void*)tx_buffer, word_length);
            if(count < 0){
                printf("'UART TX error'\n");
            }
        }
    }
};

#endif
