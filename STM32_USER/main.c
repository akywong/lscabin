/*  main.c  */

#include "sys.h"
#include "delay.h"
#include "serial_rtx.h"
#include "usart.h"
#include "led.h"
#include "io.h"
#include "string.h"
#include "beep.h"
#include "rtc.h"
#include "timer.h"
#include "key.h"
#include "24cxx.h"
#include "bme280.h"
#include "bsp_ads1256.h"
#include "TimerPWM.h"
#include "main.h"

uint8_t new_file_flag = 0;

uint8_t send_cmd[12] = {0x24,0x30,0x31,0x2C,0x57,0x56,0x3F,0x2A,0x2F,0x2F,0x0D,0x0A};//$01,WV?*//

#define ADC_CHAN_NUM 4
double ADC_value[ADC_CHAN_NUM];

//
struct sys_status status;
struct sys_config config;
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

int main(void)
{
	memset(&status, 0, sizeof(struct sys_status));
	memset(ADC_value,0,sizeof(double)*ADC_CHAN_NUM);
	delay_init();	    //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	TIM_SetInterval(1,2000);//1ms
	LED_Init();
	IO_Init();
	//rain_int_start();
	Beep_Init();
	AT24CXX_Init();
	
	TIM3_PWM_Init(1000-1,72-1);//72分频，计数到1000，周期为1ms，对应频率1KHz
	//TIM_SetCompare2(TIM3,499);//设置占空比为500us，即50%占空比
	TIM_SetCompare2(TIM3,0);
	
	USART1_Init(115200); //串口1初始化
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	LED_ON(LED1);
	while(AT24CXX_Check())
	{
		delay_ms(500);
		//delay_ms(500);
		LED_TOGGLE(LED1);
		printf("EEPROM Check Failed!\r\n");
	}
	if(CONFIG_GPIO_GET_IN()) {
		while(1) {
			if(usart1_recv_frame_flag) {
				sscanf((char*)usart1_recv, "$%d,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d:%d:%d,%d,%d",&config.baud,
					&config.cal[0].A,&config.cal[0].B,&config.cal[1].A,&config.cal[1].B,&config.cal[2].A,&config.cal[2].B,&config.cal[3].A,&config.cal[3].B,
					&config.year,&config.month,&config.date,&config.hour,&config.minute,&config.second,&config.ad_gain,&config.freq);
				usart1_recv_frame_flag = 0;
				if(config.year == 0) {
					status.rtc_flag =0;
				} else {
					status.rtc_flag = 1;
				}
				USART_SendString(USART1,usart1_recv);
				config.head = 0xAA5555AA;
				config.tail = 0xAA5555AA;
				AT24CXX_Write(0,(u8*)&config,sizeof(config));
				//AT24CXX_Read(0,(u8*)&test_config,sizeof(config));
				break;
			}
			delay_ms(50);
		}
	} else {	
		AT24CXX_Read(0,(u8*)&config,sizeof(config));
		if((config.head != 0xAA5555AA) || (config.tail != 0xAA5555AA)){
			config.head = 0xAA5555AA;
			config.tail = 0xAA5555AA;
			config.baud = 9600;
			config.cal[0].A = 1.0;
			config.cal[0].B = 0;
			config.cal[1].A = 1.0;
			config.cal[1].B = 0;
			config.cal[2].A = 1.0;
			config.cal[2].B = 0;
			config.cal[3].A = 1.0;
			config.cal[3].B = 0;
			config.year = 2018;
			config.month = 11;
			config.date = 8;
			config.hour = 0;
			config.minute = 0;
			config.second = 0;
			config.freq = 1;
			config.ad_gain = ADS1256_GAIN_1;
			AT24CXX_Write(0,(u8*)&config,sizeof(config));
		}
		status.rtc_flag = 0;
	}
	printf("usart2 baud : %d\r\n",config.baud);
	printf("ch 0 A : %f\r\n",config.cal[0].A);
	printf("ch 0 B : %f\r\n",config.cal[0].B);
	printf("ch 1 A : %f\r\n",config.cal[1].A);
	printf("ch 1 B : %f\r\n",config.cal[1].B);
	printf("ch 2 A : %f\r\n",config.cal[2].A);
	printf("ch 2 B : %f\r\n",config.cal[2].B);
	printf("ch 3 A : %f\r\n",config.cal[3].A);
	printf("ch 3 B : %f\r\n",config.cal[3].B);
	printf("ad_gain : %d\r\n",config.ad_gain);
	printf("freq : %d\r\n",config.freq);
	LED_ON(LED1);
	
	while(RTC_Init(status.rtc_flag,config.year,config.month,config.date,config.hour,config.minute,config.second)) {
		delay_ms(100);
	}
	bme280_init(&dev);
	
	bsp_InitADS1256();
	
	ADS1256_CfgADC((ADS1256_GAIN_E)config.ad_gain, ADS1256_5SPS);
	ADS1256_StartScan(1);	
	
	limit1_int_start();
	limit2_int_start();
	limit3_int_start();
	
	while(1)
	{
		//定时读取温度判断加热器是否开启
		if((tick_count - status.last_sensor) >400) {
			status.last_sensor = tick_count;
			if(BME280_OK == bme280_get_sensor_data(BME280_ALL, &comp_data, &dev)){
				if ( LOW_TEMP_LIMIT > comp_data.temperature ) {
					if (status.heater_status == 0){
						HEATER_ON;
						status.heater_status = 1;
					}
				} else if(HIGH_TEMP_LIMIT < comp_data.temperature){
					if(status.heater_status == 1) {
						HEATER_OFF;
						status.heater_status = 0;
					}
				} 
			}
		}
		
		//
		if(POWER_CHECK_GET_IN() == 0 ) {
			status.power_220v = 0;
			POWER_OFF;
			status.power_em27 = 0;
			status.door_exp = 0;	
			USART_SendString(USART1,"STOP");
		} else {
			status.power_220v = 1;
		}
		
		//		
		if(RAIN_SENSOR_GPIO_GET_IN()){
			status.rain_status = 1;
			POWER_OFF;
			status.door_exp = 0;
			status.power_em27 = 0;
			USART_SendString(USART1,"STOP");
		} else {
			status.rain_status = 0;
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
				POWER_OFF;
				status.door_exp = 0;
				status.power_em27 = 0;
				USART_SendString(USART1,"STOP");
			}
		}
		if(status.power_em27 == 0){
			if((status.power_220v)==1 && (status.rain_status==0) && (record.ADC_value0 >= LOW_LIGHT_LIMIT)) {
				POWER_ON;
				status.power_em27 = 1;
				USART_SendString(USART1,"START");
			}
		}
		if(status.door_exp != 0) {
			if(calendar.hour>3 && calendar.hour<11) {
				//舱门旋转135度
				status.door_exp = 1;
			} else if(calendar.hour>10 && calendar.hour<16) {
				//舱门旋转215度
				status.door_exp = 2;
			}
		}
		
		if(status.door_exp != status.door_exp) {
			//move_door();
			TIM_SetCompare2(TIM3,499);
		}
		delay_ms(100);
	}
}

/*void get_ADC_value(void)
{
	int32_t adc[ADC_CHAN_NUM];
	int i;
	
	for(i=0; i<ADC_CHAN_NUM; i++)
	{
		adc[i] = ADS1256_GetAdc(i);
		ADC_value[i] = ((double)adc[i] * 2500000.0) / 4194303.0;
	}
}*/
