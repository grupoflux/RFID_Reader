#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int fd; /*File Descriptor*/
	CURL *curl;
	CURLcode res;

	printf("\n +----------------------------------+");
	printf("\n |        Serial Port Read          |");
	printf("\n +----------------------------------+");

	/*------------------------------- Opening the Serial Port -------------------------------*/

	/* Change /dev/ttyUSB0 to the one corresponding to your system */

	fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY); /* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
												  /* O_RDWR   - Read/Write access to serial port       */
												  /* O_NOCTTY - No terminal will control the process   */
												  /* Open in blocking mode,read will wait              */

	if (fd == -1) /* Error Checking */
		printf("\n  Error! in Opening ttyACM0  ");
	else
		printf("\n  ttyACM0 Opened Successfully ");

	/*---------- Setting the Attributes of the serial port using termios structure --------- */

	struct termios SerialPortSettings; /* Create the structure                          */

	tcgetattr(fd, &SerialPortSettings); /* Get the current attributes of the Serial port */

	/* Setting the Baud rate */
	cfsetispeed(&SerialPortSettings, B9600); /* Set Read  Speed as 9600                       */
	cfsetospeed(&SerialPortSettings, B9600); /* Set Write Speed as 9600                       */

	/* 8N1 Mode */
	SerialPortSettings.c_cflag &= ~PARENB; /* Disables the Parity Enable bit(PARENB),So No Parity   */
	SerialPortSettings.c_cflag &= ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;  /* Clears the mask for setting the data size             */
	SerialPortSettings.c_cflag |= CS8;	 /* Set the data bits = 8                                 */

	SerialPortSettings.c_cflag &= ~CRTSCTS;		  /* No Hardware flow Control                         */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */

	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);		   /* Disable XON/XOFF flow control both i/p and o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */

	SerialPortSettings.c_oflag &= ~OPOST; /*No Output Processing*/

	/* Setting Time outs */
	SerialPortSettings.c_cc[VMIN] = 12; /* Read at least 12 characters */
	SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */

	if ((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
		printf("\n  ERROR ! in Setting attributes");
	else
		printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");

	/*------------------------------- Read data from serial port -----------------------------*/

	tcflush(fd, TCIFLUSH); /* Discards old data in the rx buffer            */

	char read_buffer[32]; /* Buffer to store the data received              */
	int bytes_read = 0;   /* Number of bytes read by the read() system call */
	int i = 0;

	bytes_read = read(fd, &read_buffer, 12); /* Read the data                   */

	printf("\n\n  Bytes Rxed -%d", bytes_read); /* Print the number of bytes read */
	printf("\n\n  ");
	printf("\n +----------------------------------+\n\n\n");

	close(fd); /* Close the serial port */

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl == NULL)
	{
		return 128;
	}
	char dest[12];

	char *jsonObja = "uuid=";
	strcpy(dest, jsonObja);
	strcat(dest, read_buffer);

	char *jsonObj = dest;
	struct curl_slist *headers = NULL;
	curl_slist_append(headers, "Accept: application/json");
	curl_slist_append(headers, "Content-Type: application/json");
	curl_slist_append(headers, "charsets: utf-8");

	curl_easy_setopt(curl, CURLOPT_URL, "https://postman-echo.com/post");

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);
	curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, main);
	res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return res;
}