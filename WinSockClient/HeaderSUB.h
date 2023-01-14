#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define SLEEP_TIME 50

extern bool stopUDPListener;

bool InitializeWindowsSockets();
bool ConnectionToTCP(SOCKET connectSocket, sockaddr_in serverAddress);

SOCKET CreateTCPSocket(SOCKET connectSocket);

int CreateUniqueUDPPort();
DWORD WINAPI UDP_thread(LPVOID param);

struct TCPMessage
{
	int UDP_port;
	char* topic_message;
};

struct UDPMessage
{
	char* topic;
	char* text;
};