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
void GetCommand_UART0(char *Command,int len ){ 		
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


void UART1_Init (void){// Should be called only once
	SET_BIT(SYSCTL_RCGCUART_R , 1); // Activate UART1
	SET_BIT(SYSCTL_RCGCGPIO_R,1); // Activate Port B 
	while(GET_BIT(SYSCTL_PRGPIO_R , 1)==0); //Check on the clock
	CLR_BIT(UART1_CTL_R , 0); // Disable UART1
	UART1_IBRD_R = 0x68; // IBRD=int (160000000/ (16*9600) ) = 104
	UART1_FBRD_R = 0xB; // FBRD = int (0.166 * 69 + 0.5)=11
	UART1_LCRH_R = 0x0070; // 8-bit word length, enable FIFO 001110000
	UART1_CTL_R = 0x0301; // Enable RE, IXE and UART 001100000001
	GPIO_PORTB_AFSEL_R |= 0x03; // Enable alt function PBO , PB1  
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFFFF00) +0x00000011; // Configure UART on PB0 , PB1
	GPIO_PORTB_DEN_R |= 0x03; // Enable digital I/0 on PBO , PB1 
	GPIO_PORTB_AMSEL_R &= ~0x03; // Disable analog function on PBO , PB1
}

void UART1_OutChar(char data){
		while((UART1_FR_R & 0x20)!=0);  //check if transmitter is full or not
	UART1_DR_R=data;
}
char UART1_getChar() {
	while((UART1_FR_R & 0x10) !=0);
return (char) UART1_DR_R;
}

void GetCommand_UART1(char *Command,int len ){ 
		
		char character_UART1;
		int p;		
		for(p=0 ; p< len; p++){
			character_UART1 = UART1_getChar();
			if(character_UART1!=CB)
			{
				Command[p]=character_UART1;
			}
			else if(character_UART1==CB)
				break;		
		}
}
void UART1_OutString(char *pt){
	while(*pt){
		UART1_OutChar(*pt);
		pt++;
	}
}

//for testing trajectory
char Point[]={"  30.0657606,30.0657641,30.0657586,30.0657702,30.0658360,30.0659112,30.0661776,30.0662298,30.0662153,31.4792633,31.4793548,31.4794423,31.4795000,31.4794967,31.4794953,31.4794742,31.4793639,31.4792304\n"};

void Send_Draw(){
	char flag=0;
		while(1){	
			if(UART0_getChar()=='U'){
				flag=1;
				break;
			}
				
		}
			if(flag){
				UART0_OutString(Point);
			}
}

void Send_Draw_modified(char *Map_array){
	char flag=0;
		while(1){		
			if(UART0_getChar()=='U'){
				flag=1;
				break;
			}
		}
			if(flag){
				UART0_OutString(Map_array);
			}
}

void append_with_comma(char *big_array, int big_array_size,  char *small_array) {
    // Calculate the current length of the big array
    int current_length = strlen(big_array);

    // Calculate the needed length: current length + length of small array + 1 for comma + 1 for \n
    int needed_length = current_length + strlen(small_array) + 1 + 1;

    // Check if there is enough space in the big array
    if(needed_length > big_array_size) {
        return;
    }

    // Append a comma if the big array is not empty
    if (current_length > 0) {
        strcat(big_array, ",");
    }

    // Append the small array to the big array
    strcat(big_array, small_array);
}




