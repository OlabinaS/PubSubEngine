
#include "Header.h"

int  main(void) 
{
    HANDLE SubTCP;

    SubTCP = CreateThread(0, 0, &SubscribeToTopic, NULL, 0, 0);
    

    CloseHandle(SubTCP);

    getch();
    return 0;
}


