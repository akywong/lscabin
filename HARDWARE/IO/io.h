#ifndef __IO_H_
#define __IO_H_

#include "sys.h"


/***************** IO操作函数宏定义 *********************/
#define IO_ON(x)      x=1  
#define IO_OFF(x)     x=0
#define IO_TOGGLE(x)  x^=1
/****************************** end *********************************/

/***************** GPIO口时钟宏定义 ***************************/
#define HEATER_GPIO_RCC_CLK  RCC_APB2Periph_GPIOB//加热器开关控制管脚
/****************************** end *********************************/


/********************* LED所在GPIO口宏定义 ***************************/
#define HEATER_GPIO  GPIOB

#define HEATER_PIN   GPIO_Pin_7


#define HEATER PBout(7)// PB5

#define HEATER_ON   IO_ON(HEATER)
#define HEATER_OFF  IO_OFF(HEATER)

/****************************** end *********************************/
//雨感器中断IO
#define RAIN_SENSOR_GPIO_RCC_CLK 	RCC_APB2Periph_GPIOA
#define RAIN_SENSOR_GPIO	GPIOA
#define RAIN_SENSOR_PIN	GPIO_Pin_3

//风扇控制IO
#define FAN_GPIO_RCC_CLK RCC_APB2Periph_GPIOA
#define FAN_GPIO   GPIOA
#define FAN_PIN    GPIO_Pin_7

#define FAN_ON   IO_ON(FAN)
#define FAN_OFF  IO_OFF(FAN)

#define FAN     PAout(7)

#define FAN_ON   IO_ON(FAN)
#define FAN_OFF  IO_OFF(FAN)

/************************* 函数声明 *********************************/
void IO_Init(void);//初始化

void rain_int_start(void);
void rain_int_stop(void);

/****************************** end *********************************/






		 				    
#endif
