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

#define CR 0x0D
#define CB 0x2A

float total_Distance=0.0;

/// ************************************************************************************************************************************* /// 
 
	int main() {
		
		float receivedLat = 0 , receivedLong = 0 , Speed = 0 , oldLat = 0 , oldLong = 0 , Temp = 0 ;
		
		float Lon[100]={0};				//EEPROM
		float Lat[100]={0};				////EEPROM
		int k=0;									//counter
		int l=0;									//counter

		UART0_init();
		GPIO_Init_PortF();
		UART1_Init();
		
		

		
	while(1){
		
		while(total_Distance<100){
			
			GPIO_SetLedValue(GPIO_GREEN_LED , GPIO_LED_OFF);
			GPIO_SetLedValue(GPIO_BLUE_LED , GPIO_LED_OFF);
			GPIO_SetLedValue(GPIO_RED_LED , GPIO_LED_ON);
			
			GPS_read(&receivedLat , &receivedLong , &Speed);
			
		if(oldLat != 0 && oldLong != 0){
			Temp = Get_Distance(receivedLat , receivedLong , oldLat , oldLong);
			total_Distance = total_Distance + Temp ;
		}
		
		oldLong = receivedLong;
		oldLat = receivedLat;
		Lon[l++] = oldLong;							//EEPROM for trajectory (Longtitude Cooredinates)
		Lat[k++] = oldLat;							//EEPROM for trajectory (Latitude Cooredinates)
		}
		
		GPIO_SetLedValue(GPIO_RED_LED , GPIO_LED_OFF);
		GPIO_SetLedValue(GPIO_BLUE_LED , GPIO_LED_ON);
		GPIO_SetLedValue(GPIO_GREEN_LED , GPIO_LED_ON);
		}
	

    return 0;
}


/// ************************************************************************************************************************************* ///
