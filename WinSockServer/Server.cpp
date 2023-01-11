
#include "Header.h"

int  main(void) 
{
    HANDLE SubTCP;

    hash_map map;

    hash_map_init(&map);

    SubTCP = CreateThread(0, 0, &SubscribeToTopic, &map, 0, 0);
    //UDP slanje 
    //TCP publisher

    CloseHandle(SubTCP);

    getch();
    return 0;
}


