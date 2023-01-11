#include <vcruntime_string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_MAP_SIZE 100

struct portAndTopic
{
	int UDP_port;
	char* topic_message;
};

struct hash_map
{
	portAndTopic data[HASH_MAP_SIZE];
	int count;
};

void hash_map_init(hash_map* map);
void hash_map_insert(hash_map* map, int key, char* topic);
int* hash_map_search(hash_map* map, char* topic);
bool hash_map_exist(hash_map* map, int key, char* topic);
void hash_map_print(hash_map* map);