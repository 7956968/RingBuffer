#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ringBuffer.h"

#define MAX_BUFFER_SIZE 8192

struct _ring_buffer {
    ElementType * buffer;
    int readPos;
    int writePos;
    bool empty;
    bool full;
    int maxSize;

};

RingBuffer * createRingBuffer(int bufferSize) {
    if (bufferSize <= 0) {
        bufferSize = MAX_BUFFER_SIZE;
    }

    RingBuffer * ringBuffer = (RingBuffer *)malloc(sizeof(RingBuffer));
    if (ringBuffer == NULL) {
        return NULL;
    }

    memset(ringBuffer, 0, sizeof(RingBuffer));

    ringBuffer->buffer = (ElementType *)malloc(bufferSize * sizeof(ElementType));
    if (ringBuffer->buffer == NULL) {
        free(ringBuffer);
        ringBuffer = NULL;
        return NULL;
    }

    memset(ringBuffer->buffer, 0, bufferSize * sizeof(ElementType));

    ringBuffer->empty = true;
    ringBuffer->full = false;
    ringBuffer->readPos = 0;
    ringBuffer->writePos = 0;
    ringBuffer->maxSize = bufferSize;

    return ringBuffer;    
}

void releaseRingBuffer(RingBuffer **ppRingBuffer) {
    if (ppRingBuffer == NULL) {
        return;
    }

    RingBuffer *pRingBuffer = *ppRingBuffer;
    if (pRingBuffer == NULL) {
        return;
    }

    if (pRingBuffer->buffer != NULL) {
        free(pRingBuffer->buffer);
        pRingBuffer->buffer = NULL;
    }

    free(pRingBuffer);
    *ppRingBuffer = NULL;
}

int readRingBuffer(RingBuffer * ringBuffer, ElementType * array, int len) {
    int canReadLen = 0;
    int readLen = 0;
    int nextReadPos = 0;
    int i = 0;
    readLen = len;

    if (ringBuffer == NULL || ringBuffer->buffer == NULL || array == NULL) {
        return -1;
    }

    if (ringBuffer->empty == true) {
        return -1;
    }

    if (readLen == 0) {
        return 0;
    }

    int writePos = ringBuffer->writePos;
    int readPos = ringBuffer->readPos;
    int maxBUfferSize = ringBuffer->maxSize;

    if (writePos > readPos) {
        canReadLen = writePos - readPos;
    } 
    else if (writePos == readPos) {
        canReadLen = maxBUfferSize;
    } 
    else {
        canReadLen = maxBUfferSize - (readPos - writePos);
    }

    if (readLen > canReadLen) {
        readLen = canReadLen;
    }

    nextReadPos = (readPos + readLen) % maxBUfferSize;

    if (nextReadPos == (readPos + readLen)) {
        //TODO copy data from ringBuffer->buffer to array
        memcpy(array, ringBuffer->buffer + readPos, readLen);
    } else {
        //TODO 1: copy data in ringBuffer from readPos to end to array
        int firstLen = readLen - nextReadPos;
        memcpy(array, ringBuffer->buffer + readPos, firstLen);
        //TODO 2: copy data in ringBuffer from 0 to nextReadPos to array
        memcpy(array + firstLen, ringBuffer->buffer, nextReadPos);
    }

    readPos = nextReadPos;

    if (writePos == readPos) {
        ringBuffer->empty = true;
    }

    ringBuffer->full = false;
    ringBuffer->readPos = readPos;

    return readLen;
}

int writeRingBuffer(RingBuffer *ringBuffer, ElementType * array, int len) {
    int canWriteLen = 0;
    int writeLen = 0;
    int nextWritePos = 0;
    int i = 0;
    writeLen = len;

    if (ringBuffer == NULL || ringBuffer->buffer == NULL) {
        return -1;
    }

    if (ringBuffer->full == true) {
        return -1;
    }

    if (writeLen == 0) {
        return 0;
    }

    int writePos = ringBuffer->writePos;
    int readPos = ringBuffer->readPos;
    int maxBUfferSize = ringBuffer->maxSize;

    if (writePos > readPos) {
        canWriteLen = maxBUfferSize - (writePos - readPos);
    }
    else if (writePos == readPos) {
        canWriteLen = maxBUfferSize;
    } else {
        canWriteLen = readPos - writePos;
    }

    if (writeLen > canWriteLen) {
        writeLen = canWriteLen;
    }

    nextWritePos = (writePos + writeLen) % maxBUfferSize;

    if (nextWritePos == (writePos + writeLen)) {
        //TODO copy data from array to buffer from writePos to writePos + writeLen
        memcpy(ringBuffer->buffer + writePos, array, writeLen);
    } else {
        //TODO 1: copy data from array to buffer from writePos to end
        int firstLen = writeLen - nextWritePos;
        memcpy(ringBuffer->buffer + writePos, array, firstLen);
        //TODO 2: copy remaining data in array to buffer from 0 to nextWritePos
        memcpy(ringBuffer->buffer, array + firstLen, nextWritePos);
    } 

    writePos = nextWritePos;
    ringBuffer->writePos = writePos;
    if (writePos == readPos) {
        ringBuffer->full = true;
    }
    ringBuffer->empty = false;

    return writeLen;
} 

void traverseRingBuffer(RingBuffer *ringBuffer)
{
    if (ringBuffer == NULL || ringBuffer->buffer == NULL) {
        printf("ringBuffer is NULL\n");
    }

    if (ringBuffer->empty) {
        printf("ringBuffer is Empty\n");
        return;
    }

    if (ringBuffer->full) {
        printf("ringBuffer is full\n");
    }

	int i = 0;
    int readPos = ringBuffer->readPos;
    int writePos = ringBuffer->writePos;
	
    printf("element in RingBuffer:\n");
    if (writePos > readPos) {
        for(i = 0; (readPos + i) < writePos; i++) {
            printf("%c ", ringBuffer->buffer[readPos + i]);
        }
    } else {
        // readPos --> END
        for(i = 0; (readPos + i) < ringBuffer->maxSize; i++) {
            printf("%c ", ringBuffer->buffer[readPos + i]);
        }

        //0 --> writePos
        for (i = 0; i < writePos; i++) {
            printf("%c ", ringBuffer->buffer[i]);
        }
    }
	printf("\n");
}