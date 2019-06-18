#include "ringBuffer.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

    RingBuffer * ringBuffer = createRingBuffer(-1);
    char *data = "123456";
    char dest[10] = {0};
    printf("start wirte\n");
    printf("write %d data\n", writeRingBuffer(ringBuffer, data, strlen(data)));
    traverseRingBuffer(ringBuffer);
    readRingBuffer(ringBuffer, dest, 2);
    printf("first Time: %s\n", dest);
    traverseRingBuffer(ringBuffer);
    writeRingBuffer(ringBuffer, data, strlen(data));
    traverseRingBuffer(ringBuffer);

    readRingBuffer(ringBuffer, dest, 5);
    printf("second Time: %s \n", dest);
    traverseRingBuffer(ringBuffer);
    printf("write %d data\n", writeRingBuffer(ringBuffer, data, strlen(data)));
    traverseRingBuffer(ringBuffer);

    releaseRingBuffer(&ringBuffer);
    return 0;
}