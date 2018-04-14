#include "DS18B20.h"

/*Address of the devices*/
int add_device1[8]={0x28,0x11,0x1b,0xde,0x07,0x00,0x00,0x10};
int add_device2[8]={0x28,0x9f,0x48,0xde,0x07,0x00,0x00,0x32};
int add_device3[8]={0x28,0x04,0xbe,0xde,0x07,0x00,0x00,0x41};
int add_device4[8]={0x28,0x43,0xfa,0xde,0x07,0x00,0x00,0x47};
int add_device5[8]={0x28,0x92,0xcc,0xde,0x07,0x00,0x00,0xac};
double avg_temp=0;
pthread_t ps[5];
pthread_mutex_t lock;
unsigned long difference=0;
unsigned long long js, es=0;

int main()
{
	unsigned int time1,time2,time3,time4,time5,time6;
	int i=0,error;
	wiringPiSetup();											//Setup function of wiringPi
	pinMode(0,OUTPUT);											//defining 0th pin as output
	pinMode(7,INPUT);											//defining 7th pin as input (for data)
	for(i=0;i<5;i++)
	{
		error=pthread_create(&(ps[i]),NULL,&sensor1,NULL);
		if(error!=0)
		{
			printf("Error creating thread\n");
		}
	}
	pthread_join(ps[0],NULL);
	pthread_join(ps[1],NULL);
	pthread_join(ps[2],NULL);
	pthread_join(ps[3],NULL);
	pthread_join(ps[4],NULL);
	pthread_mutex_destroy(&lock);
	printf("Average Temperature=%f\n",avg_temp/5);
	printf("Time elapsed=%lu\n",difference);

	return 0;
}
