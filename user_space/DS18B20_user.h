#ifndef DS18B20_USER_H
#define DS18B20_USER_H

void initialize(void);
void writerom(int);
void readrom(int);
void bin2hex(int *,int);
void configure(int,int,int,int*);
void address(int*);
void *sensor(void *arg);


#endif
