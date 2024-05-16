#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "Bit_Utilies.h"
#include "GPIO.h"
#include "GPS.h"
#include "UART.h"
#include "SysTick.h"
#include "LCD.h"

#define CR 0x0D					// 0x0D == Enter
#define CB 0x2A					// 0x2A == '*'



float total_Distance=0.0;
char dis[10]={0};


/// ************************************************************************************************************************************* /// 
 
	int main() {
		

		float receivedLat = 0 , receivedLong = 0 , Speed = 0 , oldLat = 0 , oldLong = 0 , Temp = 0 ;
		
		char tempp[10];
		char Lat[301]={0};				//trajectory	// 301 = 150(lat)+150(long)+1(newline char =="\n")
		char Lon[150]={0};				//trajectory
		char lat[15]={0};					// array to convert the float value of lat to char of array
		char lon[15]={0};					// array to convert the float value of lon to char of array
		
///***** Useful Counters *****///		
		int b;		
		int t;
		int k=0;									
		int l=0;									
		char c;
		
///***** Initializations *****///		
		GPIO_Init_PortF();
		UART0_Init();
		UART1_Init();
		SysTick_Init();
		LCD_Init();
		
		GPIO_PORTF_DATA_R |= 0x04;		
		LCD_Cmd(0x80);
		LCD_String_modified("Welcome ");
		
		SysTick_Wait10ms(300);

	while(1){
				
		SysTick_Wait10ms(200);
		GPIO_PORTF_DATA_R &= ~0x04;
		
		while(total_Distance<100){
				
				GPIO_PORTF_DATA_R |= 0x08;
				
				GPS_read(&receivedLat , &receivedLong , &Speed);
				
				if(Speed >= 1){				
					GPIO_PORTF_DATA_R |= 0x04;
					ToDegree_modified(&receivedLat);
					ToDegree_modified(&receivedLong);
					sprintf(lat,"%f",receivedLat);
					sprintf(lon,"%f",receivedLong);
					for(b = 0; b < 1; b++){
						append_with_comma(Lat, 301, lat);
						append_with_comma(Lon, 150, lon);
					}
			if(oldLat != 0 && oldLong != 0){
					Temp = Get_Distance_modified(receivedLat , receivedLong , oldLat , oldLong);
					total_Distance = total_Distance + Temp ;
				}
					sprintf(dis ,"%.3f",total_Distance);
					LCD_Cmd(0x01);
					LCD_Cmd(0x80);
					LCD_String_modified("Total Distance = ");
					LCD_Cmd(0xC0);
					LCD_String_modified(dis);
					oldLong = receivedLong;
					oldLat = receivedLat;	
			}
				GPIO_PORTF_DATA_R &= ~0x04;
		}
		
		///***** Exceeded 100 meters *****///
		GPIO_PORTF_DATA_R &= ~0x08;
		GPIO_PORTF_DATA_R |= 0x02;
		///***** Concatenate the lat and long array to send them to trajectory mapping *****///
		strcat(Lat,",");
		strcat(Lat,Lon);
		strcat(Lat,"\n");
		
		LCD_Cmd(0x01);
		LCD_Cmd(0x80);
		LCD_String_modified("Final Dest.");
		Send_Draw_modified(Lat);
		
//		for(t=0 ; t<4 ; t++){
//			Send_Draw();
//		}
	}		
    return 0;
}


/// ************************************************************************************************************************************* ///
