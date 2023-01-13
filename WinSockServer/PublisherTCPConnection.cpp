#include "HeaderENG.h"

DWORD WINAPI  PublisherTCP(LPVOID param)
{
    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET acceptedSocket = INVALID_SOCKET;
    int iResult;
    char recvbuf[DEFAULT_PUB_BUFLEN];
    topic_and_text data;
    circular_buffer* buffer = (circular_buffer*)param;

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
    iResult = getaddrinfo(NULL, DEFAULT_PUB_TCP_PORT, &hints, &resultingAddress);
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

    printf("Server initialized, waiting for Publisher.\n");

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
            iResult = recv(acceptedSocket, recvbuf, DEFAULT_PUB_BUFLEN, 0);
            if (iResult > 0)
            {
                data.topic = (char*)malloc(sizeof(char) * 16);
                data.text = (char*)malloc(sizeof(char) * (iResult));
                //sscanf(recvbuf, "%s %s", data.topic, data.text);
                //memcpy(&data, recvbuf, sizeof(data));

                data.topic = strtok(recvbuf, " ");
                data.text = strtok(NULL, "|^&");
                
                printf("Topic received from Publisher: %s.\n%s\n", data.topic, data.text);

                // Adding text to circular buffer

                EnterCriticalSection(&cs);
                    addMessage(buffer, data.text, data.topic);
                LeaveCriticalSection(&cs);
                
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

        } while (iResult > 0);

        // server shutdown 
        //circular_buffer_print(buffer);

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