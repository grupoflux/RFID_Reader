# include <stdio.h>
# include <stdlib.h>
# include <termios.h> // POSIX Terminal Control Definitions //
# include <unistd.h> // UNIX Standard Definitions //
# include <fcntl.h> // File Control Definitions //
# include <errno.h> // ERROR Number Definitions //
# include <curl/curl.h>


int main() {

    int fd, bytes_read;
    int bit[1];
    
    // abertura da porta serial
    fd = open("/dev/ttyACM0", O_RDWR|O_NOCTTY|O_NDELAY);
    sleep(2);

    //checagem de erro na abertura da porta serial
    if (fd == -1) {
        printf("Erro ao abrir a porta USB.\n");
    } else {
        printf("Porta USB aberta com sucesso.\n");
    }

    // Configuração da porta serial
    struct termios tty;

    tcgetattr(fd, &tty);
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_cflag &= ~CRTSCTS;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    tty.c_oflag &= ~OPOST;

    tty.c_cc[VMIN] &= 10;
    tty.c_cc[VTIME] &= 0;

    if ((tcsetattr(fd, TCSANOW, &tty)) != 0) {
        printf("ERROR in setting attributes.\n");
    } else {
        printf("BaudRate = 9600 \t StopBits = 1 \t Parity = none\n");
    } 

    tcflush(fd, TCIFLUSH);
    
    // leitura do dado que vem do Arduino
    while (1) {

        bytes_read = read(fd, bit, 1);
        if (bytes_read == -1) {
            printf("Erro");
            break;
        } else if (bytes_read == 0) {

            continue;
        } else {
            printf("Bit enviado com sucesso!\n");
            break;
        }
        
    }
    

    close(fd);
    return 0;
}