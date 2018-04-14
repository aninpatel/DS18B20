//Inclusion of Linux Header Files
#include "DS18B20.h"

#define LED 25
#define WRITE 17				//Defining GPIO 17 as a WRTE Pin
#define READ 4					//Defining GPIO 4 as a READ Pin
#define ROM 64					//Defining 64 bit ROM for Sensor as ROM
#define DATA 72					//72 bits data to be read from Sensor

static int add_device1[8]={0x28,0x11,0x1b,0xde,0x07,0x00,0x00,0x10};	//Address of the sensors connected to GPIO
static int add_device2[8]={0x28,0x9f,0x48,0xde,0x07,0x00,0x00,0x32};
static int add_device3[8]={0x28,0x04,0xbe,0xde,0x07,0x00,0x00,0x41};
static int add_device4[8]={0x28,0x43,0xfa,0xde,0x07,0x00,0x00,0x47};
static int add_device5[8]={0x28,0x92,0xcc,0xde,0x07,0x00,0x00,0xac};

DEFINE_MUTEX(lock);		//Defining Mutex
static struct task_struct *thread[5];	//Defining array of 5 threads
unsigned long long js,es=0,js1,es1;		//Variables to hold measured time
unsigned long difference=0;						//Variable to hold total time calculated
struct timeval st,et;
static int count=0;
int sensor(void *arg)	//Functions to be executed by thread (Critical Region)
{
	mutex_lock(&lock);	//Locking the critical region so that only one thread at a time executes this Function
	count++;
	//unsigned long long start,end;
	js=ktime_to_us(ktime_get());	//measuring time in microseconds
	js1=ktime_to_ms(ktime_get());	//measuring time in milliseconds
	printk("Sensor %d is being operated\n",count);
	if(count==1)
	{
		printk("Context Switch Time:%llu\n",js-js);		//Measuring the Context Switch time
		configure(0x7F,0x80,0x7F,add_device1);	//Configuring a specific device(sensor)
		address(add_device1);				//Writing commands to and reading data from a sensor
	}
	else if(count==2)
	{
		printk("Context Switch Time:%llu\n",js-es);		//Measuring the Context Switch time
		configure(0x7F,0x80,0x7F,add_device2);
		address(add_device2);
	}
	else if(count==3)
	{
		printk("Context Switch Time:%llu\n",js-es);		//Measuring the Context Switch time
		configure(0x7F,0x80,0x7F,add_device3);
		address(add_device3);
	}
	else if(count==4)
	{
		printk("Context Switch Time:%llu\n",js-es);		//Measuring the Context Switch time
		configure(0x7F,0x80,0x7F,add_device4);
		address(add_device4);
	}
	else
	{
		printk("Context Switch Time:%llu\n",js-es);		//Measuring the Context Switch time
		configure(0x7F,0x80,0x7F,add_device5);
		address(add_device5);
	}
	es=ktime_to_us(ktime_get());
	es1=ktime_to_ms(ktime_get());
	difference+=es1-js1;					//measuring a latency till now
	if(count==5)
	{
		printk("Time elapsed=%lu\n",difference);
	}
	mutex_unlock(&lock);					//Unlocking a critical Region
	//do_exit(0);
	return 0;
}
static int thread_init (void)		//Function executed when Kernel Module is loaded
{
	int ret = 0,flag=0,j=0;
	//unsigned long time;
	//unsigned long long t1,t2,gs,ge;
	printk(KERN_INFO "Module Loaded\n");
	ret=gpio_request_one(WRITE,GPIOF_OUT_INIT_LOW,"write_pin");	//Defining GPIO 17 as an output
	if(ret)
	{
		printk(KERN_ALERT "Pin %d could not be assigned",ret);		//Priniting message in log file
		flag=1;
	}
	ret=gpio_request_one(READ,GPIOF_IN,"read_pin");							//Defining GPIO 4 as an input
	if(ret)
	{
    		printk(KERN_ALERT "Pin %d could not be assigned",ret);
		flag=1;
  	}
  	printk(KERN_INFO "in init");
  	for(j=0;j<5;j++)
  	{
  		thread[j]= kthread_run(&sensor,NULL,"Sensor");			//Creating 5 threads for 5 sensors
  	}
	//printk("Time elapsed=%lu\n",difference);
  	return 0;
}
static void thread_cleanup(void)		//function to be executed when module is unloaded from kernel
{
	printk(KERN_INFO "%s\n", __func__);
	gpio_set_value(WRITE,0);	//Setting WRITE Pin to zero
	gpio_free(WRITE);					//Releasing
	gpio_free(READ);					//the GPIOs

}
static void initialize(void)			//initialize function for Initializing the Sensor
{
	gpio_set_value(WRITE,0);
	udelay(480);
	gpio_set_value(WRITE,1);
	udelay(480);
}

MODULE_LICENSE("GPL");
module_init(thread_init);
module_exit(thread_cleanup);
