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

int readTemperatur(void)
{
	int temperature;
	temperature = snesor_read; // como fazemos para a emulação da UART
	return temperature
}

/* 
 * cmdProcessor
 */ 
int cmdProcessor(void)
{
	int i;
	unsigned char cmd, cs;
	unsigned int dataLen = 0;
	unsigned int checksum = 0;
		
	/* Detect empty cmd string */
	if(rxBufLen == 0)
		return -1; 
	
	/* Find index of SOF */
	for(i=0; i < rxBufLen; i++) {
		if(UARTRxBuffer[i] == SOF_SYM) {
			break;
		}
	}
	
	/* If a SOF was found look for commands */
	if(i < rxBufLen) {
		/* Check if there are enough bytes for a complete command */
		if (i + 4 >= rxBufLen)
			return -1; // Incomplete command
			
		/* Check for EOF symbol */
		if (UARTRxBuffer[i+4] != EOF_SYM)
			return -1; // Incomplete command
		
		/* Extract CMD */
		cmd = UARTRxBuffer[i+1];
		
		/* Extract DATA length */
		dataLen = rxBufLen - i - 4;
		
		/* Calculate checksum */
		cs = 0;
		for (int j = i + 1; j < i + 1 + dataLen; j++) {
			checksum += UARTRxBuffer[j];
		}
		cs = (unsigned char)(checksum % 256);
		
		/* Check if calculated checksum matches received checksum */
		if (cs != UARTRxBuffer[i+1+dataLen])
			return -2; // Incorrect checksum
		
		/* Process command */
		switch(cmd) { 
			case 'P':		
				/* Command "P" detected.							*/
				/* Follows one DATA byte that specifies the sensor	*/ 
				/* to read. I assume 't','h','c' for temp., humid. 	*/
				/* and CO2, resp.									*/   
				if (dataLen != 1)
					return -1; // Incorrect data length
					
				/* Process sensor reading */
				unsigned char sensorType = UARTRxBuffer[i+2];
				switch(sensorType) {
					case 't':
						// Code to read temperature sensor
						// Assuming a temperature of +25°C for now
						txChar('+');
						txChar('2');
						txChar('5');
						break;
					case 'h':
						// Code to read humidity sensor
						// Placeholder, actual implementation required
						break;
					case 'c':
						// Code to read CO2 sensor
						// Placeholder, actual implementation required
						break;
					default:
						return -1; // Invalid sensor type
				}
				break;
			default:
				/* If code reaches this place, the command is not recognized */
				return -1;				
		}
		
		/* Here you should remove the characters that are part of the command from the RX buffer */
		/* For simplicity, I am resetting it, but it's not a good solution */
		rxBufLen = 0;	
		
		return 0;
	}
	
	/* Cmd string not null and SOF not found */
	return -1;

}

/*
 * rxChar
 */
int rxChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (rxBufLen < UART_RX_SIZE) {
		UARTRxBuffer[rxBufLen] = car;
		rxBufLen += 1;
		return 0;		
	}	
	/* If cmd string full return error */
	return -1;
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
