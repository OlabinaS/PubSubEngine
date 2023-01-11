
#include "Header.h"

int  main(void) 
{
    HANDLE SubTCP, PubTCP;

    hash_map map;

    hash_map_init(&map);

    SubTCP = CreateThread(0, 0, &SubscriberTCP, &map, 0, 0);
    PubTCP = CreateThread(0, 0, &PublisherTCP, NULL, 0, 0);
    //ThreadPool
    //UDP slanje klijentu

    CloseHandle(SubTCP);

    getch();
    return 0;
}


