#include "DS18B20.h"

#define LED 25
#define WRITE 17		//Defining GPIO 17 as a WRTE Pin
#define READ 4			//Defining GPIO 4 as a READ Pin
#define ROM 64			//Defining 64 bit ROM for Sensor as ROM
#define DATA 72			//72 bits data to be read from Sensor

static int add_device1[8]={0x28,0x11,0x1b,0xde,0x07,0x00,0x00,0x10};	//Address of the sensors connected to GPIO
static int add_device2[8]={0x28,0x9f,0x48,0xde,0x07,0x00,0x00,0x32};
static int add_device3[8]={0x28,0x04,0xbe,0xde,0x07,0x00,0x00,0x41};
static int add_device4[8]={0x28,0x43,0xfa,0xde,0x07,0x00,0x00,0x47};
static int add_device5[8]={0x28,0x92,0xcc,0xde,0x07,0x00,0x00,0xac};
static int sensor_irq=-1;
//unsigned long js,es;		//Variables to hold measured time
//struct timeval st,et;
int *q=add_device1;

static int __init gpiomod_init(void)	//Function executed when Kernel Module is loaded
{
	int ret = 0,flag=0;
	unsigned long long t1,t2;
	printk(KERN_INFO "Module Loaded\n");
	ret=gpio_request_one(WRITE,GPIOF_OUT_INIT_LOW,"write_pin");	  //Defining GPIO 17 as an output
	if(ret)
	{
		printk(KERN_ALERT "Pin %d could not be assigned",ret);   //Priniting message in log file
		flag=1;
	}
	ret=gpio_request_one(READ,GPIOF_IN,"read_pin");				//Defining GPIO 4 as an input
	if(ret)
        {
                printk(KERN_ALERT "Pin %d could not be assigned",ret);
		flag=1;
        }
	ret=gpio_to_irq(READ);
	if(ret<0)
	{
		printk("Error");
	}
	sensor_irq=ret;
	t1=ktime_to_ms(ktime_get());
	//rdtscl(t1);
	configure(0x7F,0x80,0x7F,add_device1);		//Configuring a specific device(sensor)
	q=add_device1;
	address(add_device1);						//Writing commands to and reading data from a sensor
	configure(0x7F,0x80,0x7F,add_device2);
	q=add_device2;
	address(add_device2);
	configure(0x7F,0x80,0x7F,add_device3);
	q=add_device3;
	address(add_device3);
	configure(0x7F,0x80,0x7F,add_device4);
	q=add_device4;
	address(add_device4);
	configure(0x7F,0x80,0x7F,add_device5);
	q=add_device5;
	address(add_device5);
	//t2=ktime_to_ms(ktime_get());
	//rdtscl(t2);
	printk("Time elapased:%llu\n",t2-t1);
	return flag;
}
static void __exit gpiomod_exit(void)
{
	printk(KERN_INFO "%s\n", __func__);
	gpio_set_value(WRITE,0);
	gpio_free(WRITE);
	gpio_free(READ);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aniruddha Patel");
MODULE_DESCRIPTION("Basic Linux Kernel module using GPIOs to read DS18B20 Temperature Sensor");

module_init(gpiomod_init);
module_exit(gpiomod_exit);
