# include <stdio.h>
# include <stdlib.h>
# include <termios.h> // POSIX Terminal Control Definitions //
# include <unistd.h> // UNIX Standard Definitions //
# include <fcntl.h> // File Control Definitions //
# include <errno.h> // ERROR Number Definitions //


int main() {

    int fd, bytes_read;
    int bit;
    
    fd = open("/dev/ttyUSB0", O_RDWR|O_NOCTTY);

    if (fd == -1) {
        printf("Erro ao abrir a porta USB.\n");
    } else {
        printf("Porta USB aberta com sucesso.\n");
    }

    struct termios tty;

    tcgetattr(fd, &tty);
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    tty.c_cflag &= PARENB;
    tty.c_cflag &= CSTOPB;
    tty.c_cflag &= CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_iflag &= (IXON | IXOFF | IXANY);
    tty.c_iflag &= (ICANON | ECHO | ECHOE | ISIG);

    tty.c_oflag &= OPOST;

    tty.c_cc[VMIN] &= 10;
    tty.c_cc[VTIME] &= 0;

    if ((tcsetattr(fd, TCSANOW, &tty)) != 0) {
        printf("ERROR in setting attributes.\n");
    } else {
        printf("BaudRate = 9600 \t StopBits = 1 \t Parity = none\n");
    } 

    tcflush(fd, TCIFLUSH);
    
    /* trecho com problemas
     
    bytes_read = read(fd, &bit, 1);

    printf("bytes lidos: %d\n", bytes_read);
    printf("%c\n", bit);

    while (1) {

        //bytes_read = read(fd, &bit, 1);
        if (bytes_read == 1) {
            printf("Bit enviado com sucesso!\n");
            break;
        }
    
    }
    */

    close(fd);
    return 0;
}