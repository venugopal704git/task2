#include "baud_rate_check.h"
#define baudrate B115200
int main(int argc,char **argv)
{
	if(argc!=2)
	{
		perror("argv");
		return 0;
	}
	int serial_port=open(argv[1],O_RDWR);
	if (serial_port==-1) 
	{
		perror("Error opening serial port");
		return 0;
	}

	struct termios tty;
	if (tcgetattr(serial_port, &tty) != 0)
	{
		perror("Error getting terminal attributes");
		printf("errorno[%d] is strerr(%s)\n",errno,strerror(errno));
		close(serial_port);
		return 0;
	}
	cfsetispeed(&tty,baudrate);
	cfsetospeed(&tty,B9600);
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	tty.c_cflag |= (CLOCAL | CREAD);
	tty.c_cflag &= ~(PARENB | PARODD);
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	tty.c_lflag = 0;
	tty.c_iflag &= ~(IXON|IXOFF|IXANY);
	tty.c_oflag = 0;
	tty.c_cc[VMIN]  = 1;
	tty.c_cc[VTIME] = 5;

	if (tcsetattr(serial_port,TCSANOW,&tty) != 0) 
	{
		perror("Error setting terminal attributes");
		close(serial_port);
		return 0;
	}
	speed_t device_input_baudrate=cfgetispeed(&tty);
	speed_t device_output_baudrate=cfgetospeed(&tty);
	while(1)
	{
		char output_buffer[1024]={'\0'};int n=0;
		n=read(serial_port,&output_buffer,sizeof(output_buffer));
		if(n<0)
		{
			perror("Error reading to serial port");
			close(serial_port);
			return 1;
		}
		else if(baudrate!=device_input_baudrate || device_output_baudrate!=baudrate)
		{
			printf("baudrate error\n");
			close(serial_port);
			return 1;
		}
		else
		{
			//output_buffer[n]='\0';
			printf("%s",output_buffer);
		}
	}
	close(serial_port);
}
