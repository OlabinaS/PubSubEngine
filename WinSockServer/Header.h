#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>

#include "HashMap.h"
#include "CircularBuffer.h"

#define DEFAULT_PUB_BUFLEN 1024
#define DEFAULT_SUB_BUFLEN 32
#define DEFAULT_CLIENT_TCP_PORT "27016"
#define DEFAULT_PUB_TCP_PORT "27017"

bool InitializeWindowsSockets();

DWORD WINAPI SubscriberTCP(LPVOID param);
DWORD WINAPI PublisherTCP(LPVOID param);

