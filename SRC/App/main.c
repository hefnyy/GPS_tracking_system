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

#define CB 0x2A									//Hexadecimal for (*)

float total_Distance=0.0;

/// ************************************************************************************************************************************* /// 
 
	int main() {
		
		float receivedLat = 0 , receivedLong = 0 , Speed = 0 , oldLat = 0 , oldLong = 0 , Temp = 0 ;		//initialization of variables
		
		float Lon[50]={0};						//Array of float to save Longitude points in EEPROM to plot them on map;
		float Lat[50]={0};						//Array of float to save Latitude points in EEPROM to plot them on map;
		int k=0;							//Counter for Lat[]
		int l=0;							//Counter for Lon[]

		UART0_init();					//initialize UART0 for PC communication
		GPIO_Init_PortF();				//initialize Port F for LEDs & Switches
		UART1_Init();					//initialize UART0 for GPS communication
		
		

		
	while(1){
		
		while(total_Distance<100){
			
			GPIO_SetLedValue(GPIO_GREEN_LED , GPIO_LED_OFF);				//make sure that Green & Blue LEDs are turned Off
			GPIO_SetLedValue(GPIO_BLUE_LED , GPIO_LED_OFF);
			GPIO_SetLedValue(GPIO_RED_LED , GPIO_LED_ON);					//TurnOn RedLED
			
			GPS_read(&receivedLat , &receivedLong , &Speed);
			
		if(oldLat != 0 && oldLong != 0){
			Temp = Get_Distance(receivedLat , receivedLong , oldLat , oldLong);		//Calculate distance between current point and previous point 
			total_Distance = total_Distance + Temp ;								//Cumulate total distance
		}
		
		oldLong = receivedLong;						//Update oldlong point
		oldLat = receivedLat;						//Update oldlat point
		Lon[l++] = oldLong;							//Assign values for trajectory (Longtitude Cooredinates)
		Lat[k++] = oldLat;							//Assign Values for trajectory (Latitude Cooredinates)
		}
		
		GPIO_SetLedValue(GPIO_RED_LED , GPIO_LED_OFF);								//when reach the destination (total Distance > 100) turnOff RedLED and turnOn Blue & Green LEDs
		GPIO_SetLedValue(GPIO_BLUE_LED , GPIO_LED_ON);
		GPIO_SetLedValue(GPIO_GREEN_LED , GPIO_LED_ON);
		}
	

    return 0;
}


/// ************************************************************************************************************************************* ///
