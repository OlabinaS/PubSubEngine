#include "Header.h"

DWORD WINAPI  SubscribeToTopic(LPVOID param)
{ 
    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET acceptedSocket = INVALID_SOCKET;
    int iResult;
    char recvbuf[DEFAULT_SUB_BUFLEN];
    portAndTopic data;
    hash_map* map = (hash_map*)param;

    if (InitializeWindowsSockets() == false)
    {
        return 1;
    }

    // Prepare address information structures
    addrinfo* resultingAddress = NULL;
    addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4 address
    hints.ai_socktype = SOCK_STREAM; // Provide reliable data streaming
    hints.ai_protocol = IPPROTO_TCP; // Use TCP protocol
    hints.ai_flags = AI_PASSIVE;     // 

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_CLIENT_TCP_PORT, &hints, &resultingAddress);
    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    listenSocket = socket(AF_INET,      // IPv4 address famly
        SOCK_STREAM,  // stream socket
        IPPROTO_TCP); // TCP

    if (listenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(resultingAddress);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket - bind port number and local address 
    // to socket
    iResult = bind(listenSocket, resultingAddress->ai_addr, (int)resultingAddress->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(resultingAddress);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Since we don't need resultingAddress any more, free it
    freeaddrinfo(resultingAddress);

    // Set listenSocket in listening mode
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    printf("Server initialized, waiting for clients.\n");

    do
    {
        acceptedSocket = accept(listenSocket, NULL, NULL);

        if (acceptedSocket == INVALID_SOCKET)
        {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        do
        {
            iResult = recv(acceptedSocket, recvbuf, DEFAULT_SUB_BUFLEN, 0);
            if (iResult > 0)
            {
                data.topic_message = (char*)malloc(sizeof(char)*(iResult));
                sscanf(recvbuf, "%s %d", data.topic_message, &data.UDP_port);

                if (hash_map_exist(map, data.UDP_port, data.topic_message))
                    printf("\nSubscriber are alredy subscribed on topic\n\n");
                else
                {
                    printf("Topic received from client: %s.\nSend publisher message on UDP port: %d\n\n", data.topic_message, data.UDP_port);
                    hash_map_insert(map, data.UDP_port, data.topic_message);
                }
            }
            else if (iResult == 0)
            {
                closesocket(acceptedSocket);
            }
            else
            {
                //error
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(acceptedSocket);
            }

            memset(recvbuf, 0, DEFAULT_SUB_BUFLEN);
            data.UDP_port = 0;
        } while (iResult > 0);

        // server shutdown 
        //hash_map_print(map);

    } while (1);

    // shutdown the connection since we're done
    iResult = shutdown(acceptedSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(acceptedSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(listenSocket);
    closesocket(acceptedSocket);
    WSACleanup();
}