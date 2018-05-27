#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

int main()
{
	write(1,"Connecting to server...\n",24);

	int sd, ns;
	char buf[256] = {0};
	struct sockaddr_in addr;
	int fromlen;
	sd = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(27015);
	addr.sin_addr.s_addr = inet_addr("10.0.2.2");

	if (connect(sd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{	
		write(1,"connection error", 16);
		exit(1);
	}

	write(1,"Connected to Server\n", 20);
	
	while(buf[0]!='*')
	{		
		for (int i = 0; i < 256; i++)
			buf[i] = '\0';

		read(1,buf,10);
		write(sd,buf,10);
		/*if(buf[0] == '*') {
			
			break;
		}*/

		read(sd,buf,10);
		
		if(buf[SIGUSR1] == SIGUSR1)
		{	
			printf("Bye USR1\n");
			break;
		}
		write(1,buf,10);		
	}
}