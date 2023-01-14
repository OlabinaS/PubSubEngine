#include "HeaderENG.h"

DWORD WINAPI SendMessageToSUB(LPVOID param)
{
	parameters* prm = (parameters*)param;

	hash_map* map = prm->hash_map;
	circular_buffer* cbuffer = prm->cbuffer;

	topic_and_text* data;


	sockaddr_in serverAddress;
	int sockAddrLen = sizeof(struct sockaddr);
	char outgoingBuffer[DEFAULT_PUB_BUFLEN];
	int serverPort;
	int iResult;

	InitializeWindowsSockets();
	SOCKET clientSocket;
	
	while (!StopWorking)
	{
		EnterCriticalSection(&cs);
			data = getMessage(cbuffer);
		LeaveCriticalSection(&cs);

		if (data == NULL)
		{
			Sleep(SLEEP_TIME);
			continue;
		}

		int* UDP_ports = hash_map_search(map, data->topic);

		if (UDP_ports == NULL)
		{
			printf("There is no one subsciber on the %s topic\n", data->topic);
			continue;
		}
		for (int i = 0; i < map->count; i++)
		{


			serverPort = UDP_ports[i];

			memset((char*)&serverAddress, 0, sizeof(serverAddress));
			serverAddress.sin_family = AF_INET;
			serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
			serverAddress.sin_port = htons((u_short)serverPort);

			clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (clientSocket == INVALID_SOCKET)
			{
				printf("Creating socket failed with error: %d\n", WSAGetLastError());
				WSACleanup();
				return 1;
			}

			int len = sprintf(outgoingBuffer, "%s %s", data->topic, data->text);

			strcat(outgoingBuffer, "|^&");

			iResult = sendto(clientSocket,
							outgoingBuffer,
							strlen(outgoingBuffer),
							0,
							(LPSOCKADDR)&serverAddress,
							sockAddrLen);

			if (iResult == SOCKET_ERROR)
			{
				printf("sendto failed with error: %d\n", WSAGetLastError());
				closesocket(clientSocket);
				WSACleanup();
				return 1;
			}



		}

		
		

		free(UDP_ports);
	}

	iResult = closesocket(clientSocket);
	if (iResult == SOCKET_ERROR)
	{
		printf("closesocket failed with error: %d\n", WSAGetLastError());
		return 1;
	}

	iResult = WSACleanup();
	if (iResult == SOCKET_ERROR)
	{
		printf("WSACleanup failed with error: %ld\n", WSAGetLastError());
		return 1;
	}

}