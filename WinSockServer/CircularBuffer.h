#include <corecrt_malloc.h>

#define CIRCULAR_BUFFER_SIZE 20
#define DEFAULT_PUB_BUFLEN 1024

struct topic_and_text
{
	char* topic;
	char* text;
};

struct circular_buffer
{
	topic_and_text data[DEFAULT_PUB_BUFLEN];
	int head;
	int tail;
	int size;
};


void CircularBuffer_Init(circular_buffer* cbuffer);
void freeCircularBuffer(circular_buffer* cb);
void addMessage(circular_buffer* cb, char* text, char* topic);
topic_and_text* getMessage(circular_buffer* cb);