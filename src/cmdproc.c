/* ****************************** */
/* See cmdProc.h for indications  */
/* ****************************** */
// Author: Hugo Alcaire Verde
// Date : 31 March 2024
#include <stdio.h>
#include <string.h>

#include "cmdproc.h"

/* Internal variables */
/* Used as part of the UART emulation */
static unsigned char UARTRxBuffer[UART_RX_SIZE];
static unsigned char rxBufLen = 0; 

static unsigned char UARTTxBuffer[UART_TX_SIZE];
static unsigned char txBufLen = 0; 

static int sensor_temp [] = {-50, -47, -40, -33, -30, -22, -20, -11, -10, -9, 0, 6, 10, 13, 20, 27, 30, 35, 40, 48}; 

static int sensor_humidity [] = {0, 5, 8, 12, 15, 20, 22, 30, 33, 35, 45, 49,  57, 61, 73, 77, 81, 88, 99, 100 };

static int sensor_CO2[] = {1550, 4545,5146, 5526, 7009, 10009, 10589, 10964, 11081, 11131, 12161, 13088, 14904 15469, 15554, 16024, 16160, 16510, 16651, 18761};
 
/* Function implementation */

/* 
 * cmdProcessor
 */ 
int cmdProcessor(void)
{
	int i;
	unsigned char sid;
		
	/* Detect empty cmd string */
	if(rxBufLen == 0)
		return -1; 
	
	/* Find index of SOF */
	for(i=0; i < rxBufLen; i++) 
	{
		if(UARTRxBuffer[i] == SOF_SYM) {
			break;
		}
	}
	
	/* If a SOF was found look for commands */
	if(i < rxBufLen) 
	{
		
		switch(UARTRxBuffer[i+1]) { 
			
			case 'P':		
				/* Command "P" detected.							*/
				/* Follows one DATA byte that specifies the sensor	*/ 
				/* to read. I assume 't','h','c' for temp., humid. 	*/
				/* and CO2, resp.									*/   
		
				/* Check sensor type */
				sid = UARTRxBuffer[i+2];
				if(sid != 't' && sid != 'h' && sid != 'c') {
					return -2;
				}
				
				if(sid == 't')
				{
				
					//implement to read the position zero of array temperature
					
					
					txChar(sensor_temp[0]); 
				}
				
				if(sid == 'h')
				{
					// implement to read the position zero of array humidity 
					
					
					txChar(sensor_humidity[0]);
				
				}
				
				if(sid == 'c')
				{
				
					// implement to read the position zero of array co2
					
					
					txChar(sensor_CO2[0]);
				}
				break;
				
			case 'A':
			
				// implement to read the position zero value of all the arrays
				
					
					txChar(sensor_temp[0]);
					txChar(sensor_humidity[0]);
					txChar(sensor_CO2[0]);
					
					
				break;
				
			case 'L':
			
				// implement to read all the 20 positions of the 3 arrays !
				
			
				
				for(int i = 0; i<20; i++)
				{
					txChar(sensor_temp[i]);
					txChar(sensor_humidity[i]);
					txChar(sensor_CO2[i]);
					
					// Como implementar ? Dar reset depois de armazernar cada variavel?
					
				}
				
				break;
				
			case 'R':
				
				// implement to reset everything. Reset array valuers or just start the program from the beggining?
				
				resettRxBuffer();
				resetTxBuffer();
				break;
				
				
				/* Check checksum */
				if(!(calcChecksum(&(UARTRxBuffer[i+1]),2))) {
					return -3;
				}
				
				/* Check EOF */
				if(UARTRxBuffer[i+6] != EOF_SYM) {
					return -4;
				}
			
				/* Command is (is it? ... ) valid. Produce answer and terminate */ 
				txChar('#');
				txChar('p'); /* p is the reply to P 							*/	
				txChar('t'); /* t indicate that it is a temperature 			*/
				txChar('+'); /* This is the sensor reading. You should call a 	*/
				txChar('2'); /*   function that emulates the reading of a 		*/
				txChar('1'); /*   sensor value 	*/
				txChar('1'); /* Checksum is 114 decimal in this case		*/
				txChar('1'); /*   You should call a funcion that computes 	*/
				txChar('4'); /*   the checksum for any command 				*/  
				txChar('!');
				
				/* Here you should remove the characters that are part of the 		*/
				/* command from the RX buffer. I'm just resetting it, which is not 	*/
				/* a good solution, as a new command could be in progress and		*/
				/* resetting  will generate errors									*/
				rxBufLen = 0;	
				
				return 0;
								
			default:
				/* If code reaches this place, the command is not recognized */
				return -2;				
		}
		
		
	}
	
	/* Cmd string not null and SOF not found */
	return -4;

}

/* 
 * calcChecksum
 */ 
int calcChecksum(unsigned char * buf, int nbytes) {
	
	int checksum = 0;
    	for (int i = 0; i < nbytes; i++) 
    	{
        	checksum += buf[i];
    	}
    		checksum %= 256; // Modulo 256
    		return checksum;

}

/*
 * rxChar
 */
int rxChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (rxBufLen < UART_RX_SIZE) 
	{
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
	if (txBufLen < UART_TX_SIZE) 
	{
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
	if(txBufLen > 0) 
	{
		memcpy(buf,UARTTxBuffer,*len);
	}		
	return;
}


