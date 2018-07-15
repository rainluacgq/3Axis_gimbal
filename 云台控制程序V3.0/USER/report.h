#ifndef __REPORT_H
#define __REPORT_H
#include "stm32f10x.h"
#include "usart.h"
void usar3_niming_report(u8 fun, u8*data, u8 len);
void mpu6050_send_data(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz);
void usart3_report_imu(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz, short roll, short pitch, short yaw);
#endif

