#include "HashMap.h"



void hash_map_init(hash_map* map)
{
    map->count = 0;
    memset(map->data, 0, sizeof(map->data));
}

void hash_map_insert(hash_map* map, int key, char* topic)
{
    for (int i = map->count -1; i < HASH_MAP_SIZE; i++) {
        if (map->data[i].topic_message == NULL) {
            map->data[i].UDP_port = key;
            map->data[i].topic_message = (char*)malloc(sizeof(char) * strlen(topic) + 1);
            strcpy(map->data[i].topic_message, topic);
            map->count++;
            return;
        }
    }
    printf("Map is full");
}

int* hash_map_search(hash_map* map, char* topic)
{
    int* keys = (int*)malloc(sizeof(int) * map->count);
    int j = 0;
    for (int i = 0; i < map->count +1; i++) {
        if (map->data[i].topic_message != NULL && strcmp(map->data[i].topic_message, topic) == 0) {
            keys[j] = map->data[i].UDP_port;
            j++;
        }
    }
    return keys;
}

bool hash_map_exist(hash_map* map, int key, char* topic)
{
    bool exist = false;
    for (int i = 0; i < map->count + 1; i++)
    {
        if (map->data[i].UDP_port == key && strcmp(map->data[i].topic_message, topic) == 0)
        {
            exist = true;
        }
    }

    return exist;
}

void hash_map_print(hash_map* map)
{
    for (int i = 0; i < map->count; i++) {
        //if (map->data[i].topic_message != NULL) {
            printf("Key: %d, Value: %s\n", map->data[i].UDP_port, map->data[i].topic_message);
        //}
    }
}