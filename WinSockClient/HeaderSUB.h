#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool InitializeWindowsSockets();
bool ConnectionToTCP(SOCKET connectSocket, sockaddr_in serverAddress);

SOCKET CreateSocket(SOCKET connectSocket);

int CreateUniqueUDPPort();


struct TCPMessage
{
	int UDP_port;
	char* topic_message;
};

