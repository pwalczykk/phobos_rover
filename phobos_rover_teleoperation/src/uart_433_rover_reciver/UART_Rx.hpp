#ifndef UART_Rx_HPP_
#define UART_Rx_HPP_

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <stdlib.h>
#include <string.h>

template <typename FrameType>
class UART_Rx{
private:
    int uart0_filestream;
    struct termios options;
protected:
    int rx_length;
    int data_num;
public:
    FrameType WORD;

public:
    UART_Rx(const char* device_addres, const int data_num){
        this->data_num = data_num;
        // Open UART device
        uart0_filestream = -1;
        uart0_filestream = open(device_addres, O_RDONLY | O_NOCTTY | O_NDELAY);
        if(uart0_filestream == -1){
            printf("ERROR - Unable to acces UART\n");
        }

        // UART device settings
        tcgetattr(uart0_filestream, &options);
        options.c_cc[VMIN] = 1;
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
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
        rx_length = read(uart0_filestream, (void*)&WORD, sizeof(WORD));

        if(rx_length == 0){
            return false;
        }
        else if(rx_length > 0){
	        return true;
        }
        else{
            printf("UART RX ERROR!\n");
            return false;
        }
    }


    bool CheckControlSum(){
        int32_t control_sum = 0;

        // Control sum calculated as sum of all WORD's data elements
        for(int i = 0; i < data_num; i++){
            control_sum += *(WORD.begin + i);
        }

        if(control_sum == WORD.control_sum){
            return true;
        }else{
            printf("WRONG CONTROL SUM!!!\n");
            printf("Calculated: %d    Recived: %d\n", control_sum, WORD.control_sum);
            return false;
        }
    }
};

#endif
