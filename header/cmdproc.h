/* ******************************************************/
/* SETR 23/24, Paulo Pedreiras                          */
/*	Base code for Unit Testing                          */
/*  	Simple example of command processor             */
/*    	for smart sensor node with 3 sensors			*/
/*     													*/
/*	Code is just for illustrative effects. E.g. error 	*/ 
/*		codes are "magic numbers" in the middle of the	*/
/*    	code instead of being (defined) text literals, 	*/
/* 		sensor data is not properly emulated, missing 	*/
/* 		commands, Checksum not implemented, ...			*/
/*														*/
/* ******************************************************/

#ifndef CMD_PROC_H_
#define CMD_PROC_H_

/* Some defines */
/* Other defines should be return codes of the functions */
/* E.g. #define CMD_EMPTY_STRING -1                      */
#define UART_RX_SIZE 20 	/* Maximum size of the RX buffer */ 
#define UART_TX_SIZE 20 	/* Maximum size of the TX buffer */ 
#define SOF_SYM '#'	        /* Start of Frame Symbol */
#define EOF_SYM '!'          /* End of Frame Symbol */

/* Function prototypes */

/* ************************************************************ */
/* Processes the chars in the RX buffer looking for commands 	*/
/* Returns:                                                     */
/*  	 0: if a valid command was found and executed           */
/* 		-1: if empty string or incomplete command found         */
/* 		-2: if an invalid command was found                     */
/* 		-3: if a CS error is detected (command not executed)    */
/* 		-4: if string format is wrong                           */
/* ************************************************************ */
int cmdProcessor(void);

/* ******************************** */
/* Adds a char to the RX buffer 	*/
/* I.e., the reception of commands 	*/
/* Returns: 				        */
/*  	 0: if success 		        */
/* 		-1: if buffer full	 	    */
/* ******************************** */
int rxChar(unsigned char car);

/* ************************************ */
/* Adds a char to the TX buffer 		*/
/* I.e., the tranmsisison of answers 	*/
/* Returns: 				        	*/
/*  	 0: if success 		        	*/
/* 		-1: if buffer full	 	    	*/
/* ************************************ */
int txChar(unsigned char car);

/* ************************* */
/* Resets the RX buffer		 */  
/* ************************* */
void resetRxBuffer(void);

/* ************************* */
/* Resets the TX buffer		 */  
/* ************************* */
void resetTxBuffer(void);

/* ************************************************ */
/* Return the data that would be sent by the sensor */  
/* ************************************************ */
void getTxBuffer(unsigned char * buf, int * len);

/* ************************************************ */
/* Computes the checksum of a given number of chars */
/* ************************************************ */ 
int calcChecksum(unsigned char * buf, int nbytes);

#endif
