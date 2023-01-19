#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>
#include <Windows.h>

#include "HashMap.h"
#include "CircularBuffer.h"

extern CRITICAL_SECTION cs;
extern bool StopWorking;

#define DEFAULT_SUB_BUFLEN 32
#define DEFAULT_CLIENT_TCP_PORT "27016"
#define DEFAULT_PUB_TCP_PORT "27017"
#define SLEEP_TIME 1500

bool InitializeWindowsSockets();

DWORD WINAPI SubscriberTCP(LPVOID param);
DWORD WINAPI PublisherTCP(LPVOID param);
DWORD WINAPI SendMessageToSUB(LPVOID param);




struct parameters
{
	hash_map* hash_map;
	circular_buffer* cbuffer;
};

