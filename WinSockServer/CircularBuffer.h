
#define CIRCULAR_BUFFER_SIZE 15

struct topic_and_text
{
	char* topic;
	char* text;
};

struct circular_buffer
{
	topic_and_text data[CIRCULAR_BUFFER_SIZE];
	int head;
	int tail;
	int size;
};
