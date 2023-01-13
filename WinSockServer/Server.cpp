
#include "HeaderENG.h"

CRITICAL_SECTION cs;

int  main(void) 
{
    HANDLE SubTCP, PubTCP;

    hash_map map;
    circular_buffer cbuffer;

    hash_map_init(&map);
    CircularBuffer_Init(&cbuffer);
    InitializeCriticalSection(&cs);


    SubTCP = CreateThread(0, 0, &SubscriberTCP, &map, 0, 0);
    PubTCP = CreateThread(0, 0, &PublisherTCP, &cbuffer, 0, 0);
    //ThreadPool
    //UDP slanje klijentu


    /*while (1)
    {
        EnterCriticalSection(&cs);
            topic_and_text* msg = getMessage(&cbuffer);
        LeaveCriticalSection(&cs);
        if(msg != NULL)
            printf(" %s\n\n%s\n", msg->topic, msg->text);
        Sleep(4000);
    }*/


    CloseHandle(SubTCP);
    CloseHandle(PubTCP);

    freeCircularBuffer(&cbuffer);

    getch();

    DeleteCriticalSection(&cs);

    return 0;
}


