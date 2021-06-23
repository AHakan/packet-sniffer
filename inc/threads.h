#ifndef THREADS_H
#define THREADS_H

#include <iostream>
#include <vector>
#include <cinttypes>
#include <mutex>
#include <regex>


extern "C" {
  #include "../inc/socket.h" //a C header, so wrap it in extern "C"
}


#define MAX_BUFFER_SIZE 32768

class Threads{
	public:
		Threads(uint8_t tSize, char * regex_pattern);
		~Threads();

		void initialize();
		void printData();

		char * pattern;
		unsigned char *buffer = (unsigned char *) malloc(65536);
		int *data_size = (int *) malloc(32);

		char printPackageBuf[MAX_BUFFER_SIZE];
		char packageData[MAX_BUFFER_SIZE];

		FILE *logfile;

	private:
		uint8_t m_ThreadSize;

		void m_ProcessPacket(unsigned char* buffer);
		void m_PrintIpHeader(unsigned char* Buffer);
		void m_PrintTcpPacket(unsigned char* Buffer);
		void m_PrintUdpPacket(unsigned char *Buffer);
		void m_PrintData (unsigned char* data, int header_size);


};

#endif