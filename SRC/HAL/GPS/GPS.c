#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "GPS.h"


const int len=80;
char gps[len]={0};


char *token;

char GPS_logname[]="$GPRMC,";
char delim[]="," ;

int j=-1;
char i=0;
int flag=1;




void GPS_read(float *Lat ,float *Long , float *Speed){

    /// Check if the log name is correct ///
    again:
    do{
        GetCommand_UART1(gps , len);
        flag=1;

        for(i=0;i<7;i++){
            if(gps[i]!=GPS_logname[i]){
                strcpy(gps , "");
                flag=0;
                break;
            }
        }

    }while(flag==0);

    for(token = strtok(gps , delim) ; token ; token = strtok(NULL , delim) ){
        j++;

        if(j==0 || j==1){
            continue;
            //"$GPRMC,203522.00,A,30.06416667,N,31.28055556,W,0.004,133.4,130522,0.0,E,D*2B"
        }																							// 	0				1				2				3			4				5			6		7			8			9			10
        else if(j==2 && *token != 'A'){
            j=-1;
            strcpy(gps,"");
            goto again;
        }

        else if(j==3){
            *Lat= atof(token);

        }
        else if(j==4){
            if(*token == 'N')
                *Lat = (*Lat);
            else if(*token != 'N')
                *Lat = -(*Lat);
        }

        else if(j==5){
            *Long= atof(token);
        }

        else if(j==6){
            if(*token == 'E')
                *Lat = (*Long);
            else if(*token != 'E')
                *Lat = -(*Long);

        }
        else if(j==7){
            *Speed= atof(token);
            strcpy(gps,"");
            j=-1;
            break;

        }
    }
}

/// ************************************************************************************************************************************* ///

float Get_Distance(float startLatitude, float startLongitude,float endLatitude, float endLongitude)
{
		//UART_OutString("Distance\n");
    // convert all coordinates from Degrees into Radians
    float startLat = ToDegree(startLatitude)* PI/180;
    float startLong = ToDegree(startLongitude)* PI/180;
    float endLat = ToDegree(endLatitude)* PI/180;
    float endLong = ToDegree(endLongitude)* PI/180;

    // calculate latitude difference and longitude difference
    float latDifference = endLat - startLat;
    float longDifference = endLong - startLong;
			
		
    //use Haversine formula
    float a = sin(latDifference / 2) * sin(latDifference / 2) + cos(startLat) * cos(endLat)
    * sin(longDifference / 2) * sin(longDifference / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
				
    //Multipy by Earth's Radius to get the distance
    return c * Earth_Radius;
}

float ToDegree (float angle){
	int degree = (int)angle/100 ; 
	float minutes = angle -(float)degree*100;
	return (degree + (minutes/60) );
}

void ToDegree_modified (float *angle){
	int degree = (int)(*angle)/100 ; 
	float minutes = *angle -(float)degree*100;
			*angle=degree + (minutes/60); 
}

