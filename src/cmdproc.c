/* ****************************** */
/* See cmdProc.h for indications  */
/* ****************************** */
#include <stdio.h>
#include <string.h>

#include "cmdproc.h"

/* Constants */
#define SOF_SYM '#'  // Start of Frame symbol
#define EOF_SYM '!'  // End of Frame symbol

/* Internal variables */
/* Used as part of the UART emulation */
static unsigned char UARTRxBuffer[UART_RX_SIZE];
static unsigned char rxBufLen = 0; 

static unsigned char UARTTxBuffer[UART_TX_SIZE];
static unsigned char txBufLen = 0; 



 
/* Function implementation */


/* 
 * cmdProcessor
 */ 
int cmdProcessor(void)
{
    // Process command if a complete command is received
    if (rxChar(0) == 1) {
        // Extract command and data
        unsigned char cmd = UARTRxBuffer[1];
        unsigned char data[UART_RX_SIZE - 6]; // Subtracting 6 for SOF, CMD, DATA, CS, and EOF
        int dataLen = rxBufLen - 6; // Subtracting 6 for SOF, CMD, DATA, CS, and EOF

        memcpy(data, &UARTRxBuffer[2], dataLen);

        // Process command here
        switch (cmd) {
            case 'P':
                // Process command P
                // Example: assuming data represents temperature
                printf("Temperature: %.*s\n", dataLen, data);
                break;
                
            case 'A':
            	// Process command A
            	// 
            	
    	    case 'L':
    	    	// Process command L (return the 20 last samples of each variable)
    	    	
    	    case 'R':
    	    	// Resets the history
    	    	    
            // Add cases for other commands as needed
            default:
                // Unknown command
                break;
        }
        
        resetRxBuffer(); // Reset buffer
        return 1; // Signal that a command has been processed
    } else if (rxChar(0) == -2) {
        // Incorrect checksum
        return -2; // Signal incorrect checksum
    }

    return 0; // Signal that no command has been processed
}

/*
 * rxChar
 */
/* 
 * rxChar
 */
int rxChar(unsigned char car)
{
    static int receivingCommand = 0;
    static int dataLength = 0;
    static int checksum = 0;
    static unsigned char cmd;

    if (car == SOF_SYM) {
        resetRxBuffer(); // Reset buffer when starting a new command
        receivingCommand = 1;
        checksum = 0;
        dataLength = 0;
    }

    if (receivingCommand) {
        if (rxBufLen < UART_RX_SIZE) {
            UARTRxBuffer[rxBufLen] = car;
            rxBufLen += 1;
        } else {
            // Buffer overflow handling
            resetRxBuffer(); // Reset buffer on overflow
            receivingCommand = 0; // Reset receiving flag
            return -1;
        }

        if (car != SOF_SYM && car != EOF_SYM && car != CMD_SYM && car != CS_SYM) {
            // Accumulate checksum
            checksum += car;
            // Increment data length
            dataLength++;
        }

        if (car == EOF_SYM) {
            receivingCommand = 0; // Reset receiving flag

            // Verify checksum
            if ((checksum % 256) == UARTRxBuffer[rxBufLen - 2]) {
                // Valid command received
                return 1; // Signal that a complete command has been received
            } else {
                // Invalid checksum, discard command
                resetRxBuffer(); // Reset buffer
                return -2; // Signal incorrect checksum
            }
        }
    }

    return 0; // Signal that a character has been received
}

/*
 * txChar
 */
int txChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (txBufLen < UART_TX_SIZE) {
		UARTTxBuffer[txBufLen] = car;
		txBufLen += 1;
		return 0;		
	}	
	/* If cmd string full return error */
	return -1;
}

/*
 * resetRxBuffer
 */
void resetRxBuffer(void)
{
	rxBufLen = 0;		
	return;
}

/*
 * resetTxBuffer
 */
void resetTxBuffer(void)
{
	txBufLen = 0;		
	return;
}

/*
 * getTxBuffer
 */
void getTxBuffer(unsigned char * buf, int * len)
{
	*len = txBufLen;
	if(txBufLen > 0) {
		memcpy(buf,UARTTxBuffer,*len);
	}		
	return;
}



