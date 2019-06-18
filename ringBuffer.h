#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

typedef struct _ring_buffer RingBuffer, *pRingBuffer;
typedef char ElementType;

/**
 * @desp: construct and initialize a ring buffer with certain bufferSize 
 * 
 * @param: bufferSize the maxium element size ring buffer can store.
 *          set to 0 or -1 means use an internal deafult maxium size [8192].
 */
RingBuffer * createRingBuffer(int bufferSize);

/**
 * @desp: deconstruct ring buffer and release internal memery,
 * functiion return with Ringbufer pointer set to be NULL.
 */ 
void releaseRingBuffer(RingBuffer **ppRingBuffer);

/**
 * @desp: read data from ring buffer with expect length
 * 
 * @param: ringBuffer pointer to an intialized ring buffer object.
 * @param: array pointer to destination which store output data.
 * @param: len expect length.
 */ 
int readRingBuffer(RingBuffer * ringBuffer, ElementType * array, int len);

/**
 * @desp: write data from array to ring buffer with expect length
 * 
 * @param: ringBuffer pointer to an intialized ring buffer object.
 * @param: array pointer to source which store input data.
 * @param: len expect length.
 */
int writeRingBuffer(RingBuffer *ringBuffer, ElementType * array, int len);

/**
 * @desp: print the vaild content in ring buffer.
 * @param: ringBuffer pointer to an intialized ring buffer object.
 */ 
void traverseRingBuffer(RingBuffer *ringBuffer);

#endif //_RING_BUFFER_H_