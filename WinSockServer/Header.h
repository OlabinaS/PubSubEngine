#include <winsock2.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <conio.h>


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"

bool InitializeWindowsSockets();

DWORD WINAPI SubscribeToTopic(LPVOID param);