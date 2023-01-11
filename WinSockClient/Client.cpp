#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <ws2tcpip.h>
#include <conio.h>

#include "Header.h"

#define DEFAULT_BUFFER_SIZE 32
#define DEFAULT_TCP_PORT 27016

int __cdecl main(int argc, char **argv) 
{
    int topicNum;
    SOCKET connectSocket = INVALID_SOCKET;
    int iResult;
    char *messageToSend = "";
    TCPMessage message;
    char buffer[DEFAULT_BUFFER_SIZE];

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(DEFAULT_TCP_PORT);
 

    //Create Unique UDP Port for listening from publisher
    message.UDP_port = CreateUniqueUDPPort();
    printf("Port = %d\n\n", message.UDP_port);


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

        message.topic_message = messageToSend;

        int len = sprintf(buffer, "%s %d", message.topic_message, message.UDP_port);
        //printf("Port %d\nLen %d\n\n", message.UDP_port, len);
        iResult = send(connectSocket, buffer, len, 0);

        if (iResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\nTry again\n", WSAGetLastError());
            continue;
        }

        //printf("Bytes Sent: %ld\n", iResult);

        shutdown(connectSocket, SD_SEND);
        closesocket(connectSocket);

        message.topic_message = "";
        memset(buffer, 0, DEFAULT_BUFFER_SIZE);
    }

    // cleanup
    closesocket(connectSocket);
    WSACleanup();

    return 0;
}


