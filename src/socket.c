#include "../inc/socket.h"

int mainfunc(unsigned char *buffer)
{
	int saddr_size , data_size;
	struct sockaddr saddr;

	printf("Starting...\n");

	int sock_raw = socket( AF_PACKET , SOCK_RAW , htons(ETH_P_ALL)) ;

	if(sock_raw < 0)
	{
		perror("Socket Error");
		return 1;
	}
	while(1)
	{
		saddr_size = sizeof saddr;
		data_size = recvfrom(sock_raw , buffer , 65536 , 0 , &saddr , (socklen_t*)&saddr_size);
		if(data_size <0 )
		{
			printf("Recvfrom error , failed to get packets\n");
			return 1;
		}
	}
	close(sock_raw);
	printf("Finished");
}
