#include "DS18B20.h"

/*Address of the devices*/
int add_device1[8]={0x28,0x11,0x1b,0xDE,0x07,0x00,0x00,0x10};
int add_device2[8]={0x28,0x9f,0x48,0xde,0x07,0x00,0x00,0x32};
int add_device3[8]={0x28,0x04,0xbe,0xde,0x07,0x00,0x00,0x41};
int add_device4[8]={0x28,0x43,0xfa,0xde,0x07,0x00,0x00,0x47};
int add_device5[8]={0x28,0x92,0xcc,0xde,0x07,0x00,0x00,0xac};
double avg_temp=0;

int main()
{
	unsigned int time1, time2, time3, time4, time5, time6;
	wiringPiSetup();											//Setup function of wiringPi
	pinMode(0,OUTPUT);											//defining 0th pin as output
	pinMode(7,INPUT);											//defining 7th pin as input (for data)
	time6=millis();
	time1=millis();
	initialize();
	time2=millis();
	printf("Initialize Time:%d\n",time2-time1);
	time1=millis();
	configure(0x7F,0x80,0x7F,add_device1);						//configure the sensor
	time3=millis();
	printf("Configure Time=%d\n",time3-time1);
	time4=millis();
	address(add_device1);										//address the device and convert the temperature
	time5=millis();
	printf("Temperature Conversion Time=%d\n",time5-time4);
	configure(0x7F,0x80,0x7F,add_device2);
	address(add_device2);
	configure(0x7F,0x80,0x7F,add_device3);
	address(add_device3);
	configure(0x7F,0x80,0x7F,add_device4);
	address(add_device4);
    configure(0x7F,0x80,0x7F,add_device5);
	address(add_device5);
	time1=millis();
	printf("Average Temperature=%f",avg_temp/5);
	printf("\nTime elapsed=%d\n",time1-time6);

	return 0;
}
