#ifndef __IO_H_
#define __IO_H_

#include "sys.h"


/***************** IO���������궨�� *********************/
#define IO_ON(x)      x=1  
#define IO_OFF(x)     x=0
#define IO_TOGGLE(x)  x^=1
/****************************** end *********************************/

//���ȵ�Դ����IO--PC3
#define HEATER_GPIO_RCC_CLK RCC_APB2Periph_GPIOC
#define HEATER_GPIO  GPIOC
#define HEATER_PIN   GPIO_Pin_3
#define HEATER PCout(3)

#define HEATER_ON   IO_ON(HEATER)
#define HEATER_OFF  IO_OFF(HEATER)

//220V��Դ���IO--PE1
#define POWER_CHECK_GPIO_RCC_CLK  RCC_APB2Periph_GPIOE
#define POWER_CHECK_GPIO  GPIOE
#define POWER_CHECK_PIN   GPIO_Pin_1
#define POWER_CHECK_GET_IN()  ((POWER_CHECK_GPIO->IDR & POWER_CHECK_PIN)?(1):(0))  


//������ж�IO--PC0
#define RAIN_SENSOR_GPIO_RCC_CLK 	RCC_APB2Periph_GPIOC
#define RAIN_SENSOR_GPIO	GPIOC
#define RAIN_SENSOR_PIN	GPIO_Pin_0
#define RAIN_SENSOR_GPIO_GET_IN()  ((RAIN_SENSOR_GPIO->IDR & RAIN_SENSOR_PIN)?(1):(0))

//EM27��Դ����IO--PE0
#define POWER_GPIO_RCC_CLK RCC_APB2Periph_GPIOE
#define POWER_GPIO   GPIOE
#define POWER_PIN    GPIO_Pin_0
#define POWER    PEout(0)

#define POWER_OFF   IO_ON(POWER)
#define POWER_ON  IO_OFF(POWER)

//12V��Դ����IO--PC1
#define POWER_12V_GPIO_RCC_CLK RCC_APB2Periph_GPIOC
#define POWER_12V_GPIO   GPIOC
#define POWER_12V_PIN    GPIO_Pin_1
#define POWER_12V    PCout(1)

#define POWER_12V_ON   IO_OFF(POWER_12V)
#define POWER_12V_OFF  IO_ON(POWER_12V)

//12V��Դ����IO--PA12
#define TEST_GPIO_RCC_CLK RCC_APB2Periph_GPIOA
#define TEST_GPIO   GPIOA
#define TEST_PIN    GPIO_Pin_12
#define TEST    PAout(12)

#define TEST_ON   IO_ON(TEST)
#define TEST_OFF  IO_OFF(TEST)

//����ѡ��IO--PA1
#define CONFIG_GPIO     GPIOA
#define CONFIG_PIN			GPIO_Pin_1
#define CONFIG_GPIO_RCC_CLK RCC_APB2Periph_GPIOA
#define CONFIG_GPIO_GET_IN()  ((CONFIG_GPIO->IDR & CONFIG_PIN)?(1):(0))

//��λ�����ж�IO--PC4,PC5,PC6
#define LIMIT_GPIO_RCC_CLK 	RCC_APB2Periph_GPIOC
#define LIMIT_GPIO  GPIOC
#define LIMIT_PIN1  GPIO_Pin_5
#define LIMIT_PIN2  GPIO_Pin_4
#define LIMIT_PIN3  GPIO_Pin_7
#define LIMIT_PIN4  GPIO_Pin_9
#define LIMIT_PIN5  GPIO_Pin_11
#define LIMIT1_GPIO_GET_IN()  ((LIMIT_GPIO->IDR & LIMIT_PIN1)?(1):(0))
#define LIMIT2_GPIO_GET_IN()  ((LIMIT_GPIO->IDR & LIMIT_PIN2)?(1):(0))
#define LIMIT3_GPIO_GET_IN()  ((LIMIT_GPIO->IDR & LIMIT_PIN3)?(1):(0))
#define LIMIT4_GPIO_GET_IN()  ((LIMIT_GPIO->IDR & LIMIT_PIN4)?(1):(0))
#define LIMIT5_GPIO_GET_IN()  ((LIMIT_GPIO->IDR & LIMIT_PIN5)?(1):(0))

#define LIMIT_GPIO_GET_INT(n) ((LIMIT_GPIO->IDR & LIMIT_PIN##n)?(1):(0))

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

void limit4_int_start(void);
void limit4_int_stop(void);

void limit5_int_start(void);
void limit5_int_stop(void);

/****************************** end *********************************/






		 				    
#endif
