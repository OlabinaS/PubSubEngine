#include <winsock2.h>
#include <stdio.h>

bool InitializeWindowsSockets();
bool ConnectionToTCP(SOCKET connectSocket, sockaddr_in serverAddress);

SOCKET CreateSocket(SOCKET connectSocket);

