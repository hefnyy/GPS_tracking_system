#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <string.h>
#include "UART.h"
#include "Bit_Utilies.h"

#define CB 0x2A								//HexaDecimal for (*)

void UART0_init(void){
	SYSCTL_RCGCUART_R |=0x0001; //Activate UART0 
	SYSCTL_RCGCGPIO_R |=0x0001; //Activate PortA
	while(GET_BIT(SYSCTL_PRGPIO_R,0)==0 );	
	UART0_CTL_R &= ~(0x0001);	//Disable UART0
	UART0_IBRD_R = 0x68; //IBRD=int(160000000/(16*9600)) = 104;
	UART0_FBRD_R = 0xB;	//FBRD = int(0.166*64 +0.5) = 11
	UART0_LCRH_R = 0x0070;	// 8-bit word length, enable FIFO 001110000
	UART0_CTL_R = 0x0301; //enable RXE , TXE and UART 001100000001
	GPIO_PORTA_AFSEL_R |= 0x03; //enable alt function PA0, PA1
	GPIO_PORTA_PCTL_R = ( GPIO_PORTA_PCTL_R & 0xFFFFFF00 ) + 0x00000011; //configure UART for PA0,PA1
	GPIO_PORTA_DEN_R |= 0x03;	//enable digital I/O on PA0, PA1
	GPIO_PORTA_AMSEL_R &= ~0x03;	//disable analog function on PA0, PA1
}

char UART0_getChar() {
	while((UART0_FR_R & 0x10) !=0);
return (char) UART0_DR_R;
}

void UART0_OutChar(char data){
		while((UART0_FR_R & 0x20)!=0);
	UART0_DR_R=data;
}

void UART0_OutString(char *pt){
	while(*pt){
		UART0_OutChar(*pt);
		pt++;
	}
}
void GetCommand(char *Command,int len ){ 		
		char character;
		int i;
		for(i=0 ; i< len; i++){
			character = UART0_getChar();
			if(character!=CB)
			{
				Command[i]=character;
				UART0_OutChar(character);
			}
			else if(character==CB ||i==len)
				break;		
		}
}

