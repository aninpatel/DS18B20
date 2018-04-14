#ifndef DS18B20_H
#define DS18B20_H

static void initialize(void);	//Function to Initialize the Sensor
static void readrom(int);			//Function to read data from the Sensor
static void writerom(int);		//Function to write data from the Sensor
static void bintohex(int*,int);	//Function to convert binary data read from data to hex
static void configure(int,int,int,int*);	//Function to configure the sensor
static void address(int*);			//Function to give commands to specific sensor
static irqreturn_t sensori(int irq, void *data);

#endif