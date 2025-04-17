/* ================================================== */
/*                      INCLUDES                      */
/* ================================================== */
#include "OS_Fifo.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "OS.h"

/* ================================================== */
/*            GLOBAL VARIABLE DEFINITIONS             */
/* ================================================== */

/* ================================================== */
/*                 FUNCTION DEFINITIONS               */
/* ================================================== */
bool Enqueue(uint32_t inData, OS_FIFO_t* fifo){
    int16_t nextptr = fifo->head_OSFIFO + 1 >= fifo->size_OSFIFO ? 0 : fifo->head_OSFIFO + 1;
    if (nextptr == fifo->tail_OSFIFO) { // FIFO is full
        return false;
    }
    
    fifo->OS_FIFO[fifo->head_OSFIFO] = inData; // Store data before updating head
    
    fifo->head_OSFIFO = nextptr;
    return true;
}

bool Dequeue(uint32_t* OutData, OS_FIFO_t* fifo){
    if (fifo->head_OSFIFO == fifo->tail_OSFIFO) { // FIFO is empty
        return false;
    }

    *OutData = fifo->OS_FIFO[fifo->tail_OSFIFO];

    fifo->tail_OSFIFO = fifo->tail_OSFIFO + 1 >= fifo->size_OSFIFO ? 0 : fifo->tail_OSFIFO + 1;
    return true;
}


OS_Return_t OS_Fifo_Put(uint32_t data, OS_FIFO_t* fifo){
    // Instead of waiting, return immediately if FIFO is full (ISR-Safe)
    if (fifo->DataRoomAvailable->Value == 0) {
        return 0; // Fail immediately if FIFO is full (non-blocking for ISR)
    }

    OS_Wait(fifo->mutex); // Lock FIFO (critical section)

    bool success = Enqueue(data, fifo);

    OS_Signal(fifo->mutex); // Unlock FIFO
    OS_Signal(fifo->DataAvailable); // Signal that data is available

    return success ? 1 : 0;
}

uint32_t OS_Fifo_Get(OS_FIFO_t* fifo){
    uint32_t dataOut;
    OS_Wait(fifo->DataAvailable); // Wait for data
    OS_Wait(fifo->mutex); // Lock FIFO

    bool success = Dequeue(&dataOut, fifo);

    OS_Signal(fifo->mutex); // Unlock FIFO
    OS_Signal(fifo->DataRoomAvailable); // Signal that space is available

    return success ? dataOut : 0; // Return 0 if failure (not ideal, but prevents undefined behavior)
}

int32_t OS_Fifo_Size(OS_FIFO_t* fifo){
    return fifo->size_OSFIFO;
}

OS_Return_t OS_Fifo_Init(uint32_t size, OS_FIFO_t* fifo, uint32_t* buffer, sema4_t semaphores[3]) {
    if (fifo == NULL || buffer == NULL || semaphores == NULL || size > OS_FIFO_MAXSIZE) {
        return OS_FAIL;
    }

    fifo->OS_FIFO = buffer;
    fifo->mutex = &semaphores[0];
    fifo->DataAvailable = &semaphores[1];
    fifo->DataRoomAvailable = &semaphores[2];

    fifo->head_OSFIFO = 0;
    fifo->tail_OSFIFO = 0;
    fifo->size_OSFIFO = size + 1;  // +1 for distinguishing full vs empty

    OS_InitSemaphore(fifo->mutex, 1);                       // Mutex initially unlocked
    OS_InitSemaphore(fifo->DataAvailable, 0);               // FIFO initially empty
    OS_InitSemaphore(fifo->DataRoomAvailable, size + 1);    // All slots available

    return OS_SUCCESS;
}


