#include "HeaderSUB.h"

DWORD WINAPI UDP_thread(LPVOID param) {
    sockaddr_in serverAddress;
    int port = (int)param;
    int sockAddrLen = sizeof(struct sockaddr);
    char accessBuffer[BUFFER_SIZE];
    int iResult;


    UDPMessage data;


    printf("Listening on port: %d\n", port);

    if (InitializeWindowsSockets() == false)
    {
        return 1;
    }

    memset((char*)&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET; //set server address protocol family
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // create UDP socket
    SOCKET serverSocket = socket(AF_INET,      // IPv4 address famly
                                 SOCK_DGRAM,   // datagram socket
                                 IPPROTO_UDP); // UDP

    if (serverSocket == INVALID_SOCKET)
    {
        printf("Creating socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    iResult = bind(serverSocket, (LPSOCKADDR)&serverAddress, sizeof(serverAddress));

    if (iResult == SOCKET_ERROR)
    {
        printf("Socket bind failed with error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }


    unsigned long int nonBlockingMode = 1;
    iResult = ioctlsocket(serverSocket, FIONBIO, &nonBlockingMode);

    if (iResult == SOCKET_ERROR)
    {
        printf("ioctlsocket failed with error: %ld\n", WSAGetLastError());
        return 1;
    }


    while (!stopUDPListener)
    {
        //
        sockaddr_in clientAddress;
        memset(&clientAddress, 0, sizeof(sockaddr_in));
        //


        memset(accessBuffer, 0, BUFFER_SIZE);

        FD_SET set;
        timeval timeVal;

        FD_ZERO(&set);
        FD_SET(serverSocket, &set);

        timeVal.tv_sec = 0;
        timeVal.tv_usec = 0;

        iResult = select(0 , &set, NULL, NULL, &timeVal);

        if (iResult == SOCKET_ERROR)
        {
            fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());
            continue;
        }

        if (iResult == 0)
        {
            // there are no ready sockets, sleep for a while and check again
            Sleep(SLEEP_TIME);
            continue;
        }

        // receive client message
        iResult = recvfrom(serverSocket,
                            accessBuffer,
                            BUFFER_SIZE,
                            0,
                            (LPSOCKADDR)&clientAddress,
                            &sockAddrLen);

        if (iResult == SOCKET_ERROR)
        {
            printf("recvfrom failed with error: %d\n", WSAGetLastError());
            continue;
        }

        ///111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
        char ipAddress[16];
        // copy client ip to local char[]
        strcpy_s(ipAddress, sizeof(ipAddress), inet_ntoa(clientAddress.sin_addr));
        // convert port number from TCP/IP byte order to
        // little endian byte order
        int clientPort = ntohs((u_short)clientAddress.sin_port);

        data.topic = (char*)malloc(sizeof(char) * 16);
        data.text = (char*)malloc(sizeof(char) * (iResult));

        data.topic = strtok(accessBuffer, " ");
        data.text = strtok(NULL, "|^&");

        printf("Engine connected from ip: %s, port: %d\nTopic: %s\n\n%s\n\n", ipAddress, clientPort, data.topic, data.text);

    }


    iResult = closesocket(serverSocket);
    if (iResult == SOCKET_ERROR)
    {
        printf("closesocket failed with error: %ld\n", WSAGetLastError());
        return 1;
    }

    iResult = WSACleanup();
    if (iResult == SOCKET_ERROR)
    {
        printf("WSACleanup failed with error: %ld\n", WSAGetLastError());
        return 1;
    }

    printf("UDP Listener successfully shut down.\n");
    return 0;
}
