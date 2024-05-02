#ifndef GPS
#define GPS


#define PI 3.141592653589793238462643383279502884197
#define Earth_Radius  	6371000

void GPS_read(float *Lat ,float *Long , float *Speed);

float Get_Distance(float startLatitude, float startLongitude,float endLatitude, float endLongitude);

float ToDegree (float angle);

#endif	
