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

    // Transmiting buffer
    int word_length = strlen(argv[argc-1]);
    char* tx_buffer = (char*)malloc(word_length * sizeof(unsigned char));
    tx_buffer = argv[argc-1];

    // Transmiting bytes
    if (uart0_filestream != -1){
        int count = write(uart0_filestream, (const void*)tx_buffer, word_length);
        if(count < 0){
            printf("'UART TX error'\n");
        }
    }

    // Closing UART
    close(uart0_filestream);
    return 0;
}
