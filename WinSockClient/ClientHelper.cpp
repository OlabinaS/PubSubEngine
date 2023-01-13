#include "HeaderSUB.h"

bool ConnectionToTCP(SOCKET connectSocket, sockaddr_in serverAddress)
{
    if (connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        printf("Unable to connect to server.\n");
        closesocket(connectSocket);
        WSACleanup();

        return false;
    }

    return true;
}

SOCKET CreateSocket(SOCKET connectSocket)
{
    InitializeWindowsSockets();

    connectSocket = socket(AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);

    if (connectSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
    }

    return connectSocket;
}

int CreateUniqueUDPPort()
{
    int min = 10000;
    int max = 40000;
    int port_num = 25000;
    bool used;
    do
    {
        used = false;
        // Seed the random number generator
        srand(time(0));

        // Generate a random number in the range [min, max]
        port_num = rand() % (max - min + 1) + min;

        if (port_num == 27016 || port_num == 27017)
            used = true;


    } while (used);

    return port_num;
}


bool InitializeWindowsSockets()
{
    WSADATA wsaData;
    // Initialize windows sockets library for this process
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return false;
    }
    return true;
}