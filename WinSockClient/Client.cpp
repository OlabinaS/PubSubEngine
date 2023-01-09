#define WIN32_LEAN_AND_MEAN

#include <windows.h>
//#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
//#include <stdio.h>
#include <conio.h>

#include "Header.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27016

int __cdecl main(int argc, char **argv) 
{
    int topicNum;
    // socket used to communicate with server
    SOCKET connectSocket = INVALID_SOCKET;
    // variable used to store function return value
    int iResult;
    // message to send
    char *messageToSend = "";


    // create a socket
    /*connectSocket = socket(AF_INET,
                           SOCK_STREAM,
                           IPPROTO_TCP);

    if (connectSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }*/

    // create and initialize address structure
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(DEFAULT_PORT);
    // connect to server specified in serverAddress and socket connectSocket
    
    //ConnectionToTCP(connectSocket, serverAddress);
 
    // Send an prepared message with null terminator included
    while (messageToSend != "exit")
    {
        printf("Subscribe to topic:\n1. Sport\n2. Science\n3. History\n4. Politics\n0. Exit\n");

        int a = scanf("%d", &topicNum);
        a = getchar();

        switch (topicNum)
        {
        case 1:
            messageToSend = "Sport"; break;
        case 2:
            messageToSend = "Science"; break;
        case 3:
            messageToSend = "History"; break;
        case 4:
            messageToSend = "Politics"; break;
        case 0:
            messageToSend = "exit"; break;
        default:
            messageToSend = "continue"; break;
        }

        if (messageToSend == "exit")
            break;
        if (messageToSend == "continue")
            continue;

        connectSocket = CreateSocket(connectSocket);
        ConnectionToTCP(connectSocket, serverAddress);

        iResult = send(connectSocket, messageToSend, (int)strlen(messageToSend) + 1, 0);

        if (iResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            continue;
        }

        printf("Bytes Sent: %ld\n", iResult);

        shutdown(connectSocket, SD_SEND);
        closesocket(connectSocket);
    }

    // cleanup
    closesocket(connectSocket);
    WSACleanup();

    return 0;
}


