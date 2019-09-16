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
#include "TimerPWM.h"
#include "main.h"

uint8_t send_cmd[12] = {0x01,0x03,0x00,0x15,0x00,0x02,0xD5,0xCF};

//
struct sys_status status;
struct sys_config config;
struct bme280_dev dev;
struct bme280_data comp_data;

//struct sys_config test_config;
int check_ad_info(uint8_t *buf,float *ad);
int door_pos_cal(void);
int  loop_idx = 0;

u8 flag=0;//????
u16 t=0;//?????

float light_v =0;
int  old_sec = 0;
int main(void)
{
	memset(&status, 0, sizeof(struct sys_status));

	status.door_cur = 20;
	status.door_exp = 1;
	delay_init();	    //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	TIM_SetInterval(1,2000);//1ms
	LED_Init();
	IO_Init();
	
	POWER_12V_OFF;
	//TEST_ON;
	//rain_int_start();
	Beep_Init();
	AT24CXX_Init();
	
	TIM3_PWM_Init(1000-1,72-1);//72��Ƶ��������1000������Ϊ1ms����ӦƵ��1KHz
	TIM_SetCompare2(TIM3,999);
	
	USART1_Init(115200); //����1��ʼ��
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	USART2_Init(9600); //??2???
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	status.cmd_send_flag = 1;
	LED_ON(LED1);
	while(0){
		TIM_SetCompare2(TIM3,499);
	}
	/*while(AT24CXX_Check())
	{
		delay_ms(500);
		//delay_ms(500);
		LED_TOGGLE(LED1);
		printf("EEPROM Check Failed!\r\n");
	}
	if(CONFIG_GPIO_GET_IN()) {
		while(1) {
			if(usart1_recv_frame_flag) {
				sscanf((char*)usart1_recv, "$%d,%d,%d,%d,%d:%d:%d",&config.baud,
					&config.year,&config.month,&config.date,&config.hour,&config.minute,&config.second);
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
			config.year = 2018;
			config.month = 11;
			config.date = 8;
			config.hour = 0;
			config.minute = 0;
			config.second = 0;
			AT24CXX_Write(0,(u8*)&config,sizeof(config));
		}
		status.rtc_flag = 0;
	}*/
	LED_ON(LED1);
	status.rtc_flag = 1;
	config.year = 2018;
	config.month = 11;
	config.date = 8;
	config.hour = 0;
	config.minute = 0;
	config.second = 0;
	while(RTC_Init(status.rtc_flag,config.year,config.month,config.date,config.hour,config.minute,config.second)) {
		delay_ms(100);
	}
	bme280_init(&dev);
	
	limit1_int_start();
	limit2_int_start();
	limit3_int_start();
	while(1)
	{
		//��ʱ��ȡ�¶��жϼ������Ƿ���
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
			//printf("power off\n");
		} else {
			status.power_220v = 1;
		}
		
		//		
		if(0==RAIN_SENSOR_GPIO_GET_IN()){//Ĭ�ϸߵ�ƽ���͵�ƽ�ز���
			status.rain_status = 1;
			POWER_OFF;
			POWER_12V_OFF;
			status.door_exp = 0;
			status.power_em27 = 0;
			//printf("it's rainning\n");
		} else {
			status.rain_status = 0;
		}
		//��¼ADת����Ϣ
		if(usart2_recv_frame_flag) {
				float ad;
				if(1==check_ad_info((uint8_t*)usart2_recv, &ad)) {
					light_v = ad;
					printf("%f\n",ad);
					if(light_v < LOW_LIGHT_LIMIT) {
						POWER_OFF;
						POWER_12V_OFF;
						status.door_exp = 0;
						status.power_em27 = 0;
						//printf("close cur's light\n");
					}
				}else{
					printf("read solar irradiance failed !\n");
				}
				usart2_recv_frame_flag = 0;
        usart2_recv_cnt = 0;
        memset(usart2_recv,0,32);
    } 
		if (0 == usart2_recv_flag){
			if(status.cmd_send_flag){
				if((tick_count - status.last_cmd_tick) > 400){
					USART_SendBuf(USART2,send_cmd,8);
          status.last_cmd_tick = tick_count;
        }
			} 
    }

		if(status.power_em27 == 0){
			if((status.power_220v==1) && (status.rain_status==0) && (light_v >= LOW_LIGHT_LIMIT)) {
				POWER_ON;
				POWER_12V_ON;
				status.power_em27 = 1;
				status.door_exp = 19;
			}else{
				//printf("exp close!\n");
			}
		}
		//status.door_exp = calendar.min%3;
		
		//����ʱ�����ò���״̬
		if(status.door_exp != 0) {
			/*if((calendar.hour>=MORNING_START_HOUR) && calendar.hour<=AFTERNOON_START_HOUR) {
				//������ת135��
				status.door_exp = 1;
			} else if(calendar.hour>AFTERNOON_START_HOUR && calendar.hour<NIGHT_START_HOUR) {
				//������ת215��
				status.door_exp = 2;
			}	else {
				status.door_exp = 0;
			}*/
			status.door_exp = door_pos_cal();
		}
		//printf("exp,cur : %d,%d\r\n",status.door_exp,status.door_cur);
		if(calendar.sec != old_sec) {
			old_sec = calendar.sec;
			printf("TIME : %04d-%02d-%02d,%02d:%02d:%02d\r\n",calendar.w_year,calendar.w_month,
						calendar.w_date,calendar.hour,calendar.min,calendar.sec);
			printf("door exp pos:%d,door cur pos:%d\n",status.door_exp,status.door_cur);
			printf("power 220V:%d\n",status.power_220v);
			printf("rain status��%d\n",status.rain_status);
			printf("power em27��%d\n",status.power_em27);
		}
		if(status.door_exp != status.door_cur) {
			//move_door();
			TIM_SetCompare2(TIM3,749);
		}
		delay_ms(10);
	}
}


//����յ�������ַ����ĸ�ʽ
int check_ad_info(uint8_t *buf,float *ad)
{
	//uint16_t crc16;
	uint8_t *temp;
	if((buf[0]!=0x01) || (buf[1]!=0x03) || (buf[2]!= 0x04)){
		return 0;
	}
	/*memcpy(&crc16,buf+6,2);
	if(crc16!=crc16_cal(buf,7)){
		return 0;
	}*/
	temp = (uint8_t *)ad;
	
	temp[0] = buf[4];
	temp[1] = buf[3];
	temp[2] = buf[6];
	temp[3] = buf[5];
	
	return 1;
}

int door_pos_cal(void)
{
	uint32_t cur_sec;
	int pos=0;
	cur_sec = calendar.hour*3600+calendar.min*60+calendar.sec;
	if((cur_sec>=MORNING_START) && (cur_sec<AFTERNOON_START)) {
		pos = 1;
	} else if((cur_sec>=AFTERNOON_START)&&(cur_sec<NIGHT_START)){
		pos = 2;
	} else {
		pos =0;
	}
	
	return pos;
}
