#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    // Open UART device
    int uart0_filestream = -1;
    uart0_filestream = open("/dev/ttyAMA0", O_RDONLY | O_NOCTTY);
    if(uart0_filestream == -1){
        printf("ERROR - Unable to acces UART\n");
    }

    // UART settings
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_iflag = 0;
    options.c_iflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    // UART RX Buffer
    unsigned char rx_buffer[256];

    // Reciving data
    while(1){
        if (uart0_filestream != -1)
        {
            // Read up to 255 characters from the port if they are there
            int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)
            if (rx_length < 0)
            {
                printf("'UART RX error'\n");
            }
            else if (rx_length == 0)
            {
                // printf("No data\n");
            }
            else
            {
                rx_buffer[rx_length] = '\0';
                printf("%i bytes read : %s\n", rx_length, rx_buffer);
            }
        }
    }

    close(uart0_filestream);
    return 0;
}
