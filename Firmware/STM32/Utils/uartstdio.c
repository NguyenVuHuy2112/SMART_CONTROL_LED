/*********************
 *      INCLUDES
 *********************/

#include "uartstdio.h"
#include <stdlib.h>
#include <string.h>

/**********************
 *      TYPEDEFS
 **********************/

typedef struct
{
    int front, rear, size;

    uint8_t *queue_arr;
} queueHandle_t;

/******************************
 *  STATIC PROTOTYPE FUNCTION
 ******************************/

static void QUEUE_Init(queueHandle_t *queue_x, uint32_t size);
static uint8_t QUEUE_Is_Empty(queueHandle_t *queue_x);
static uint8_t QUEUE_Is_Full(queueHandle_t *queue_x);
static void QUEUE_Push_Data(queueHandle_t * queue_x, uint8_t element);
static uint8_t QUEUE_Pull_Data(queueHandle_t * queue_x);

/**********************
 *  STATIC VARIABLES
 **********************/

static UART_HandleTypeDef uart;
static uint8_t          rxData;
static queueHandle_t    rxBuffer;

/*********************
 * INTERRUPT FUNCTION
 *********************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == uart.Instance)
    {
        QUEUE_Push_Data(&rxBuffer, rxData);
    }
    HAL_UART_Receive_IT(huart, &rxData, 1);
}

/*********************
 *   GLOBAL FUNCTION
 *********************/

/**
 * The function `UARTConfig` initializes a UART configuration with a specified receive buffer size and
 * sets up interrupt-driven reception.
 * 
 * @param uart_p The `uart_p` parameter is a pointer to a structure of type `UART_HandleTypeDef`, which
 * contains configuration settings for a UART (Universal Asynchronous Receiver-Transmitter) peripheral.
 * @param rx_buffer_size The `rx_buffer_size` parameter specifies the size of the receive buffer used
 * for storing incoming data in the UARTConfig function. This buffer will be initialized using the
 * QUEUE_Init function to manage incoming data efficiently.
 */
void UARTConfig(UART_HandleTypeDef * uart_p, uint32_t rx_buffer_size)
{
    uart = *uart_p;
    QUEUE_Init(&rxBuffer, rx_buffer_size);
    HAL_UART_Receive_IT(uart_p, &rxData, 1);
}

/**
 * The function UARTWrite transmits data over UART using the HAL library.
 * 
 * @param data The `data` parameter is a pointer to a character array, which represents the data that
 * you want to transmit over UART.
 * @param bytes The `bytes` parameter in the `UARTWrite` function represents the number of bytes to be
 * transmitted over UART (Universal Asynchronous Receiver-Transmitter) communication. This parameter
 * specifies the size of the data buffer that is being sent through the UART interface.
 */
void UARTWrite(char *  data, uint32_t bytes)
{
    HAL_UART_Transmit(&uart, (const uint8_t *) data, bytes, 1000);
}

/**
 * The function `UARTRead` reads data from a buffer and stores it in a character array until the buffer
 * is empty, returning the number of bytes read.
 * 
 * @param data The `data` parameter in the `UARTRead` function is a pointer to a character array where
 * the received data from the UART will be stored.
 * 
 * @return The function `UARTRead` returns the total number of bytes read from the `rxBuffer` and
 * stored in the `data` array.
 */
uint32_t UARTRead(char * data)
{
    uint32_t Bytes  = 0;
    while (!QUEUE_Is_Empty(&rxBuffer))
    {
        *(data + Bytes) = (char)QUEUE_Pull_Data(&rxBuffer);
        Bytes++;
    }
    return Bytes;
}

/********************
 *  STATIC FUNCTION
 ********************/

/**
 * The function `QUEUE_Init` initializes a queue with the specified size and allocates memory for the
 * queue array.
 * 
 * @param queue_x The `queue_x` parameter is a pointer to a structure of type `queueHandle_t`.
 * @param size The `size` parameter specifies the maximum number of elements that the queue can hold.
 */
static void QUEUE_Init(queueHandle_t *queue_x, uint32_t size)
{
    queue_x->front = -1;
    queue_x->rear = - 1;
    queue_x->size = size;
    queue_x->queue_arr = (uint8_t *)malloc(size * sizeof(uint8_t));
}

/**
 * The function `QUEUE_Is_Empty` checks if a queue is empty based on the front index.
 * 
 * @param queue_x The `queue_x` parameter is a pointer to a `queueHandle_t` structure, which likely
 * represents a queue data structure in the code.
 * 
 * @return The function `QUEUE_Is_Empty` is returning a value of type `uint8_t`, which is an unsigned
 * 8-bit integer. The return value is the result of the comparison `(queue_x->front == -1)`, which
 * checks if the front index of the queue pointed to by `queue_x` is equal to -1. If the front index is
 * -1, the function will
 */
static uint8_t QUEUE_Is_Empty(queueHandle_t *queue_x)
{
    return (queue_x->front == -1);
}

/**
 * The function `QUEUE_Is_Full` checks if a queue is full based on the rear index and the queue size.
 * 
 * @param queue_x The `queue_x` parameter is a pointer to a structure or object of type
 * `queueHandle_t`, which likely represents a queue data structure.
 * 
 * @return The function `QUEUE_Is_Full` is returning a boolean value indicating whether the queue is
 * full or not. It returns 1 if the queue is full (rear is at the last index of the queue), and 0 if
 * the queue is not full.
 */
static uint8_t QUEUE_Is_Full(queueHandle_t *queue_x)
{
    return (queue_x->rear == (queue_x->size - 1));
}

/**
 * The function `QUEUE_Push_Data` adds an element to a queue if it is not full.
 * 
 * @param queue_x queue_x is a pointer to a structure representing a queue. The structure contains
 * members such as front (indicating the front index of the queue), rear (indicating the rear index of
 * the queue), and queue_arr (an array to store elements in the queue).
 * @param element The `element` parameter in the `QUEUE_Push_Data` function represents the data element
 * that you want to add to the queue. It is of type `uint8_t`, which means it is an unsigned 8-bit
 * integer value that can range from 0 to 255. When you call
 * 
 * @return If the `QUEUE_Is_Full(queue_x)` function returns true, the function `QUEUE_Push_Data` will
 * return without pushing the element into the queue.
 */
static void QUEUE_Push_Data(queueHandle_t * queue_x, uint8_t element)
{
    if (QUEUE_Is_Full(queue_x))
    {
        return;
    }
    if (queue_x->front == -1)
    {
        queue_x->front = 0;
    }
    
    queue_x->rear = queue_x->rear + 1;
    queue_x->queue_arr[queue_x->rear] = element;
}

/**
 * The function `QUEUE_Pull_Data` pulls data from a queue and updates the front and rear pointers
 * accordingly.
 * 
 * @param queue_x The `queue_x` parameter is a pointer to a structure of type `queueHandle_t`, which
 * likely contains information about a queue such as the queue array, front index, and rear index.
 * 
 * @return The function `QUEUE_Pull_Data` returns the item that is pulled from the queue. If the queue
 * is empty, it returns `0xff`.
 */
static uint8_t QUEUE_Pull_Data(queueHandle_t * queue_x)
{
    if (QUEUE_Is_Empty(queue_x))
    {
        return 0xff;
    }
    uint8_t item = queue_x->queue_arr[queue_x->front];
    queue_x->front = queue_x->front + 1;
    if (queue_x->front > queue_x->rear)
    {
        queue_x->front = -1;
        queue_x->rear = -1;
    }
    
    return item;
}