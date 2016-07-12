#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    // Open UART device
    int uart0_filestream = -1;
    uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
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

    // Transmiting bytes
    // unsigned char tx_buffer[BUFF_SIZE];
    // unsigned char *p_tx_buffer;
    //
    // p_tx_buffer = &tx_buffer[0];
    // *p_tx_buffer++ = 'H';
    // *p_tx_buffer++ = 'e';
    // *p_tx_buffer++ = 'l';
    // *p_tx_buffer++ = 'l';
    // *p_tx_buffer++ = 'o';
    // *p_tx_buffer++ = '!';

    // Transmiting buffer
    int word_length = strlen(argv[argc-1]);

    char* tx_buffer = (char*)malloc(word_length * sizeof(unsigned char));

    tx_buffer = argv[argc-1];

    if (uart0_filestream != -1){
        // write_function(Filestream, Begin of bytes to write, number of bytes to write);
        int count = write(uart0_filestream, (const void*)tx_buffer, word_length);
        if(count < 0){
            printf("'UART TX error'\n");
        }
    }
}
