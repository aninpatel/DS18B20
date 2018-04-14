#include "DS18B20.h"

void initialize(void)                   //Sending reset pulse from MASTER
{
    int a=5;
    digitalWrite(0,LOW);	            //pulling the Data line down
    delayMicroseconds(480);	            //delay for 480 microseconds
    digitalWrite(0,HIGH);               //pulling the data line HIGH
    while((a=digitalRead(7)));
    delayMicroseconds(480);
}

void writerom(int r)
{
    int mask=0x01;	                     //mask with value 1 to get lsb
    int i;
    int bit[8];
    for(i=0;i<8;i++)
    {
        bit[i]=r&mask;                   //extracting a lsb of data
        r=r>>1;		                     //shift the data to right by 1 bit
    }
    for(i=0;i<8;i++)	                 //writing data to sensor
    {
        delayMicroseconds(1);
        digitalWrite(0,LOW);
        if(bit[i]==1)
        {
            delayMicroseconds(1);
            digitalWrite(0,HIGH);
        }
        delayMicroseconds(60);
        digitalWrite(0,HIGH);
    }
}

void readrom(int count)
{
    int i;
    int *ptr;
    ptr=(int *)malloc(count*sizeof(int));
    if(ptr==NULL)
    {
        printf("Couldn't allocate memory");
        printf("Terminating the progarm");
        exit(0);
    }
    for(i=0;i<count;i++)
    {
        digitalWrite(0,LOW);
        delayMicroseconds(1);
        digitalWrite(0,HIGH);
        delayMicroseconds(10);
        *(ptr+i)=digitalRead(7);	        //reading the data from sesor and storing it in a buffer
        delayMicroseconds(49);
    }
    bin2hex(ptr,count);	                    //converting the data into hex format
}

void bin2hex(int *p,int count)
{
    int i,j,z,k=0,test;
    int *data;
    double value=0,test1;
    data=(int *)malloc(count*sizeof(int));
    if(data==NULL)
    {
        printf("Couldn't allocate the memory");
        printf("Terminating the program");
        exit(0);
    }
    for(i=0;i<count/4;i++)
    {
        j=0;
        z=0;
        while(j<=3)
        {
            test=pow(2,j);
            z+=test*(*(p+k));
            k++;
            j++;
        }
        *(data+i)=z;
    }
    for(i=i-1;i>=0;i--)
    {
        printf("%0x",*(data+i));
    }
    printf("\n");
    for(i=0;i<11;i++)
    {
        test1=pow(2,i-4);
        value+=test1*(*(p+i));
    }
    if(*(p+i))
    {
        value=value*-1;
    }
    avg_temp+=value;
    printf("Temperature Value=%f\n",value);
}

void configure(int Th,int Tl,int resolution,int *p)
{
    int i;
    clock_t t1,t2;
    t1=clock();
    initialize();
    writerom(0x55);
    for(i=0;i<8;i++)
    {
        writerom(*(p+i));
    }
    writerom(0x4E);
    writerom(Th);	                                  //writing data to Th register
    writerom(Tl);	                                  //writing data to Tl register
    writerom(resolution);	                          //setting a resolution of the sensor
    t2=clock();
    printf("In Configure:%d\n",t2-t1);
}

void address(int *p)
{
    int i;
    long int t1,t2;
    t1=millis();
    initialize();
    writerom(0x55);		                               //Match ROM Command
    for(i=0;i<8;i++)
    {
        writerom(*(p+i));
    }
    writerom(0x44);                                    //Temperature conversion commanf)
    delay(750);
    initialize();
    writerom(0x55);
    for(i=0;i<8;i++)
    {
        writerom(*(p+i));
    }
    writerom(0xBE);
    readrom(72);
    t2=millis();
    printf("In Address:%d",t2-t1);
}

void *sensor(void *arg)
{
	pthread_mutex_lock(&lock);
	unsigned int time1=0,time2=0,end_time;
	static int count=0;
	js=micros();
	count++;
	printf("Sensor %d is being operated\n",count);
	time1=millis();
	if(count==1)
	{
		printf("Context Switch Time:%llu\n",js-js);
		configure(0x7F,0x80,0x7F,add_device1);
		address(add_device1);
		time2=millis();
	}
	else if(count==2)
	{
		printf("Context Switch Time:%llu\n",js-es);
		configure(0x7F,0x80,0x7F,add_device2);
		address(add_device2);
		time2=millis();
	}
	else if(count==3)
	{
		printf("Context Switch Time:%llu\n",js-es);
		configure(0x7F,0x80,0x7F,add_device3);
		address(add_device3);
		time2=millis();
	}
	else if(count==4)
	{
		printf("Context Switch Time%llu\n",js-es);
		configure(0x7F,0x80,0x7F,add_device4);
		address(add_device4);
		time2=millis();
	}
	else
	{
		printf("Context Switch Time%llu\n",js-es);
		configure(0x7F,0x80,0x7F,add_device5);
		address(add_device5);
		time2=millis();
	}
	difference+=time2-time1;
	end_time=millis();
	es=micros();
	if(count==5)
	{
		printf("Time elapsed\n",difference);
	}
	pthread_mutex_unlock(&lock);
	return 0;
}
