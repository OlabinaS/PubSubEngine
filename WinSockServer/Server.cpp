
#include "HeaderENG.h"

CRITICAL_SECTION cs;
bool StopWorking = false;

int  main(void) 
{
    HANDLE SubTCP, PubTCP, SubUDP;

    hash_map map;
    circular_buffer cbuffer;

    hash_map_init(&map);
    CircularBuffer_Init(&cbuffer);
    InitializeCriticalSection(&cs);

    parameters param;
    param.hash_map = &map;
    param.cbuffer = &cbuffer;

    SubTCP = CreateThread(0, 0, &SubscriberTCP, &map, 0, 0);
    PubTCP = CreateThread(0, 0, &PublisherTCP, &cbuffer, 0, 0);


    SubUDP = CreateThread(0, 0, &SendMessageToSUB, &param, 0, 0);



    CloseHandle(SubTCP);
    CloseHandle(PubTCP);
    CloseHandle(SubUDP);

    freeCircularBuffer(&cbuffer);

    getch();

    StopWorking = true;

    Sleep(100);

    DeleteCriticalSection(&cs);

    return 0;
}


