#include "../inc/threads.h"

#include <thread>

using namespace std;

mutex kilit1;


Threads::Threads(uint8_t tSize, char * regex_pattern)
{
	m_ThreadSize = tSize;
	pattern = regex_pattern;

	logfile=fopen("alldata.log0","w");

	printf("Logging file: alldata.log0 \n");

	if(logfile==NULL)
	{
		printf("Unable to create log.txt file.");
	}

	initialize();

}

Threads::~Threads()
{

}

void Threads::initialize()
{
	vector<thread> threads;

	for (uint8_t i = 0; i < m_ThreadSize; i++) {
		threads.push_back(thread(&Threads::printData, this));
	}

	thread m_Th1(mainfunc, buffer);

	for (auto &th : threads) {
		th.join();
	}


	m_Th1.join();
}


void Threads::printData()
{
	while(1)
	{
		kilit1.lock();

		m_ProcessPacket(buffer);
		this_thread::sleep_for(100ms);
		kilit1.unlock();
		this_thread::sleep_for(10ms);

	}
}

void Threads::m_ProcessPacket(unsigned char* buffer)
{
	struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
	switch (iph->protocol) //Check the Protocol and do accordingly...
	{
		case 6:  //TCP Protocol
			m_PrintTcpPacket(buffer);
			break;

		case 17: //UDP Protocol
			m_PrintUdpPacket(buffer);
			break;

		default: //Some Other Protocol like ARP etc.
			break;
	}

}


void Threads::m_PrintIpHeader(unsigned char* Buffer)
{

	unsigned short iphdrlen;
	struct sockaddr_in source,dest;

	struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );
	iphdrlen =iph->ihl*4;

	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iph->saddr;

	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iph->daddr;

	fprintf(logfile , "\n-Protocol         : %d\n",(unsigned int)iph->protocol);
	fprintf(logfile , "-Source IP        : %s\n",inet_ntoa(source.sin_addr));
	fprintf(logfile , "-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));

	snprintf(printPackageBuf, MAX_BUFFER_SIZE, "\nProtocol: %d \nSource IP: %s \nDestination IP: %s \n", (unsigned int)iph->protocol, inet_ntoa(source.sin_addr), inet_ntoa(dest.sin_addr));
 	strncat(packageData, printPackageBuf, sizeof(printPackageBuf) / sizeof(char*) );
}


void Threads::m_PrintTcpPacket(unsigned char* Buffer)
{
	 unsigned short iphdrlen;

	struct iphdr *iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );
	iphdrlen = iph->ihl*4;

	struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));

	int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;

	m_PrintIpHeader(Buffer);

	fprintf(logfile , "-Source Port      : %u\n",ntohs(tcph->source));
	fprintf(logfile , "-Destination Port : %u\n",ntohs(tcph->dest));

	snprintf(printPackageBuf, MAX_BUFFER_SIZE, "Source Port: %u \nDestination Port: %u \n", ntohs(tcph->source), ntohs(tcph->dest));
	strncat(packageData, printPackageBuf, sizeof(printPackageBuf) / sizeof(char*) );

	m_PrintData(Buffer + header_size, header_size);
}


void Threads::m_PrintUdpPacket(unsigned char *Buffer)
{

	unsigned short iphdrlen;

	struct iphdr *iph = (struct iphdr *)(Buffer +  sizeof(struct ethhdr));
	iphdrlen = iph->ihl*4;

	struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  + sizeof(struct ethhdr));

	int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof udph;

	m_PrintIpHeader(Buffer);

	fprintf(logfile , "-Source Port      : %d\n" , ntohs(udph->source));
	fprintf(logfile , "-Destination Port : %d\n" , ntohs(udph->dest));

	snprintf(printPackageBuf, MAX_BUFFER_SIZE, "Source Port: %u \nDestination Port: %u \n", ntohs(udph->source), ntohs(udph->dest));
	strncat(packageData, printPackageBuf, sizeof(printPackageBuf) / sizeof(char*) );

	m_PrintData(Buffer + header_size, header_size);
}


void Threads::m_PrintData(unsigned char* data, int header_size)
{
	int i , j;

	unsigned char m_datatext[65536-header_size];

	string m_data;

	fprintf(logfile , "-Data            :\n");

	for(i=0 ; i < 65536-header_size ; i++)
	{
		if( i!=0 && i%16==0)   //if one line of hex printing is complete...
		{

			for(j=i-16 ; j<i ; j++)
			{
				if(data[j]>=32 && data[j]<=128)
					fprintf(logfile , "%c",(unsigned char)data[j]);
                    if(isalpha((char)data[j]))
						m_data += (char)data[j];
			}
		}

		if( i==65536-header_size -1)  //print the last spaces
		{
			for(j=0;j<15-i%16;j++)
			{
			  fprintf(logfile , "   "); //extra spaces
			}

			for(j=i-i%16 ; j<=i ; j++)
			{
				if(data[j]>=32 && data[j]<=128)
				{
				  	fprintf(logfile , "%c",(unsigned char)data[j]);
                  	if(isalpha((char)data[j]))
						m_data += (char)data[j];
				}
			}

			fprintf(logfile , "\n" );
		}
	}


	if(regex_match(m_data.begin(), m_data.end(), regex(pattern)) == true)
	{
		printf("%s", packageData);
    		cout <<  m_data << endl;
	}

	memset(&packageData[0], 0, sizeof(packageData));
}