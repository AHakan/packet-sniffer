#ifndef SOCKET_H
#define SOCKET_H

#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<netinet/udp.h>	// udp header
#include<netinet/tcp.h>	// tcp header
#include<netinet/ip.h>	// ip header
#include<netinet/if_ether.h>	//For ETH_P_ALL
#include<net/ethernet.h>	//For ether_header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>

int mainfunc(unsigned char *buffer);

#endif
