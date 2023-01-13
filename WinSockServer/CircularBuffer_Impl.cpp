#include "CircularBuffer.h"
#include <string.h>

void CircularBuffer_Init(circular_buffer* cb)
{
    memset(cb->data, 0, sizeof(cb->data));
    cb->head = 0;
    cb->tail = 0;
    cb->size = 0;
}

void freeCircularBuffer(circular_buffer* cb) {
    for (int i = 0; i < CIRCULAR_BUFFER_SIZE; i++) {
        free(cb->data[i].text);
        free(cb->data[i].topic);
    }
    //free(cb->data);
    //free(cb);
}

void addMessage(circular_buffer* cb, char* text, char* topic) {
    if (cb->size == CIRCULAR_BUFFER_SIZE) {
        // Overwrite oldest data
        free(cb->data[cb->head].text);
        free(cb->data[cb->head].topic);
        cb->head = (cb->head + 1) % CIRCULAR_BUFFER_SIZE;
    }
    else {
        cb->size++;
    }
    int tail = cb->tail;
    cb->data[tail].text = (char*)malloc((strlen(text) + 1) * sizeof(char));
    cb->data[tail].topic = (char*)malloc((strlen(topic) + 1) * sizeof(char));
    strcpy(cb->data[tail].text, text);
    strcpy(cb->data[tail].topic, topic);
    cb->tail = (cb->tail + 1) % CIRCULAR_BUFFER_SIZE;
}

topic_and_text* getMessage(circular_buffer* cb) {
    if (cb->head == cb->tail) {
        // buffer is empty
        return NULL;
    }
    topic_and_text* msg = &cb->data[cb->head];
    cb->head = (cb->head + 1) % CIRCULAR_BUFFER_SIZE;
    return msg;
}