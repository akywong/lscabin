#ifndef __IO_H_
#define __IO_H_

#include "sys.h"


/***************** IO���������궨�� *********************/
#define IO_ON(x)      x=1  
#define IO_OFF(x)     x=0
#define IO_TOGGLE(x)  x^=1
/****************************** end *********************************/

//���ȵ�Դ����IO--PE0
#define HEATER_GPIO_RCC_CLK RCC_APB2Periph_GPIOE
#define HEATER_GPIO  GPIOE
#define HEATER_PIN   GPIO_Pin_0
#define HEATER PEout(0)

#define HEATER_ON   IO_ON(HEATER)
#define HEATER_OFF  IO_OFF(HEATER)

//220V��Դ���IO
#define POWER_CHECK_GPIO_RCC_CLK  RCC_APB2Periph_GPIOC
#define POWER_CHECK_GPIO  GPIOC
#define POWER_CHECK_PIN   GPIO_Pin_0
#define POWER_CHECK_GET_IN()  ((POWER_CHECK_GPIO->IDR & POWER_CHECK_PIN)?(1):(0))  


//������ж�IO--PC1
#define RAIN_SENSOR_GPIO_RCC_CLK 	RCC_APB2Periph_GPIOC
#define RAIN_SENSOR_GPIO	GPIOC
#define RAIN_SENSOR_PIN	GPIO_Pin_1
#define RAIN_SENSOR_GPIO_GET_IN()  ((RAIN_SENSOR_GPIO->IDR & RAIN_SENSOR_PIN)?(1):(0))

//EM27��Դ����IO--PE1
#define POWER_GPIO_RCC_CLK RCC_APB2Periph_GPIOE
#define POWER_GPIO   GPIOE
#define POWER_PIN    GPIO_Pin_1
#define POWER    PEout(1)

#define POWER_ON   IO_ON(POWER)
#define POWER_OFF  IO_OFF(POWER)

//����ѡ��IO--PA1
#define CONFIG_GPIO     GPIOA
#define CONFIG_PIN			GPIO_Pin_1
#define CONFIG_GPIO_RCC_CLK RCC_APB2Periph_GPIOA
#define CONFIG_GPIO_GET_IN()  ((CONFIG_GPIO->IDR & CONFIG_PIN)?(1):(0))

//��λ�����ж�IO--PC4,PC5,PC6
#define LIMIT_GPIO_RCC_CLK 	RCC_APB2Periph_GPIOC
#define LIMIT_GPIO  GPIOC
#define LIMIT_PIN1  GPIO_Pin_4
#define LIMIT_PIN2  GPIO_Pin_5
#define LIMIT_PIN3  GPIO_Pin_6
#define LIMIT1_GPIO_GET_IN()  ((LIMIT_GPIO->IDR & LIMIT_PIN1)?(1):(0))
#define LIMIT2_GPIO_GET_IN()  ((LIMIT_GPIO->IDR & LIMIT_PIN2)?(1):(0))
#define LIMIT3_GPIO_GET_IN()  ((LIMIT_GPIO->IDR & LIMIT_PIN3)?(1):(0))

#define LIMIT_GPIO_RCC_CLK RCC_APB2Periph_GPIOC
/************************* �������� *********************************/
void IO_Init(void);//��ʼ��

void rain_int_start(void);
void rain_int_stop(void);
	
void limit1_int_start(void);
void limit1_int_stop(void);
	
void limit2_int_start(void);
void limit2_int_stop(void);
	
void limit3_int_start(void);
void limit3_int_stop(void);

/****************************** end *********************************/






		 				    
#endif
