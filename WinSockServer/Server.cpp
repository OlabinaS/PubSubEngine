
#include "Header.h"


int  main(void) 
{
    HANDLE SubTCP, PubTCP;

    hash_map map;
    circular_buffer cbuffer;

    hash_map_init(&map);
    CircularBuffer_Init(&cbuffer);

    SubTCP = CreateThread(0, 0, &SubscriberTCP, &map, 0, 0);
    PubTCP = CreateThread(0, 0, &PublisherTCP, &cbuffer, 0, 0);
    //ThreadPool
    //UDP slanje klijentu

    while (1)
    {
        topic_and_text* msg = getMessage(&cbuffer);
        if(msg != NULL)
            printf(" %s\n\n%s\n", msg->topic, msg->text);

    }
    CloseHandle(SubTCP);

    getch();
    return 0;
}


