#include "read_data_from_serial_port.h"
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
		printf("errorno[%d] is strerr(%s)\n",errno,strerror(errno));
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
	cfsetospeed(&tty, B115200);
	cfsetispeed(&tty, B115200);
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	tty.c_cflag |= (CLOCAL | CREAD);
	tty.c_cflag &= ~(PARENB | PARODD);
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	tty.c_lflag = 0;
	tty.c_iflag &= ~(IXON|IXOFF|IXANY);
	tty.c_oflag = 0;
	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 10;

	if (tcsetattr(serial_port,TCSANOW,&tty) != 0) 
	{
		perror("Error setting terminal attributes");
		printf("errorno[%d] is strerr(%s)\n",errno,strerror(errno));
		close(serial_port);
		return 0;
	}
	char output_buffer[1024]={'\0'};int character_count=0;
	while(1)
	{ 
		if (character_count=read(serial_port,&output_buffer,sizeof(output_buffer))==-1) 
		{
			perror("Error reading to serial port");
			printf("errorno[%d] is strerr(%s)\n",errno,strerror(errno));
			return 0;
		}
		
		printf("%s",output_buffer);
	}
	close(serial_port);
}

