#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/mutex.h>


#include "DS18B20.h"

static void initialize(void)   //initialize function for Initializing the Sensor
{
	gpio_set_value(WRITE,0);
	udelay(480);
	gpio_set_value(WRITE,1);
	udelay(480);
}
static void writerom(int command)  //Function to write data (which passed as an argument) into sensor
{
	int mask=0x01;			//defining the mask
	int i=0,bit[8];			//array to store bits
	for(i=0;i<8;i++)
	{
		bit[i]=(command)&(mask);   //extracting Least Significant Bit from Command Variable
		command=command>>1;		   //Right Shift the command variable to extract the next bit
	}
	for(i=0;i<8;i++)    //Writing Each bit extracted from Command to Sensor starting from lsb
	{
		udelay(1);
		gpio_set_value(WRITE,0);
		if(bit[i]==1)
		{
			udelay(1);
			gpio_set_value(WRITE,1);
		}
		udelay(60);
		gpio_set_value(WRITE,1);
	}
}
static void readrom(int b)    //Function to read data from sensor with number of bits to be read as an argument
{
	int i=0;
	int *p=(int *)kmalloc(b*sizeof(int),GFP_KERNEL);   //Allocating memory for the number of bits to be read
	for(i=0;i<72;i++)
	{
		*(p+i)=0;
	}
	for(i=0;i<72;i++)       //Reading data from sensor and storing it
	{
		gpio_set_value(WRITE,0);
		udelay(1);
		gpio_set_value(WRITE,1);
		udelay(10);
		*(p+i)=gpio_get_value(READ);
		udelay(49);
	}
	bintohex(p,b);    //Conversion of read data from binary to hex to make it more readable
}
static void bintohex(int *p,int b)   //Binary to Hex function
{
	int j=0,value=0;
	int temperature=0;
	printk("Hex Value:");
	for(j=b-1;j>=0;j-=4)
	{
		value=(*(p+j)*8)+(*(p+j-1)*4)+(*(p+j-2)*2)+(*(p+j-3)*1);
		if(j==b-1)
		{
			printk("%0x",value);
		}
		else
		{
			printk("%0x",value);
		}
	}
	temperature=(*(p+4)*1)+(*(p+5)*2)+(*(p+6)*4)+(*(p+7)*8)+(*(p+8)*16)+(*(p+9)*32)+(*(p+10)*64);  //Conversion of binary data into temperature according to Datasheet
	if(*(p+11))
	{
		temperature*=-1;     //if 12th bit of data read is set, temperature is negative
	}
	printk("\nTemperature in celcius: %d",temperature);
	printk("\n");

}
static void configure(int Th,int Tl,int resolution,int *p)    //Function to configure the internal registers of sensor
{															  //Higher Temperature Threshold, Lower Temperature Threshold
	int i;   												  //Resolution
	long int t1,t2;
	t1=ktime_to_ms(ktime_get());
	initialize();
	writerom(0x55);
	for(i=0;i<8;i++)
	{
		writerom(*(p+i));
	}
	writerom(0x4E);
	writerom(Th);	//writing data to Th register
	writerom(Tl);	//writing data to Tl register
	writerom(resolution);	//setting a resolution of the sensor
	t2=ktime_to_ms(ktime_get());
	printk("In Configure:%d",t2-t1);
}
static void address(int *p)
{
	int i,ret;
	long int t1,t2,t3,t4;
	t1=ktime_to_ms(ktime_get());
	initialize();
	writerom(0x55);		//Match ROM Command
	for(i=0;i<8;i++)
	{
		writerom(*(p+i));
	}
	writerom(0x44); //Temperature conversion command
	ret=request_irq(READ,sensori,IRQF_TRIGGER_RISING, "Read",NULL);
	t2=ktime_to_ms(ktime_get());
	printk("In Address:%d",t2-t1);
}
static irqreturn_t sensori(int irq, void *data)
{
	int i;
	initialize();
	writerom(0x55);
	for(i=0;i<8;i++)
	{
		writerom(*(q+i));
	}
	writerom(0xBE);
	readrom(72);
	return IRQ_HANDLED;
}	