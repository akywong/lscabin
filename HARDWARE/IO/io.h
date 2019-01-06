#ifndef __IO_H_
#define __IO_H_

#include "sys.h"


/***************** IO���������궨�� *********************/
#define IO_ON(x)      x=1  
#define IO_OFF(x)     x=0
#define IO_TOGGLE(x)  x^=1
/****************************** end *********************************/

/***************** GPIO��ʱ�Ӻ궨�� ***************************/
#define HEATER_GPIO_RCC_CLK  RCC_APB2Periph_GPIOB//���������ؿ��ƹܽ�
/****************************** end *********************************/


/********************* LED����GPIO�ں궨�� ***************************/
#define HEATER_GPIO  GPIOB

#define HEATER_PIN   GPIO_Pin_7


#define HEATER PBout(7)// PB5

#define HEATER_ON   IO_ON(HEATER)
#define HEATER_OFF  IO_OFF(HEATER)

/****************************** end *********************************/
//������ж�IO
#define RAIN_SENSOR_GPIO_RCC_CLK 	RCC_APB2Periph_GPIOA
#define RAIN_SENSOR_GPIO	GPIOA
#define RAIN_SENSOR_PIN	GPIO_Pin_3

//���ȿ���IO
#define FAN_GPIO_RCC_CLK RCC_APB2Periph_GPIOA
#define FAN_GPIO   GPIOA
#define FAN_PIN    GPIO_Pin_7

#define FAN_ON   IO_ON(FAN)
#define FAN_OFF  IO_OFF(FAN)

#define FAN     PAout(7)

#define FAN_ON   IO_ON(FAN)
#define FAN_OFF  IO_OFF(FAN)

/************************* �������� *********************************/
void IO_Init(void);//��ʼ��

void rain_int_start(void);
void rain_int_stop(void);

/****************************** end *********************************/






		 				    
#endif
