/*  main.c  */

#include "sys.h"
#include "delay.h"
#include "serial_rtx.h"
#include "usart.h"
#include "led.h"
#include "io.h"
//#include "w25qxx.h"
#include "string.h"
//#include "ff.h"
#include "sdio_sdcard.h"
#include "beep.h"
#include "rtc.h"
#include "timer.h"
#include "key.h"
#include "24cxx.h"
//#include "adc.h"
#include "bme280.h"
#include "bsp_ads1256.h"
#include "main.h"

uint8_t new_file_flag = 0;

uint8_t send_cmd[12] = {0x24,0x30,0x31,0x2C,0x57,0x56,0x3F,0x2A,0x2F,0x2F,0x0D,0x0A};//$01,WV?*//

#define ADC_CHAN_NUM 4
double ADC_value[ADC_CHAN_NUM];

//
struct sys_status status;
struct sys_config config;
struct wind_info  wind;
struct fs_status cur;
struct bme280_dev dev;
struct bme280_data comp_data;

//struct sys_config test_config;
char *check_wind_info(char *str, int len);
void record_file_write(void);
void record_head(void);

struct record_info{
	double wind_speed;
	double wind_direction;
	uint32_t wind_count;
	double temperature;
	double humidity;
	double pressure;
	uint32_t sensor_count;
	double ADC_value0;
	double ADC_value1;
	double ADC_value2;
	double ADC_value3;
	uint32_t ADC_count;
};
struct record_info record;
struct record_info record_old;

#define CONFIG_GPIO     GPIOA
#define CONFIG_PIN			GPIO_Pin_1
#define CONFIG_IO_RCC_CLK RCC_APB2Periph_GPIOA
#define CONFIG_IO_GET_IN()  ((CONFIG_GPIO->IDR & CONFIG_PIN)?(1):(0))
void config_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(CONFIG_IO_RCC_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = CONFIG_PIN;	 	     //端口配置, 推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //上拉输入
	GPIO_Init(CONFIG_GPIO, &GPIO_InitStructure);
}

int main(void)
{
	memset(&cur, 0, sizeof(struct fs_status));
	memset(&status, 0, sizeof(struct sys_status));
	memset(ADC_value,0,sizeof(double)*ADC_CHAN_NUM);
	memset(&record, 0, sizeof(record));
	memset(&record_old, 0, sizeof(record_old));
	delay_init();	    //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	TIM_SetInterval(1,2000);//1ms
	LED_Init();
	IO_Init();
	rain_int_start();
	Beep_Init();
	//Key_Init();
	config_gpio_init();
	AT24CXX_Init();
	
	USART1_Init(115200); //串口1初始化
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	bme280_init(&dev);
	
	bsp_InitADS1256();
	
	ADS1256_CfgADC((ADS1256_GAIN_E)config.ad_gain, ADS1256_5SPS);
	ADS1256_StartScan(1);	
	
	while(1)
	{
		//定时读取温度判断加热器是否开启
		if((tick_count - status.last_sensor) >400) {
			status.last_sensor = tick_count;
			if(BME280_OK == bme280_get_sensor_data(BME280_ALL, &comp_data, &dev)){
				if ( LOW_TEMP_LIMIT > comp_data.temperature ) {
					if (status.heater == 0)
						HEATER_ON;
				} else {
					if(status.heater == 1)
						HEATER_OFF;
				}
			}
		}
		//记录AD转换信息
		if((tick_count - status.last_adc) > 400) {
			status.last_adc = tick_count;
			record.ADC_value0 += (double)ADS1256_GetAdc(0);
			record.ADC_value1 += (double)ADS1256_GetAdc(1);
			record.ADC_value2 += (double)ADS1256_GetAdc(2);
			record.ADC_value3 += (double)ADS1256_GetAdc(3);
			record.ADC_count++;
			if(ADS1256_GetAdc(8)) {
				USART_SendString(USART1," ADS1256 RESET\r\n");
			}
			if(record.ADC_value0 < LOW_LIGHT_LIMIT) {
				FAN_OFF;
			}
		}
		
		if(calendar.hour>3 && calendar.hour<11) {
			//舱门旋转135度
		} else if(calendar.hour>10 && calendar.hour<16) {
			//舱门旋转215度
		}
	}
}

void get_ADC_value(void)
{
	int32_t adc[ADC_CHAN_NUM];
	int i;
	
	for(i=0; i<ADC_CHAN_NUM; i++)
	{
		adc[i] = ADS1256_GetAdc(i);
		ADC_value[i] = ((double)adc[i] * 2500000.0) / 4194303.0;
	}
}
