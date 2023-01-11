#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <ws2tcpip.h>
#include <conio.h>

#include "Header.h"

#define DEFAULT_BUFFER_SIZE 1024
#define DEFAULT_TCP_PORT 27017

int __cdecl main(int argc, char **argv) 
{
    int topicNum;
    SOCKET connectSocket = INVALID_SOCKET;
    int iResult;
    char* topicToSend = "";
    char textToSend[DEFAULT_BUFFER_SIZE - 16] = "";
    TCPMessage message;
    message.topic = "";
    message.text = "";
    char buffer[DEFAULT_BUFFER_SIZE];

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(DEFAULT_TCP_PORT);


    while (topicToSend != "exit")
    {
        printf("Write text on topic:\n1. Sport\n2. Science\n3. History\n4. Politics\n0. Exit\n");

        int a = scanf("%d", &topicNum);
        a = getchar();

        switch (topicNum)
        {
        case 1:
            topicToSend = "Sport"; break;
        case 2:
            topicToSend = "Science"; break;
        case 3:
            topicToSend = "History"; break;
        case 4:
            topicToSend = "Politics"; break;
        case 0:
            topicToSend = "exit"; break;
        default:
            topicToSend = "continue"; break;
        }

        if (topicToSend == "exit")
            break;
        if (topicToSend == "continue")
            continue;

        connectSocket = CreateSocket(connectSocket);
        ConnectionToTCP(connectSocket, serverAddress);

        message.topic = topicToSend;

        //WRITE TEXT ON TOPIC
        //fgets(input, sizeof(input), stdin);
        printf("\n%s:\n", topicToSend);
        fgets(textToSend, DEFAULT_BUFFER_SIZE - sizeof(message.topic) -1, stdin);

        //SEND
        strcat(textToSend, "|^&");
        message.text = textToSend;

        int len = sprintf(buffer, "%s %s", message.topic, message.text);
        iResult = send(connectSocket, buffer, len, 0);

        if (iResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\nTry again\n", WSAGetLastError());
            continue;
        }

        //printf("Bytes Sent: %ld\n", iResult);

        shutdown(connectSocket, SD_SEND);
        closesocket(connectSocket);

        message.topic = "";
        message.text = "";
        memset(buffer, 0, DEFAULT_BUFFER_SIZE);
    }

    // cleanup
    closesocket(connectSocket);
    WSACleanup();

    return 0;
}


