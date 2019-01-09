#include "io.h"
    
//GPIO IO初始化
void IO_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 uint32_t peri;
	
 peri = HEATER_GPIO_RCC_CLK|RAIN_SENSOR_GPIO_RCC_CLK|POWER_GPIO_RCC_CLK |CONFIG_GPIO_RCC_CLK ;
	
 RCC_APB2PeriphClockCmd(peri, ENABLE);	 //使能时钟
 //加热继电器IO初始化
 GPIO_InitStructure.GPIO_Pin = HEATER_PIN;				       //加热器端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(HEATER_GPIO, &GPIO_InitStructure);					   //根据设定参数初始化加热器
 GPIO_ResetBits(HEATER_GPIO,HEATER_PIN);						         //控制管脚输出低

	//雨感器中断IO初始化
	GPIO_InitStructure.GPIO_Pin = RAIN_SENSOR_PIN;
	GPIO_Init(RAIN_SENSOR_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = POWER_PIN;
	GPIO_Init(POWER_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CONFIG_PIN;	 	     //端口配置, 推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //上拉输入
	GPIO_Init(CONFIG_GPIO, &GPIO_InitStructure);
}
 
void rain_int_start(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Connect EXTI4 Line to PE3 pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);

	/* Configure EXTI4 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  /* 下降沿 */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI4 Interrupt  priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void rain_int_stop(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* 配置 EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	/* 下降沿 */
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;		/* 禁止 */
	EXTI_Init(&EXTI_InitStructure);

	/* 中断优先级配置 最低优先级 这里一定要分开的设置中断，不能够合并到一个里面设置 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;		/* 禁止 */
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI4_IRQHandler(void)
{
	POWER_OFF;
}
