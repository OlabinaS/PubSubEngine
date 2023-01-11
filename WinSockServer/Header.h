#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>

#include "HashMap.h"


#define DEFAULT_SUB_BUFLEN 32
#define DEFAULT_CLIENT_TCP_PORT "27016"

bool InitializeWindowsSockets();

DWORD WINAPI SubscribeToTopic(LPVOID param);

